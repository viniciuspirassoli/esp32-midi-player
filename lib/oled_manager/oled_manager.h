#ifndef OLED_MANAGER_HEADER
#define OLED_MANAGER_HEADER

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

class OLEDManager {
public:
    void begin(const int sda, const int scl);
    void displayAllComponents(const char* songName, bool isPlayButtonPressed, int elapsedTimeInSeconds, int totalDurationInSeconds);
    void displaySongName(const char* songName);
    void displayPauseIcon();
    void displayPlayIcon();
    void displayPlayLeftRightButtons();
    void displayProgressBar(int elapsedTimeInSecond , int totalDurationInSecond);

private:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled /*(U8X8_PIN_NONE)*/;
    int lastPressedButton;
    static const unsigned char image_ButtonLeftSmall_3x5_bits[] U8X8_PROGMEM;
    static const unsigned char image_ButtonRightSmall_3x5_bits[] U8X8_PROGMEM; 

};

#endif //OLED_MANAGER_HEADER