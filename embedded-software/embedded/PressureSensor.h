#include <Wire.h>
#include <MS5x.h>
MS5x barometer(&Wire);


class PressureSensor {
  private:
    bool barometerConnected = false;

    uint32_t prevConnectionAttempt = 0;
    uint32_t connectionAttemptDelay = 500; // Time in ms to wait between connection attempts to the sensor
    uint32_t prevTime = 0;; // The time, in MS the device was last polled

    double prevPressure = 0; // The value of the pressure the last time the sensor was polled
    double prevTemperature = 0; // The value of the temperature the last time the sensor was polled
    double seaLevelPressure = 0;

  public:
    void PressureSensorSetup() {
      barometer.setI2Caddr(I2C_HIGH);
      barometer.setSamples(MS5xxx_CMD_ADC_2048);
      barometer.setDelay(1000);
      barometer.setPressPa();
      barometer.setTOffset(-200); // Decreases temperature reading by 2.00°C
      barometer.setPOffset(5);
      if (barometer.connect() > 0) { // barometer.connect starts wire and attempts to connect to sensor
        //        Serial.println(F("Error connecting..."));
      } else {
        //        Serial.println(F("Connected to Sensor"));
        barometerConnected = true;
      }
    }
    int PressureSensorLoop(double *SensorValues) {
      //      Serial.print(F("\nIs Connected = "));
      //      Serial.print(barometerConnected);
      double pressure = 0;
      double temperature = 0;
      double altitude = 0;

      if (!barometerConnected) {
        if (millis() - prevConnectionAttempt >= connectionAttemptDelay) {
          // Retry connection attemp
          if (barometer.connect() > 0) {
            //            Serial.println(F("Error connecting..."));
            prevConnectionAttempt = millis();
          } else {
            //            Serial.println(F("Connected!"));
            barometerConnected = true;
          }
        }
      } else {
        /* In order to not have any delays used in code, checkUpdates cycles through sensor read process
          Step 1: Ask for raw temperature calculation to be performed
          Step 2: Once enough time has passed for calculation ask sensor to send results
          Step 3: Ask for raw pressure calculation to be performed
          Step 4: Once enough time has passed for calculation ask sensor to send results
          At this point checkUpdates returns true, but no new sensor readings will be performed until Readout function is called. */
        barometer.checkUpdates();
        if (barometer.isReady()) { // Barometer has performed inital calculations and data exists for  Temperature and Pressure
          temperature = barometer.GetTemp(); // Returns temperature in selected units
          pressure = barometer.GetPres(); // Returns pressure in selected units
          // New temperature and/or pressure values exist
          if ((temperature != prevTemperature) || (pressure != prevPressure)) {

            // If Sea Level Pressure has not been calculated, do so now.
            if (seaLevelPressure == 0) seaLevelPressure = barometer.getSeaLevel(217.3);

            //            Serial.print(F("The current pressure is: "));
            //            Serial.print(pressure);
            //            Serial.println(F(" Pascals"));
            //
            //            Serial.print(F("The current temperature is: "));
            //            Serial.print(temperature);
            //            Serial.println(F("°C"));
            //
            //            Serial.print(F("The calculated pressure at seaLevel is: "));
            //            Serial.print(seaLevelPressure);
            //            Serial.println(F(" Pascals"));

            // Calculate current altitude based on pressure reading
            altitude = barometer.getAltitude();
            //            Serial.print(F("The calculated altitude is: "));
            //            Serial.print(altitude);
            //            Serial.println(F(" meters"));

            // Calculate current altitude performing temperature corrections
            altitude = barometer.getAltitude(true);
            //            Serial.print(F("The calculated altitude with temperature correction is: "));
            //            Serial.print(altitude);
            //            Serial.println(F(" meters"));

            //            Serial.println();
            //            Serial.println();

            prevTemperature = temperature;
            prevPressure = pressure;
            SensorValues[0] = pressure;
            SensorValues[1] = temperature;
            SensorValues[2] = altitude;
            return 1;

          }
        }
      }

      return 0;
    }




};

PressureSensor PressureSensor;
