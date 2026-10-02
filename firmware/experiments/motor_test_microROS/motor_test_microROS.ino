// ================= TB6612FNG =================

// Motor A
#define BIN2 25
#define BIN1 33
#define PWMB 32

// Motor B
#define AIN1 27
#define AIN2 14
#define PWMA 12

// Standby
#define STBY 26

#define PWM_FREQ 20000
#define PWM_RESOLUTION 8

int motorPWM = 150;

// =================================================

void setup()
{
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);

  // ESP32 Core 3.x PWM
  ledcAttach(PWMA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(PWMB, PWM_FREQ, PWM_RESOLUTION);

  // Forward direction
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  setSpeed(motorPWM);

  Serial.println();
  Serial.println("===== TWO MOTOR TEST =====");
  Serial.println("Enter PWM from 0 to 255");
  Serial.println();
}

// =================================================

void setSpeed(int pwm)
{
  pwm = constrain(pwm, 0, 255);

  ledcWrite(PWMA, pwm);
  ledcWrite(PWMB, pwm);

  motorPWM = pwm;

  Serial.print("Both motors PWM = ");
  Serial.println(pwm);
}

// =================================================

void loop()
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0)
    {
      int pwm = input.toInt();

      if (pwm >= 0 && pwm <= 255)
      {
        setSpeed(pwm);
      }
      else
      {
        Serial.println("Enter a value from 0 to 255");
      }
    }
  }
}