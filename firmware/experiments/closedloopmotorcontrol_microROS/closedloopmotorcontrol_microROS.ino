#include <Wire.h>

// =====================================================
// MOTOR DRIVER - TB6612FNG
// =====================================================
#define BIN2 25
#define BIN1 33
#define PWMB 32

// Motor B
#define AIN1 27
#define AIN2 14
#define PWMA 12

#define STBY 26

#define PWM_FREQ 20000
#define PWM_RESOLUTION 8


// =====================================================
// AS5600
// =====================================================

#define AS5600_ADDR 0x36

#define RAW_ANGLE_HIGH 0x0C

// LEFT ENCODER
#define LEFT_SDA 21
#define LEFT_SCL 22

// RIGHT ENCODER
#define RIGHT_SDA 16
#define RIGHT_SCL 23

// TwoWire I2C_Left = TwoWire(0);
TwoWire I2C_Right = TwoWire(1);


// =====================================================
// ROBOT SETTINGS
// =====================================================

const float WHEEL_DIAMETER = 0.044;
const float WHEEL_CIRCUMFERENCE =
    PI * WHEEL_DIAMETER;


// =====================================================
// MOTOR CONTROL
// =====================================================

// Desired base PWM
const int TARGET_PWM = 50;

// Initial PWM for each motor
int leftPWM = TARGET_PWM;
int rightPWM = TARGET_PWM;


// =====================================================
// PID SETTINGS
// =====================================================

// Start with ONLY proportional control
float Kp = 1.3;

// Keep these zero initially
float Ki = 0.01;
float Kd = 0;

float errorIntegral = 0;
float previousError = 0;


// =====================================================
// ENCODER VARIABLES
// =====================================================

uint16_t leftPrevious;
uint16_t rightPrevious;

unsigned long lastControlTime = 0;

const unsigned long CONTROL_PERIOD = 100;


// =====================================================
// READ AS5600
// =====================================================

uint16_t readAS5600(TwoWire &bus)
{
    bus.beginTransmission(AS5600_ADDR);

    bus.write(RAW_ANGLE_HIGH);

    if (bus.endTransmission(false) != 0)
        return 0;

    bus.requestFrom(AS5600_ADDR, 2);

    if (bus.available() < 2)
        return 0;

    uint8_t highByte = bus.read();
    uint8_t lowByte = bus.read();

    uint16_t angle =
        ((uint16_t)highByte << 8) | lowByte;

    return angle & 0x0FFF;
}


// =====================================================
// ANGLE DIFFERENCE
// =====================================================

int16_t angleDifference(
    uint16_t current,
    uint16_t previous)
{
    int16_t difference =
        (int16_t)current -
        (int16_t)previous;

    if (difference > 2048)
        difference -= 4096;

    if (difference < -2048)
        difference += 4096;

    return difference;
}


// =====================================================
// MOTOR A
// =====================================================

void motorA(int pwm)
{
    pwm = constrain(pwm, 0, 255);

    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    ledcWrite(PWMA, pwm);
}


// =====================================================
// MOTOR B
// =====================================================

void motorB(int pwm)
{
    pwm = constrain(pwm, 0, 255);

    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);

    ledcWrite(PWMB, pwm);
}


// =====================================================
// STOP
// =====================================================

void stopMotors()
{
    ledcWrite(PWMA, 0);
    ledcWrite(PWMB, 0);

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

    delay(1000);

    // -----------------------------
    // Motor pins
    // -----------------------------

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    pinMode(STBY, OUTPUT);

    digitalWrite(STBY, HIGH);

    // -----------------------------
    // PWM
    // -----------------------------

    ledcAttach(
        PWMA,
        PWM_FREQ,
        PWM_RESOLUTION
    );

    ledcAttach(
        PWMB,
        PWM_FREQ,
        PWM_RESOLUTION
    );

    stopMotors();

    // -----------------------------
    // I2C
    // -----------------------------

    I2C_Left.begin(
        LEFT_SDA,
        LEFT_SCL,
        400000
    );

    I2C_Right.begin(
        RIGHT_SDA,
        RIGHT_SCL,
        400000
    );

    delay(100);

    // -----------------------------
    // Initial encoder positions
    // -----------------------------

    leftPrevious =
        readAS5600(I2C_Left);

    rightPrevious =
        readAS5600(I2C_Right);

    lastControlTime = millis();

    Serial.println();
    Serial.println("==============================");
    Serial.println(" CLOSED LOOP MOTOR CONTROL");
    Serial.println("==============================");

    Serial.print("Target PWM: ");
    Serial.println(TARGET_PWM);

    Serial.println("Starting...");
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
    unsigned long now = millis();

    if (now - lastControlTime < CONTROL_PERIOD)
        return;

    float dt =
        (now - lastControlTime) / 1000.0;

    lastControlTime = now;


    // =================================================
    // READ ENCODERS
    // =================================================

    uint16_t leftCurrent =
        readAS5600(I2C_Left);

    uint16_t rightCurrent =
        readAS5600(I2C_Right);


    // =================================================
    // ANGLE CHANGE
    // =================================================

    int16_t leftDelta =
        angleDifference(
            leftCurrent,
            leftPrevious
        );

    int16_t rightDelta =
        angleDifference(
            rightCurrent,
            rightPrevious
        );

    leftPrevious = leftCurrent;
    rightPrevious = rightCurrent;


    // =================================================
    // CALCULATE WHEEL SPEED
    // =================================================

    // AS5600 = 4096 counts/revolution

    float leftRPM =
        (leftDelta / 4096.0)
        / dt
        * 60.0;

    float rightRPM =
        (rightDelta / 4096.0)
        / dt
        * 60.0;


    // =================================================
    // SPEED ERROR
    // =================================================

    float error =
        leftRPM - rightRPM;


    // =================================================
    // PID
    // =================================================

    errorIntegral +=
        error * dt;

    errorIntegral =
        constrain(
            errorIntegral,
            -100,
            100
        );

    float derivative =
        (error - previousError) / dt;

    previousError = error;


    float correction =
        Kp * error
        + Ki * errorIntegral
        + Kd * derivative;


    // =================================================
    // APPLY CORRECTION
    // =================================================

    leftPWM =
        TARGET_PWM - correction;

    rightPWM =
        TARGET_PWM + correction;


    leftPWM =
        constrain(leftPWM, 0, 255);

    rightPWM =
        constrain(rightPWM, 0, 255);


    motorA(leftPWM);
    motorB(rightPWM);


    // =================================================
    // DEBUG
    // =================================================

    Serial.print("L RPM: ");
    Serial.print(leftRPM, 1);

    Serial.print(" | R RPM: ");
    Serial.print(rightRPM, 1);

    Serial.print(" | Error: ");
    Serial.print(error, 1);

    Serial.print(" | L PWM: ");
    Serial.print(leftPWM);

    Serial.print(" | R PWM: ");
    Serial.println(rightPWM);
}