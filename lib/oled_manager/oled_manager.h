#ifndef OLED_MANAGER_HEADER
#define OLED_MANAGER_HEADER

#include <ArduinoJson.h>
#include <Wire.h>
#include <U8x8lib.h>

class OLEDManager {
public:
    void begin(const int sda, const int scl);
    void displaySongName(const char* songName);

private:
    U8X8_SSD1306_128X64_NONAME_HW_I2C oled /*(U8X8_PIN_NONE)*/; 

};

#endif //OLED_MANAGER_HEADER