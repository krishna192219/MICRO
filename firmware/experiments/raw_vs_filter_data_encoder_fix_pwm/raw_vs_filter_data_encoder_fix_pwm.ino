#include <Wire.h>

// ================= MOTOR PINS =================

// Left motor
#define AIN1 14
#define AIN2 27
#define PWMA 12

// Right motor
#define BIN1 33
#define BIN2 25
#define PWMB 32

#define STBY 26

// ================= ENCODERS =================

// LEFT encoder -> I2C bus 1
TwoWire I2C_Left = TwoWire(1);

// RIGHT encoder -> default Wire
#define LEFT_SDA  16
#define LEFT_SCL  23

#define RIGHT_SDA 21
#define RIGHT_SCL 22

#define AS5600_ADDR 0x36

// ================= SETTINGS =================

const int TEST_PWM = 63;          // Fixed motor PWM
const int SAMPLE_TIME_MS = 1000;   // RPM calculation interval

const float COUNTS_PER_REV = 4096.0;

// ================= ENCODER VARIABLES =================

int leftRaw = 0;
int rightRaw = 0;

int lastLeftRaw = 0;
int lastRightRaw = 0;

long leftCount = 0;
long rightCount = 0;

unsigned long lastTime = 0;


// ======================================================
// READ AS5600 RAW ANGLE
// ======================================================

int readAngle(TwoWire &bus)
{
  bus.beginTransmission(AS5600_ADDR);
  bus.write(0x0C);

  if (bus.endTransmission(false) != 0)
    return -1;

  if (bus.requestFrom(AS5600_ADDR, 2) != 2)
    return -1;

  int highByte = bus.read();
  int lowByte = bus.read();

  return ((highByte << 8) | lowByte) & 0x0FFF;
}


// ======================================================
// UPDATE ENCODER
// ======================================================

void updateLeftEncoder()
{
  int raw = readAngle(I2C_Left);

  if (raw < 0)
    return;

  leftRaw = raw;

  int delta = raw - lastLeftRaw;

  // Handle 12-bit wraparound
  if (delta > 2048)
    delta -= 4096;

  if (delta < -2048)
    delta += 4096;

  leftCount += delta;

  lastLeftRaw = raw;
}


void updateRightEncoder()
{
  int raw = readAngle(Wire);

  if (raw < 0)
    return;

  rightRaw = raw;

  int delta = raw - lastRightRaw;

  // Handle 12-bit wraparound
  if (delta > 2048)
    delta -= 4096;

  if (delta < -2048)
    delta += 4096;

  rightCount += delta;

  lastRightRaw = raw;
}


// ======================================================
// MOTOR CONTROL
// ======================================================

void leftForward(int pwm)
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(PWMA, pwm);
}


void rightForward(int pwm)
{
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(PWMB, pwm);
}


// ======================================================
// SETUP
// ======================================================

void setup()
{
  Serial.begin(115200);

  // Motor pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);

  // PWM
  ledcAttach(PWMA, 20000, 8);
  ledcAttach(PWMB, 20000, 8);

  // Encoder I2C
  I2C_Left.begin(LEFT_SDA, LEFT_SCL, 400000);
  Wire.begin(RIGHT_SDA, RIGHT_SCL, 400000);

  delay(500);

  // Get initial encoder positions
  lastLeftRaw = readAngle(I2C_Left);
  lastRightRaw = readAngle(Wire);

  Serial.println();
  Serial.println("=================================");
  Serial.println(" FIXED PWM ENCODER TEST");
  Serial.println("=================================");
  Serial.print("PWM = ");
  Serial.println(TEST_PWM);
  Serial.println();

  Serial.println("Time,LeftRaw,RightRaw,LeftCount,RightCount,LeftRPM,RightRPM");

  lastTime = millis();
}


// ======================================================
// LOOP
// ======================================================

void loop()
{
  // Run both motors at fixed PWM
  leftForward(TEST_PWM);
  rightForward(TEST_PWM);

  // Update encoders continuously
  updateLeftEncoder();
  updateRightEncoder();

  unsigned long now = millis();

  if (now - lastTime >= SAMPLE_TIME_MS)
  {
    float dt = (now - lastTime) / 1000.0;

    static long previousLeftCount = 0;
    static long previousRightCount = 0;

    long leftDelta = leftCount - previousLeftCount;
    long rightDelta = rightCount - previousRightCount;

    previousLeftCount = leftCount;
    previousRightCount = rightCount;

    float leftRPM =
      (leftDelta / COUNTS_PER_REV) / dt * 60.0;

    float rightRPM =
      (rightDelta / COUNTS_PER_REV) / dt * 60.0;

    // Make both positive because your encoder directions
    // are opposite for forward motion.
    leftRPM = abs(leftRPM);
    rightRPM = abs(rightRPM);

    Serial.print(now);
    Serial.print(",");
    Serial.print(leftRaw);
    Serial.print(",");
    Serial.print(rightRaw);
    Serial.print(",");
    Serial.print(leftCount);
    Serial.print(",");
    Serial.print(rightCount);
    Serial.print(",");
    Serial.print(leftRPM, 2);
    Serial.print(",");
    Serial.println(rightRPM, 2);

    lastTime = now;
  }
}