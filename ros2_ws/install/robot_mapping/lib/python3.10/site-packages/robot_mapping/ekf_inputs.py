"""ROS-independent parsing and pose differentiation for the EKF bridge."""
from dataclasses import dataclass
import math


class LineBuffer:
    """Retain incomplete serial lines; reject an overlong line through its newline."""

    def __init__(self, limit=4096):
        self.buffer = bytearray()
        self.limit = limit
        self.discarding = False

    def feed(self, data):
        lines = []
        for byte in data:
            if byte == 10:
                if not self.discarding:
                    lines.append(self.buffer.decode('ascii', errors='replace').strip())
                self.buffer.clear()
                self.discarding = False
            elif not self.discarding:
                self.buffer.append(byte)
                if len(self.buffer) > self.limit:
                    self.buffer.clear()
                    self.discarding = True
        return lines


def parse_record(line):
    """Return finite POSE/IMU fields only; debug/scan lines are ignored."""
    parts = line.split(',')
    count = {'POSE': 3, 'IMU': 6}.get(parts[0])
    if count is None or len(parts) != count + 1:
        return None
    try:
        values = tuple(float(part) for part in parts[1:])
    except ValueError:
        return None
    if not all(math.isfinite(value) for value in values):
        return None
    return parts[0], values


@dataclass(frozen=True)
class Motion:
    x: float
    y: float
    yaw: float
    vx: float
    yaw_rate: float


class PoseVelocity:
    """Differentiate already sign-corrected firmware pose using host monotonic time.

    Too-short samples accumulate; gaps and implausible jumps rebase without
    publishing velocity. This cannot replace firmware acquisition timestamps.
    """

    def __init__(self, min_dt=0.03, max_dt=0.5,
                 max_linear_speed=0.6, max_yaw_rate=6.0):
        self.min_dt = min_dt
        self.max_dt = max_dt
        self.max_linear_speed = max_linear_speed
        self.max_yaw_rate = max_yaw_rate
        self.previous = None
        self.reason = 'first sample'

    def reset(self):
        self.previous = None
        self.reason = 'reset'

    def update(self, pose, now):
        x, y, yaw = pose
        if not all(math.isfinite(v) for v in (*pose, now)):
            self.reason = 'nonfinite'
            return None
        current = (x, y, yaw, now)
        if self.previous is None:
            self.previous = current
            self.reason = 'first sample'
            return None
        px, py, pyaw, before = self.previous
        dt = now - before
        if dt <= 0 or dt > self.max_dt:
            self.previous = current
            self.reason = 'clock or sample gap'
            return None
        if dt < self.min_dt:
            self.reason = 'accumulating'
            return None
        delta = math.atan2(math.sin(yaw - pyaw), math.cos(yaw - pyaw))
        midpoint = pyaw + delta / 2.0
        dx, dy = x - px, y - py
        vx = (dx * math.cos(midpoint) + dy * math.sin(midpoint)) / dt
        wz = delta / dt
        self.previous = current
        if math.hypot(dx, dy) / dt > self.max_linear_speed or abs(wz) > self.max_yaw_rate:
            self.reason = 'implausible motion or pose reset'
            return None
        self.reason = 'valid'
        return Motion(x, y, yaw, vx, wz)
