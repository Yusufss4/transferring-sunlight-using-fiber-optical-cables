class LuxSensor {
  private:
    SFE_TSL2561 light;
    boolean gain; // Gain setting, 0 = X1, 1 = X16;
    unsigned int ms;

  public:
    void LuxSensorSetup() {
      light.begin();
      unsigned char ID;
      if (light.getID(ID))
      {
        //        Serial.print("Got factory ID: 0X");
        //        Serial.print(ID, HEX);
        //        Serial.println(", should be 0X5X");
      }
      // Most library commands will return true if communications was successful,
      // and false if there was a problem. You can ignore this returned value,
      // or check whether a command worked correctly and retrieve an error code:
      else
      {
        byte error = light.getError();
        printError(error);
      }
      gain = 0;

      // If time = 0, integration will be 13.7ms
      // If time = 1, integration will be 101ms
      // If time = 2, integration will be 402ms
      // If time = 3, use manual start / stop to perform your own integration

      unsigned char time = 2;

      // setTiming() will set the third parameter (ms) to the
      // requested integration time in ms (this will be useful later):

      //      Serial.println("Set timing...");
      light.setTiming(gain, time, ms);

      // To start taking measurements, power up the sensor:

      //      Serial.println("Powerup...");
      light.setPowerUp();




    }
    double LuxSensorLoop() {
      delay(ms);
      unsigned int data0, data1;
      double lux;
      if (light.getData(data0, data1))
      {
        // getData() returned true, communication was successful

        //        Serial.print("data0: ");
        //        Serial.print(data0);
        //        Serial.print(" data1: ");
        //        Serial.print(data1);

        // To calculate lux, pass all your settings and readings
        // to the getLux() function.

        // Resulting lux value
        boolean good; // True if neither sensor is saturated

        // Perform lux calculation:

        good = light.getLux(gain, ms, data0, data1, lux);

        // Print out the results:

        //        Serial.print(" lux: ");
        //        Serial.print(lux);
        //        if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
      }
      else
      {
        // getData() returned false because of an I2C error, inform the user.

        byte error = light.getError();
        printError(error);
        return -1;
      }
      return lux;
    }

    void printError(byte error)
    // If there's an I2C error, this function will
    // print out an explanation.
    {
      Serial.print("I2C error: ");
      Serial.print(error, DEC);
      Serial.print(", ");

      switch (error)
      {
        case 0:
          Serial.println("success");
          break;
        case 1:
          Serial.println("data too long for transmit buffer");
          break;
        case 2:
          Serial.println("received NACK on address (disconnected?)");
          break;
        case 3:
          Serial.println("received NACK on data");
          break;
        case 4:
          Serial.println("other error");
          break;
        default:
          Serial.println("unknown error");
      }
    }



};

LuxSensor LuxSensor;
