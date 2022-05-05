#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SparkFunTSL2561.h>
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
#include <Ewma.h>
#include "DHT.h"

// Less smoothing - faster to detect changes, but more prone to noise
Ewma adcFilter1(0.2);
Ewma adcFilter2(0.2);
Ewma adcFilter3(0.2);
Ewma adcFilter4(0.2);
Ewma adcFilter5(0.2);

#include "LuxSensor.h"
#include "MotorPositionCalculation.h"
#include "MotorTest.h"
#include "PressureSensor.h"
#include "LedDriver.h"
#include "SDCard.h"


#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
Weather TempSensor;

#include "soc/sens_reg.h" // needed for manipulating ADC2 control register
uint64_t reg_b; // Used to store ADC2 control register
#define ldrPin4 25 // Substitute xx with your ADC2 Pin number



const char* ssid = "SUPERONLINE_WiFi_5918";
const char* password = "V9dp6ACGgxjE";
const char* serverName = "https://192.168.1.6:45455/api/students";
const char* serverName2 = "https://192.168.1.6:45455/api/students/1";
const int LDR_Size = 5;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000; //Send HTTP Request every 60 seconds.

const int referenceLdr = 34;  //ADC_1 Pin
const int ldrPin1 = 35; //ADC_1 Pin
const int ldrPin2 = 32; //ADC_1 Pin 32 -> 36
const int ldrPin3 = 33; //ADC_1 Pin
//const int ldrPin4 = 25; //ADC_2 Pin ??
const int ldrPinBackup = 39; //VN
//const int ldrPinBackup2 = 36; //VP


String sensorReadings;
String sensorReadings2;
float sensorReadingsArr[3];
int myInt = 0;
int i = 1;

void readLDRData(int *dest);
String httpGETRequest(const char* serverName);
String httpPostRequest(const String RequestData);

//PressureSensors
double PressureSensor_Values[3] = {0, 0, 0};

void setup() {
  Serial.begin(115200);

  //We need this to use ADC while wifi connection.
  reg_b = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);

  //Wifi Connection
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  PressureSensor.PressureSensorSetup();
  //double PressureSensor_Values[3] = {0, 0, 0};
  LuxSensor.LuxSensorSetup();
  MotorPositionCalculation.MotorPositionCalculationSetup();
  MotorTest.MotorTestSetup();
  LedDriver.LedDriverSetup();
  SDCardClass.SDCardSetup();
  dht.begin();
  TempSensor.begin();


}

