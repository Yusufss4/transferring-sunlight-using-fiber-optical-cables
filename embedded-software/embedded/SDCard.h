#include "FS.h"
#include "SD.h"
#include "SPI.h"

class SDCardClass {
  private:
  public:
    void SDCardSetup() {
      if (!SD.begin(5)) {
        Serial.println("Card Mount Failed");
        return;
      }
      uint8_t cardType = SD.cardType();
      if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
      }

    }

    void appendFile(fs::FS &fs, const char * path, const char * message) {
//      Serial.printf("Appending to file: %s\n", path);
//
//      File file = fs.open(path, FILE_APPEND);
//      if (!file) {
//        Serial.println("Failed to open file for appending");
//        return;
//      }
//      if (file.print(message)) {
//        Serial.println("Message appended");
//      } else {
//        Serial.println("Append failed");
//      }
//      file.close();
    }

    void writeToSDCard(char * FILENAME, char * DATA) {
      char buffer[40];
      sprintf(buffer, "/%s.txt", FILENAME);
      fs::FS &fs = SD;
      Serial.print("\nBuffer -> ");
      Serial.println(buffer);
      char * path = buffer;
      Serial.print("\Path -> ");
      Serial.println(path);
      char * message = DATA;
      Serial.print("\Data -> ");
      Serial.println(message);

      File file = fs.open(path, FILE_APPEND);
      if (!file) {
        Serial.println("Failed to open file for appending");
        return;
      }
      if (file.print(message)) {
        Serial.println("Message appended");
      } else {
        Serial.println("Append failed");
      }
      file.close();


      // appendFile(SD, FILENAME, DATA);
      delay(1000);
    }


};

SDCardClass SDCardClass;
