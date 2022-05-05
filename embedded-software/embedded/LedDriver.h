class LedDriver {
  private:
    //-- LED Dimming Setup --//
    const int ledPin = 15;
    // setting PWM properties
    const int freq = 5000;
    const int ledChannel = 0;
    const int resolution = 8;

  public:
    void LedDriverSetup() {
      // configure LED PWM functionalitites
      ledcSetup(ledChannel, freq, resolution);

      // attach the channel to the GPIO to be controlled
      ledcAttachPin(ledPin, ledChannel);

    }
    int LedDriverLoop(double lux, int manual, int brightness) {
      Serial.println("Driving the LED");
      if (lux > 500) {
        brightness = 0;
        ledcWrite(ledChannel, brightness);
      }
      else if (lux > 400) {
        brightness = 255 * 0.2;
        ledcWrite(ledChannel, 255);
      }
      else if (lux > 300) {
        brightness = 255 * 0.3;
        ledcWrite(ledChannel, 255);
      }
      else if (lux > 200) {
        brightness = 255 * 0.5;
        ledcWrite(ledChannel, 255);
      }
      else if (lux > 100) {
        brightness = 255 * 0.6;
        ledcWrite(ledChannel, 255);
      }
      else if (lux > 50) {
        brightness = 255 * 0.8;
        ledcWrite(ledChannel, 255);
      }
      else if (lux > 20) {
        brightness = 255 * 0.9;
        ledcWrite(ledChannel, 255);
      }
      else {
        brightness = 255 * 1;
        ledcWrite(ledChannel, 255);
      }


      return 1;

    }


};
LedDriver LedDriver;
