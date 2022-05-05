class MotorTest {
  private:
#define MY_dirPin 12
#define MY_stepPin 13
#define MX_dirPin 27
#define MX_stepPin 14
#define stepsPerRevolution 200*16

  public:
    void MotorTestSetup() {
      pinMode(MX_stepPin, OUTPUT);
      pinMode(MX_dirPin, OUTPUT);
      pinMode(MY_stepPin, OUTPUT);
      pinMode(MY_dirPin, OUTPUT);

    }
    void testMotors() {

      Serial.println("Motor Y ClockWise for half rev");
      digitalWrite(MY_dirPin, HIGH);
      for (int i = 0; i < stepsPerRevolution / 3; i++) {
        // These four lines result in 1 step:
        digitalWrite(MY_stepPin, HIGH);
        delayMicroseconds(4000);
        digitalWrite(MY_stepPin, LOW);
        delayMicroseconds(4000);
      }
      delay(2000); //2 Sec

      Serial.println("Motor Y Counter ClockWise for half rev");
      digitalWrite(MY_dirPin, LOW);
      for (int i = 0; i < stepsPerRevolution / 3; i++) {
        // These four lines result in 1 step:
        digitalWrite(MY_stepPin, HIGH);
        delayMicroseconds(4000);
        digitalWrite(MY_stepPin, LOW);
        delayMicroseconds(4000);
      }
      delay(2000);

      Serial.println("Motor X ClockWise");
      digitalWrite(MX_dirPin, HIGH); digitalWrite(MY_dirPin, HIGH);
      // Spin the stepper motor 1 revolution slowly:
      for (int i = 0; i < stepsPerRevolution / 3; i++) {
        // These four lines result in 1 step:
        digitalWrite(MX_stepPin, HIGH); digitalWrite(MY_stepPin, HIGH);
        delayMicroseconds(4000);
        digitalWrite(MX_stepPin, LOW);  digitalWrite(MY_stepPin, LOW);
        delayMicroseconds(4000);
      }
      delay(2000);

      Serial.println("Motor X Counter ClockWise");
      digitalWrite(MX_dirPin, LOW); digitalWrite(MY_dirPin, LOW);
      // Spin the stepper motor 1 revolution slowly:
      for (int i = 0; i < stepsPerRevolution / 3; i++) {
        // These four lines result in 1 step:
        digitalWrite(MX_stepPin, HIGH); digitalWrite(MY_stepPin, HIGH);
        delayMicroseconds(4000);
        digitalWrite(MX_stepPin, LOW);  digitalWrite(MY_stepPin, LOW);
        delayMicroseconds(4000);
      }

    }


};
MotorTest MotorTest;
