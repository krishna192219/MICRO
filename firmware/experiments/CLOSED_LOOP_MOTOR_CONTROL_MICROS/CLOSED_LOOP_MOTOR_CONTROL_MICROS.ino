    #include <Wire.h>

    // =====================================================
    // TB6612FNG
    // =====================================================

    // Left motor
    #define AIN1 27
    #define AIN2 14
    #define PWMA 12

    // Right motor
    #define BIN1 33
    #define BIN2 25
    #define PWMB 32

    #define STBY 26

    #define PWM_FREQ 20000
    #define PWM_RESOLUTION 8

    // =====================================================
    // FEED-FORWARD PWM
    // =====================================================
    //
    // Your measured values for straight motion:
    // Left  = 115
    // Right = 150
    //

    float LEFT_BASE_PWM  = 90.0;
    float RIGHT_BASE_PWM = 125.0;


    float leftRPMRaw = 0.0;
    float rightRPMRaw = 0.0;

    const float RPM_FILTER_ALPHA = 0.25;
    // =====================================================
    // AS5600
    // =====================================================

    #define AS5600_ADDR 0x36
    #define RAW_ANGLE_HIGH 0x0C

    // Left encoder
    #define LEFT_SDA 21
    #define LEFT_SCL 22

    // Right encoder
    #define RIGHT_SDA 16
    #define RIGHT_SCL 23

    TwoWire I2C_Right = TwoWire(1);

    // =====================================================
    // ROBOT
    // =====================================================

    const float WHEEL_DIAMETER = 0.044;

    const float WHEEL_CIRCUMFERENCE =
        PI * WHEEL_DIAMETER;

    // =====================================================
    // ENCODERS
    // =====================================================

    uint16_t leftPrevious = 0;
    uint16_t rightPrevious = 0;

    long leftCounts = 0;
    long rightCounts = 0;

    // =====================================================
    // SPEED
    // =====================================================

    float leftRPM = 0.0;
    float rightRPM = 0.0;

    // =====================================================
    // TARGET
    // =====================================================

    float targetRPM = 100.0;

    // =====================================================
    // PID
    // =====================================================

    // Start with small values.
    // We will tune these later.

    float Kp = 1;
    float Ki = 0.2;
    float Kd = 0.0;

    // Integral terms
    float leftIntegral = 0.0;
    float rightIntegral = 0.0;

    // Previous errors
    float leftPreviousError = 0.0;
    float rightPreviousError = 0.0;

    // =====================================================
    // PWM LIMITS
    // =====================================================

    const float MIN_PWM = 0.0;
    const float MAX_PWM = 255.0;

    // =====================================================
    // READ AS5600
    // =====================================================

    uint16_t readAS5600(TwoWire &bus)
    {
        bus.beginTransmission(AS5600_ADDR);

        bus.write(RAW_ANGLE_HIGH);

        if (bus.endTransmission(false) != 0)
        {
            return 0;
        }

        bus.requestFrom(AS5600_ADDR, 2);

        if (bus.available() < 2)
        {
            return 0;
        }

        uint8_t highByte = bus.read();
        uint8_t lowByte  = bus.read();

        uint16_t angle =
            ((uint16_t)highByte << 8) | lowByte;

        return angle & 0x0FFF;
    }

    // =====================================================
    // HANDLE AS5600 WRAPAROUND
    // =====================================================

    int16_t angleDifference(
        uint16_t current,
        uint16_t previous)
    {
        int16_t difference =
            (int16_t)current -
            (int16_t)previous;

        if (difference > 2048)
        {
            difference -= 4096;
        }

        if (difference < -2048)
        {
            difference += 4096;
        }

        return difference;
    }

    // =====================================================
    // LEFT MOTOR
    // =====================================================

    void leftMotor(int pwm)
    {
        pwm = constrain(pwm, 0, 255);

        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);

        ledcWrite(PWMA, pwm);
    }

    // =====================================================
    // RIGHT MOTOR
    // =====================================================

    void rightMotor(int pwm)
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
    // CALCULATE RPM
    // =====================================================

    void updateRPM(float dt)
    {
        static long leftAccumulatedCounts = 0;
        static long rightAccumulatedCounts = 0;

        static float measurementTime = 0.0;

        uint16_t leftCurrent =
            readAS5600(I2C_Right);

        uint16_t rightCurrent =
            readAS5600(Wire);

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

        // ---------------------------------------------
        // Accumulate encoder movement
        // ---------------------------------------------

        leftAccumulatedCounts += leftDelta;
        rightAccumulatedCounts += rightDelta;

        leftCounts += leftDelta;
        rightCounts += rightDelta;

        measurementTime += dt;

        // ---------------------------------------------
        // Calculate RPM every 200 ms
        // ---------------------------------------------

        if (measurementTime >= 0.20)
        {
            leftRPMRaw =
                -((float)leftAccumulatedCounts / 4096.0)
                / measurementTime
                * 60.0;

            rightRPMRaw =
                -((float)rightAccumulatedCounts / 4096.0)
                / measurementTime
                * 60.0;

            // -----------------------------------------
            // Low-pass filter
            // -----------------------------------------

            leftRPM =
                RPM_FILTER_ALPHA * leftRPMRaw
                + (1.0 - RPM_FILTER_ALPHA) * leftRPM;

            rightRPM =
                RPM_FILTER_ALPHA * rightRPMRaw
                + (1.0 - RPM_FILTER_ALPHA) * rightRPM;

            // -----------------------------------------
            // Reset measurement window
            // -----------------------------------------

            leftAccumulatedCounts = 0;
            rightAccumulatedCounts = 0;

            measurementTime = 0.0;
        }
    }

    // =====================================================
    // LEFT PID
    // =====================================================

    float calculateLeftPID(float dt)
    {
        float error =
            targetRPM - leftRPM;

        // Integral
        leftIntegral += error * dt;

        // Prevent integral windup
        leftIntegral =
            constrain(leftIntegral, -100.0, 100.0);

        // Derivative
        float derivative =
            (error - leftPreviousError) / dt;

        leftPreviousError = error;

        float correction =
            Kp * error
            + Ki * leftIntegral
            + Kd * derivative;

        return correction;
    }

    // =====================================================
    // RIGHT PID
    // =====================================================

    float calculateRightPID(float dt)
    {
        float error =
            targetRPM - rightRPM;

        // Integral
        rightIntegral += error * dt;

        // Prevent integral windup
        rightIntegral =
            constrain(rightIntegral, -100.0, 100.0);

        // Derivative
        float derivative =
            (error - rightPreviousError) / dt;

        rightPreviousError = error;

        float correction =
            Kp * error
            + Ki * rightIntegral
            + Kd * derivative;

        return correction;
    }

    // =====================================================
    // SETUP
    // =====================================================

    void setup()
    {
        Serial.begin(115200);

        delay(1000);

        // -------------------------------------------------
        // MOTOR PINS
        // -------------------------------------------------

        pinMode(AIN1, OUTPUT);
        pinMode(AIN2, OUTPUT);

        pinMode(BIN1, OUTPUT);
        pinMode(BIN2, OUTPUT);

        pinMode(STBY, OUTPUT);

        digitalWrite(STBY, HIGH);

        // -------------------------------------------------
        // PWM
        // -------------------------------------------------

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

        // -------------------------------------------------
        // I2C
        // -------------------------------------------------

        Wire.begin(
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

        // -------------------------------------------------
        // INITIAL ENCODER POSITION
        // -------------------------------------------------

        leftPrevious =
            readAS5600(I2C_Right);

        rightPrevious =
            readAS5600(Wire);

        Serial.println();
        Serial.println("======================================");
        Serial.println(" CLOSED LOOP MOTOR CONTROL");
        Serial.println("======================================");

        Serial.print("Target RPM: ");
        Serial.println(targetRPM);

        Serial.print("Left base PWM: ");
        Serial.println(LEFT_BASE_PWM);

        Serial.print("Right base PWM: ");
        Serial.println(RIGHT_BASE_PWM);

        Serial.println();

        Serial.println("Starting in 3 seconds...");

        delay(3000);

        // Reset
        leftCounts = 0;
        rightCounts = 0;

        leftIntegral = 0;
        rightIntegral = 0;

        leftPreviousError = 0;
        rightPreviousError = 0;

        leftPrevious =
            readAS5600(I2C_Right);

        rightPrevious =
            readAS5600(Wire);

        Serial.println("MOVING");

        // Initial feed-forward PWM
        leftMotor((int)LEFT_BASE_PWM);
        rightMotor((int)RIGHT_BASE_PWM);
    }

    // =====================================================
    // LOOP
    // =====================================================

    void loop()
    {
        static unsigned long lastControlTime = 0;

        unsigned long now = millis();

        // =================================================
        // CONTROL LOOP
        // =================================================

        if (now - lastControlTime >= 50)
        {
            float dt =
                (now - lastControlTime) / 1000.0;

            lastControlTime = now;

            // ---------------------------------------------
            // READ ENCODERS + CALCULATE RPM
            // ---------------------------------------------

            updateRPM(dt);

            // ---------------------------------------------
            // PID
            // ---------------------------------------------

            float leftCorrection =
                calculateLeftPID(dt);

            float rightCorrection =
                calculateRightPID(dt);

            // ---------------------------------------------
            // FEED-FORWARD + PID
            // ---------------------------------------------

            float leftPWM =
                LEFT_BASE_PWM + leftCorrection;

            float rightPWM =
                RIGHT_BASE_PWM + rightCorrection;

            // ---------------------------------------------
            // LIMIT PWM
            // ---------------------------------------------

            leftPWM =
                constrain(
                    leftPWM,
                    MIN_PWM,
                    MAX_PWM
                );

            rightPWM =
                constrain(
                    rightPWM,
                    MIN_PWM,
                    MAX_PWM
                );

            // ---------------------------------------------
            // MOTOR OUTPUT
            // ---------------------------------------------

            leftMotor((int)leftPWM);
            rightMotor((int)rightPWM);

            // ---------------------------------------------
            // SERIAL DEBUG
            // ---------------------------------------------

            Serial.print("Target: ");
            Serial.print(targetRPM, 1);

            Serial.print(" | Left RPM: ");
            Serial.print(leftRPM, 1);

            Serial.print(" | Left PWM: ");
            Serial.print(leftPWM, 1);

            Serial.print(" | Right RPM: ");
            Serial.print(rightRPM, 1);

            Serial.print(" | Right PWM: ");
            Serial.println(rightPWM, 1);
            float leftError = targetRPM - leftRPM;
            float rightError = targetRPM - rightRPM;

            Serial.print(" | LE: ");
            Serial.print(leftError, 2);

            Serial.print(" | LC: ");
            Serial.print(leftCorrection, 2);

            Serial.print(" | RE: ");
            Serial.print(rightError, 2);

            Serial.print(" | RC: ");
            Serial.print(rightCorrection, 2);
        }
    }