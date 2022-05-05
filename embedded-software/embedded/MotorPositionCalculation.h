class MotorPositionCalculation {
  private:
    int ldr_values[5];
    int refLDR = 0;
    int UL = 0;
    int UR = 0;
    int DL = 0;
    int DR = 0;

#define MY_dirPin 12
#define MY_stepPin 13
#define MX_dirPin 27
#define MX_stepPin 14
#define stepsPerRevolution 200*16
#define thresholdError 0.12

  public:
    void MotorPositionCalculationSetup() {
      pinMode(MX_stepPin, OUTPUT);
      pinMode(MX_dirPin, OUTPUT);
      pinMode(MY_stepPin, OUTPUT);
      pinMode(MY_dirPin, OUTPUT);

    }
    int findMotorPosition(int *dest) {

      refLDR = dest[0];
      UL = dest[1];
      UR = dest[2];
      DL = dest[3];
      DR = dest[4];
      refLDR = (refLDR + UL + UR + DL + DR) / 5;
      float thresholdLDR = thresholdError * refLDR;

      float left = (UL + DL) / 2;
      float rigth = (UR + DR) / 2;
      float up = (UL + UR) / 2;
      float down = (DL + DR) / 2;

      int LeftRightABS = abs(left - rigth);
      int UpDownABS = abs(up - down);


      Serial.print("Reference LDR -> ");
      Serial.println(refLDR);
      Serial.print("Left -> ");
      Serial.println(left);
      Serial.print("Right -> ");
      Serial.println(rigth);
      Serial.print("Up -> ");
      Serial.println(up);
      Serial.print("Down -> ");
      Serial.println(down);


      if (left > rigth && LeftRightABS >= thresholdLDR) {
        Serial.println("Motor X ClockWise");

        digitalWrite(MX_dirPin, HIGH); digitalWrite(MY_dirPin, HIGH);
        // Spin the stepper motor 1 revolution slowly:
        for (int i = 0; i < stepsPerRevolution / 1024; i++) {
          // These four lines result in 1 step:
          digitalWrite(MX_stepPin, HIGH); digitalWrite(MY_stepPin, HIGH);
          delayMicroseconds(3000);
          digitalWrite(MX_stepPin, LOW);  digitalWrite(MY_stepPin, LOW);
          delayMicroseconds(3000);
        }

      }
      else if (left <= rigth && LeftRightABS >= thresholdLDR) {
        Serial.println("Motor X Counter ClockWise");

        digitalWrite(MX_dirPin, LOW); digitalWrite(MY_dirPin, LOW);
        // Spin the stepper motor 1 revolution slowly:
        for (int i = 0; i < stepsPerRevolution / 1024; i++) {
          // These four lines result in 1 step:
          digitalWrite(MX_stepPin, HIGH); digitalWrite(MY_stepPin, HIGH);
          delayMicroseconds(3000);
          digitalWrite(MX_stepPin, LOW);  digitalWrite(MY_stepPin, LOW);
          delayMicroseconds(3000);
        }


      }
      else if (up > down && UpDownABS >= thresholdLDR ) {
        Serial.println("Motor Y ClockWise");

        digitalWrite(MY_dirPin, HIGH);
        // Spin the stepper motor 1 revolution slowly:
        for (int i = 0; i < stepsPerRevolution / 1024; i++) {
          // These four lines result in 1 step:
          digitalWrite(MY_stepPin, HIGH);
          delayMicroseconds(3000);
          digitalWrite(MY_stepPin, LOW);
          delayMicroseconds(3000);
        }

      }
      else if (up <= down && UpDownABS >= thresholdLDR) {
        Serial.println("Motor Y Counter ClockWise");

        digitalWrite(MY_dirPin, LOW);
        // Spin the stepper motor 1 revolution slowly:
        for (int i = 0; i < stepsPerRevolution / 1024; i++) {
          // These four lines result in 1 step:
          digitalWrite(MY_stepPin, HIGH);
          delayMicroseconds(3000);
          digitalWrite(MY_stepPin, LOW);
          delayMicroseconds(3000);
        }
      }
      else {
        Serial.println("Stop the Motor!");
        return 1; //Exit from the algorithm.
      }

      if (UpDownABS <= thresholdLDR && LeftRightABS <= thresholdLDR)  {
        Serial.println("Stop the Motor!");
        Serial.println("Stop the Motor!");
        return 1;
      }

      return 0;
    }


};
MotorPositionCalculation MotorPositionCalculation;
