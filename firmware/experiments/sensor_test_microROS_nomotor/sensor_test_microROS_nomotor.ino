#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox;

// =====================================================
// I2C
// =====================================================

// LEFT AS5600 + VL53L0X
#define LEFT_SDA 21
#define LEFT_SCL 22

// RIGHT AS5600
#define RIGHT_SDA 16
#define RIGHT_SCL 23

TwoWire I2C_Right = TwoWire(1);

// =====================================================
// AS5600
// =====================================================

#define AS5600_ADDR 0x36
#define RAW_ANGLE_HIGH 0x0C

// =====================================================
// ROBOT DIMENSIONS
// =====================================================

const float WHEEL_DIAMETER = 0.044;  // 44 mm
const float WHEEL_TRACK = 0.100;     // 100 mm

const float WHEEL_CIRCUMFERENCE =
  PI * WHEEL_DIAMETER;

// =====================================================
// ENCODERS
// =====================================================

uint16_t leftPrevious = 0;
uint16_t rightPrevious = 0;

long leftTotalCounts = 0;
long rightTotalCounts = 0;

// =====================================================
// ROBOT POSE
// =====================================================

float robotX = 0.0;
float robotY = 0.0;

// Radians
// CCW = positive
float heading = 0.0;

// =====================================================
// READ AS5600
// =====================================================

uint16_t readAS5600(TwoWire &bus) {
  bus.beginTransmission(AS5600_ADDR);
  bus.write(RAW_ANGLE_HIGH);

  if (bus.endTransmission(false) != 0) {
    return 0;
  }

  bus.requestFrom(AS5600_ADDR, 2);

  if (bus.available() < 2) {
    return 0;
  }

  uint8_t highByte = bus.read();
  uint8_t lowByte = bus.read();

  uint16_t angle =
    ((uint16_t)highByte << 8) | lowByte;

  return angle & 0x0FFF;
}

// =====================================================
// HANDLE AS5600 WRAPAROUND
// =====================================================

int16_t angleDifference(
  uint16_t current,
  uint16_t previous) {
  int16_t difference =
    (int16_t)current - (int16_t)previous;

  if (difference > 2048) {
    difference -= 4096;
  }

  if (difference < -2048) {
    difference += 4096;
  }

  return difference;
}

// =====================================================
// UPDATE ODOMETRY
// =====================================================

void updateOdometry() {
  uint16_t leftCurrent =
    readAS5600(Wire);

  uint16_t rightCurrent =
    readAS5600(I2C_Right);

  int16_t leftDelta =
    angleDifference(
      leftCurrent,
      leftPrevious);

  int16_t rightDelta =
    angleDifference(
      rightCurrent,
      rightPrevious);

  leftPrevious = leftCurrent;
  rightPrevious = rightCurrent;

  // -------------------------------------------------
  // YOUR ENCODERS ARE REVERSED
  // Forward = negative encoder delta
  //
  // Correcting the sign:
  // -------------------------------------------------

  leftDelta = -leftDelta;
  rightDelta = -rightDelta;

  // -------------------------------------------------
  // TOTAL COUNTS
  // -------------------------------------------------

  leftTotalCounts += leftDelta;
  rightTotalCounts += rightDelta;

  // -------------------------------------------------
  // DISTANCE TRAVELED
  // -------------------------------------------------

  float deltaLeft =
    (leftDelta / 4096.0) * WHEEL_CIRCUMFERENCE;

  float deltaRight =
    (rightDelta / 4096.0) * WHEEL_CIRCUMFERENCE;

  // -------------------------------------------------
  // DIFFERENTIAL DRIVE
  // -------------------------------------------------

  float deltaDistance =
    (deltaLeft + deltaRight) / 2.0;

  // CCW = positive
  float deltaHeading =
    (deltaLeft - deltaRight)
    / WHEEL_TRACK;

  heading += deltaHeading;

  // -------------------------------------------------
  // UPDATE X/Y
  // -------------------------------------------------

  float middleHeading =
    heading - (deltaHeading / 2.0);

  robotX +=
    deltaDistance * cos(middleHeading);

  robotY +=
    deltaDistance * sin(middleHeading);
}

// =====================================================
// READ VL53L0X
// =====================================================

int readDistance() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(
    &measure,
    false);

  if (measure.RangeStatus != 4) {
    return measure.RangeMilliMeter;
  }

  return -1;
}

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println(" ROBOT ODOMETRY + VL53L0X");
  Serial.println("================================");

  // =================================================
  // I2C BUS 0
  //
  // LEFT AS5600 + VL53L0X
  // =================================================

  Wire.begin(
    LEFT_SDA,
    LEFT_SCL,
    400000);

  // =================================================
  // I2C BUS 1
  //
  // RIGHT AS5600
  // =================================================

  I2C_Right.begin(
    RIGHT_SDA,
    RIGHT_SCL,
    400000);

  delay(100);

  // =================================================
  // VL53L0X
  // =================================================

  Serial.println("Starting VL53L0X...");

  if (!lox.begin()) {
    Serial.println("VL53L0X ERROR!");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("VL53L0X OK!");

  // Continuous measurements
  lox.startRangeContinuous(30);

  // =================================================
  // INITIAL ENCODER POSITIONS
  // =================================================

  leftPrevious =
    readAS5600(Wire);

  rightPrevious =
    readAS5600(I2C_Right);

  Serial.print("Left AS5600: ");
  Serial.println(leftPrevious);

  Serial.print("Right AS5600: ");
  Serial.println(rightPrevious);

  Serial.println();
  Serial.println("READY");
  Serial.println();
}

// =====================================================
// LOOP
// =====================================================

void loop() {
  // -------------------------------------------------
  // UPDATE ROBOT POSITION
  // -------------------------------------------------

  updateOdometry();

  // -------------------------------------------------
  // READ VL53L0X
  // -------------------------------------------------

  int distance =
    readDistance();

  // -------------------------------------------------
  // OUTPUT ROBOT POSE
  // -------------------------------------------------

  Serial.print("POSE,");
  Serial.print(robotX, 4);
  Serial.print(",");
  Serial.print(robotY, 4);
  Serial.print(",");
  Serial.println(heading, 5);

  // -------------------------------------------------
  // OUTPUT SCAN DATA
  // -------------------------------------------------

  if (distance > 0) {
    Serial.print("SCAN,");
    Serial.print(robotX, 4);
    Serial.print(",");
    Serial.print(robotY, 4);
    Serial.print(",");
    Serial.print(heading, 5);
    Serial.print(",");
    Serial.println(distance);
  }

  delay(30);
}