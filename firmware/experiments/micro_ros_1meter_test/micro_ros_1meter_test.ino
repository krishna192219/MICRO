#include <Wire.h>

// =====================================================
// MOTOR PINS
// =====================================================

#define AIN2 27
#define AIN1 14
#define PWMA 12

#define BIN1 33
#define BIN2 25
#define PWMB 32

#define STBY 26


// =====================================================
// ENCODERS
// LEFT  -> SDA 16, SCL 23
// RIGHT -> SDA 21, SCL 22
// =====================================================

TwoWire I2C_Left = TwoWire(1);

#define AS5600_ADDR 0x36
#define AS5600_RAW_ANGLE_H 0x0C


// =====================================================
// ROBOT PARAMETERS
// =====================================================

const float WHEEL_DIAMETER_M = 0.044;
const float WHEEL_CIRCUMFERENCE_M =
    PI * WHEEL_DIAMETER_M;

const float COUNTS_PER_REV = 4096.0;

const float TARGET_DISTANCE_M = 1.0;

const long TARGET_COUNTS =
    (long)((TARGET_DISTANCE_M /
            WHEEL_CIRCUMFERENCE_M) *
           COUNTS_PER_REV);


// =====================================================
// MOTOR SPEED
// =====================================================

const int LEFT_PWM  = 63;
const int RIGHT_PWM = 63;


// =====================================================
// ENCODER VARIABLES
// =====================================================

int leftPrevRaw;
int rightPrevRaw;

long leftCounts = 0;
long rightCounts = 0;

unsigned long startTime;


// =====================================================
// READ LEFT ENCODER
// =====================================================

int readLeftAngle()
{
  I2C_Left.beginTransmission(AS5600_ADDR);
  I2C_Left.write(AS5600_RAW_ANGLE_H);

  if (I2C_Left.endTransmission(false) != 0)
    return -1;

  I2C_Left.requestFrom(AS5600_ADDR, 2);

  if (I2C_Left.available() < 2)
    return -1;

  int highByte = I2C_Left.read();
  int lowByte = I2C_Left.read();

  return ((highByte << 8) | lowByte) & 0x0FFF;
}


// =====================================================
// READ RIGHT ENCODER
// =====================================================

int readRightAngle()
{
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(AS5600_RAW_ANGLE_H);

  if (Wire.endTransmission(false) != 0)
    return -1;

  Wire.requestFrom(AS5600_ADDR, 2);

  if (Wire.available() < 2)
    return -1;

  int highByte = Wire.read();
  int lowByte = Wire.read();

  return ((highByte << 8) | lowByte) & 0x0FFF;
}


// =====================================================
// LEFT MOTOR FORWARD
// =====================================================

void setLeftMotor(int pwm)
{
  pwm = constrain(pwm, 0, 255);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  ledcWrite(PWMA, pwm);
}


// =====================================================
// RIGHT MOTOR FORWARD
// =====================================================

void setRightMotor(int pwm)
{
  pwm = constrain(pwm, 0, 255);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(PWMB, pwm);
}


// =====================================================
// STOP MOTORS
// =====================================================

void stopMotors()
{
  ledcWrite(PWMA, 0);
  ledcWrite(PWMB, 0);

  // Also explicitly put motor direction pins LOW
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // Motor pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);


  // PWM
  ledcAttach(PWMA, 20000, 8);
  ledcAttach(PWMB, 20000, 8);


  // I2C
  Wire.begin(21, 22);
  I2C_Left.begin(16, 23);


  delay(1000);


  // Get initial encoder positions
  leftPrevRaw = readLeftAngle();
  rightPrevRaw = readRightAngle();


  Serial.println();
  Serial.println("================================");
  Serial.println("1 METER DISTANCE TEST");
  Serial.println("================================");

  Serial.print("Target counts: ");
  Serial.println(TARGET_COUNTS);

  Serial.println("Starting in 3 seconds...");

  delay(3000);


  // Re-read immediately before motion
  leftPrevRaw = readLeftAngle();
  rightPrevRaw = readRightAngle();

  leftCounts = 0;
  rightCounts = 0;

  startTime = millis();


  Serial.println("GO!");

  setLeftMotor(LEFT_PWM);
  setRightMotor(RIGHT_PWM);
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
  int leftRaw = readLeftAngle();
  int rightRaw = readRightAngle();

  if (leftRaw < 0 || rightRaw < 0)
  {
    Serial.println("ENCODER ERROR!");
    stopMotors();

    while (true)
    {
      delay(1000);
    }
  }


  // ---------------------------------------------------
  // LEFT DELTA
  // ---------------------------------------------------

  int leftDelta = leftRaw - leftPrevRaw;

  if (leftDelta > 2048)
    leftDelta -= 4096;

  else if (leftDelta < -2048)
    leftDelta += 4096;


  // ---------------------------------------------------
  // RIGHT DELTA
  // ---------------------------------------------------

  int rightDelta = rightRaw - rightPrevRaw;

  if (rightDelta > 2048)
    rightDelta -= 4096;

  else if (rightDelta < -2048)
    rightDelta += 4096;


  // ---------------------------------------------------
  // ACCUMULATE
  // ---------------------------------------------------

  leftCounts += leftDelta;
  rightCounts += rightDelta;


  leftPrevRaw = leftRaw;
  rightPrevRaw = rightRaw;


  // ---------------------------------------------------
  // DISTANCE
  // ---------------------------------------------------

  float leftDistance =
      ((float)leftCounts / COUNTS_PER_REV) *
      WHEEL_CIRCUMFERENCE_M;

  float rightDistance =
      ((float)(-rightCounts / COUNTS_PER_REV)) *
      WHEEL_CIRCUMFERENCE_M;


  float averageDistance =
      (leftDistance + rightDistance) / 2.0;


  // ---------------------------------------------------
  // PRINT
  // ---------------------------------------------------

  Serial.print("L: ");
  Serial.print(leftCounts);

  Serial.print(" | R: ");
  Serial.print(rightCounts);

  Serial.print(" | Ldist: ");
  Serial.print(leftDistance, 3);

  Serial.print(" | Rdist: ");
  Serial.print(rightDistance, 3);

  Serial.print(" | Avg: ");
  Serial.print(averageDistance, 3);

  Serial.println(" m");


  // ===================================================
  // STOP IF EITHER WHEEL REACHES 1 METER
  // ===================================================

  if (leftCounts >= TARGET_COUNTS ||
      rightCounts >= TARGET_COUNTS)
  {
    stopMotors();

    Serial.println();
    Serial.println("================================");
    Serial.println("TARGET REACHED - MOTORS STOPPED");
    Serial.println("================================");

    Serial.print("Left counts: ");
    Serial.println(leftCounts);

    Serial.print("Right counts: ");
    Serial.println(rightCounts);

    Serial.print("Left distance: ");
    Serial.print(leftDistance, 4);
    Serial.println(" m");

    Serial.print("Right distance: ");
    Serial.print(rightDistance, 4);
    Serial.println(" m");

    Serial.print("Average distance: ");
    Serial.print(averageDistance, 4);
    Serial.println(" m");

    while (true)
    {
      delay(1000);
    }
  }


  // ===================================================
  // HARD SAFETY TIMEOUT
  // ===================================================

  if (millis() - startTime > 10000)
  {
    stopMotors();

    Serial.println();
    Serial.println("!!! SAFETY TIMEOUT !!!");
    Serial.println("Motors stopped.");

    while (true)
    {
      delay(1000);
    }
  }


  delay(10);
}