void loop() {

  //----  Motor Test ----//
  //Test the Motors X and Y axis
  //MotorTest.testMotors();
  //---------------------------------//

  //----  Sun Tracking Algorithm ----//
   //There will be some kind of timer, enter is limited.
    int isMotorStop = 0;
    while (isMotorStop == 0) {
      int ldr_values[LDR_Size];
      readLDRData(ldr_values);
      isMotorStop = MotorPositionCalculation.findMotorPosition(ldr_values);
    }
  //---------------------------------//


  //----  Temp and Humidty DHT11 ----//
  float DHT11_Values[3] = {0, 0, 0};
  /*0 -> Humidity
    1 -> Temp
    2 -> Heat Index */
  if (readTempAndHumidity(DHT11_Values)) {
    Serial.print(F("Humidity: "));
    Serial.print(DHT11_Values[0]);
    Serial.print(F("%  Temperature: "));
    Serial.print(DHT11_Values[1]);
    Serial.print(F("°C "));
    Serial.print(F("Heat index: "));
    Serial.print(DHT11_Values[2]);
    Serial.println(F("°C "));
  }
  else {
    Serial.println(F("MAIN: Failed to read from DHT sensor!"));
  }
  //---------------------------------//

  //---- Pressure,Temp,Meters MS5611 ----//
  //double PressureSensor_Values[3] = {0, 0, 0};
  /*0 -> Pressure
    1 -> Temp
    2 -> Meters */
  if (PressureSensor.PressureSensorLoop(PressureSensor_Values)) {
    Serial.print(F("Pressure: "));
    Serial.print(PressureSensor_Values[0]);
    Serial.println(F(" Pascals"));

    Serial.print(F("Temperature: "));
    Serial.print(PressureSensor_Values[1]);
    Serial.println(F("°C"));

    Serial.print(F("Altitude: "));
    Serial.print(PressureSensor_Values[2]);
    Serial.println(F(" meters"));
  }
  else {
    Serial.println(F("MAIN: Failed to read from MS5611 Pressure Sensor"));
  }
  //---------------------------------//

  //---- LUX TSL256 ----//
  double luxValueFromFunction = 0;
  luxValueFromFunction = LuxSensor.LuxSensorLoop();
  if (luxValueFromFunction == -1) {
    Serial.println(F("MAIN: Failed to read from TSL256 Lux Sensor"));
  }
  else {
    Serial.println(luxValueFromFunction);
  }
  //---------------------------------//

  //---- LDR Sensors ----//
  int ldr_values[LDR_Size] = {0, 0, 0, 0, 0};
  readLDRData(ldr_values);
  //---------------------------------//

  //---- Telemetry ----//
  char text[36]; //LUX Eklencek
  snprintf(text, 36, "%d,%d,%d,%d,%d", ldr_values[0], ldr_values[1], ldr_values[2], ldr_values[3], ldr_values[4]);
  Serial.println(text);
  //---------------------------------//


  //---- LED DIMMER ----//
  LedDriver.LedDriverLoop(luxValueFromFunction,0,255);
  //-------------------//

  //---- SD CARD TEST ----//
  //SDCardClass.writeToSDCard("TEST2", "DATA3");
  //-------------------//


  //Timer for HTTP Request
  if ((millis() - lastTime) > timerDelay) {

    //SD CARD
    char dataForSDCard[500];
    //LDR1,LDR2,LDR3,LDR4,LDR5,TEMP1,TEMP2,HUMIDITY,HEATINDEX,AIRPRESSURE,ALTITUDE,LUX"
    sprintf(dataForSDCard, "%d,%d,%d,%d,%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n", ldr_values[0], ldr_values[1], ldr_values[2], ldr_values[3], ldr_values[4],
            DHT11_Values[1], PressureSensor_Values[1], DHT11_Values[0], DHT11_Values[2], PressureSensor_Values[0], PressureSensor_Values[2], luxValueFromFunction);
    SDCardClass.writeToSDCard("11-07-2021", dataForSDCard);


    if (WiFi.status() == WL_CONNECTED) {
      String httpRequestData = "";
      StaticJsonDocument<384> doc;
      doc["lastName"] = "test1";
      doc["firstMidName"] = "test2";
      doc["LDR1"] = map(ldr_values[0], 0, 4095, 0, 100);
      doc["LDR2"] = map(ldr_values[1], 0, 4095, 0, 100);
      doc["LDR3"] = map(ldr_values[2], 0, 4095, 0, 100);
      doc["LDR4"] = map(ldr_values[3], 0, 4095, 0, 100);
      doc["LDR5"] = map(ldr_values[4], 0, 4095, 0, 100);
      if (PressureSensor_Values[1] == 0) {
        doc["temperature"] = DHT11_Values[1];
      }
      else {
        doc["temperature"] = (PressureSensor_Values[1] + DHT11_Values[1]) / 2;
      }
      doc["humidity"] = DHT11_Values[0];
      doc["heatIndex"] = DHT11_Values[2];
      doc["airPressure"] = PressureSensor_Values[0];
      doc["altitude"] = PressureSensor_Values[2];
      doc["lux"] = luxValueFromFunction;
      serializeJson(doc, httpRequestData);
      //httpPostRequest(httpRequestData);
      //httpGETRequest(serverName2);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

//READ AND CHANGE LDR VALUES
void readLDRData(int *dest)
{
  int referenceLdrValue = 0;
  int ldrValue1 = 0;
  int ldrValue2 = 0;
  int ldrValue3 = 0;
  int ldrValue4 = 0;


  //referenceLdrValue = map(analogRead(referenceLdr),0,4095,0,100);
  float filteredReferenceLdrValue = adcFilter1.filter(analogRead(referenceLdr));
  //referenceLdrValue = map(filteredReferenceLdrValue, 0, 4095, 0, 100);

  //ldrValue1 = map(analogRead(ldrPin1),0,4095,0,100);
  float filteredLdrValue1 = adcFilter2.filter(analogRead(ldrPin1));
  ldrValue1 = map(filteredLdrValue1, 0, 4095, 0, 100);

  //ldrValue2 = map(analogRead(ldrPin2),0,4095,0,100);
  float filteredLdrValue2 = adcFilter3.filter(analogRead(ldrPin2));
  ldrValue2 = map(filteredLdrValue2, 0, 4095, 0, 100);

  //ldrValue3 = map(analogRead(ldrPin3),0,4095,0,100);
  float filteredLdrValue3 = adcFilter4.filter(analogRead(ldrPin3));
  ldrValue3 = map(filteredLdrValue3, 0, 4095, 0, 100);

  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, reg_b);
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);
  //We are using backup because adc2 doesnt work when wi-fi is on.
  //ldrValue4 = map(analogRead(ldrPinBackup),0,4095,0,100);
  float filteredLdrValue4 = adcFilter5.filter(analogRead(ldrPinBackup));
  ldrValue4 = map(filteredLdrValue4, 0, 4095, 0, 100);

  dest[0] = filteredReferenceLdrValue;
  dest[1] = filteredLdrValue1;
  dest[2] = filteredLdrValue2;
  dest[3] = filteredLdrValue3;
  dest[4] = filteredLdrValue4;

  //    dest[0] = referenceLdrValue;
  //    dest[1] = ldrValue1;
  //    dest[2] = ldrValue2;
  //    dest[3] = ldrValue3;
  //    dest[4] = ldrValue4;
  //return dest;
}

//POST REQUEST
String httpPostRequest(const String RequestData) {
  HTTPClient http;
  // Your IP address with path or Domain name with URL path
  http.begin(serverName);
  // Send HTTP POST request
  http.addHeader("Content-Type", "application/json");
  Serial.println("Data of POST ->");
  Serial.println(RequestData);
  int httpResponseCode = http.POST(RequestData);
  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  return payload;
}


//GET REQUEST
String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  Serial.println(payload);
  return payload;
}

int readTempAndHumidity(float *values) {
  float humidity = TempSensor.getRH();
  float temperature = TempSensor.getTemp();

  if (!(isnan(humidity) || isnan(temperature)) ) {
    float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
    values[0] = humidity;
    values[1] = temperature;
    values[2] = heatIndex;
    return 1; //1 = SUCCESS
  }
  else {
    return 0; //0 = FAILURE
  }
}
