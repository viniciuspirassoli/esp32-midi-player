#ifndef OLED_MANAGER_HEADER
#define OLED_MANAGER_HEADER

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

class OLEDManager {
public:
    OLEDManager();
    ~OLEDManager();
    void begin();
    void displayAllComponents(const char* songName, bool isPlaying, int elapsedTimeInSeconds, int totalDurationInSeconds);
    void drawSongName(const char* songName);
    void drawPauseIcon();
    void drawPlayIcon();
    void drawPlayLeftRightButtons();
    void drawProgressBar(int elapsedTimeInSecond , int totalDurationInSecond);
    void drawProgressBarVolume();
    void secondsToMMSS(int timeInSeconds, char* buffer);

private:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C* oled;
    static const unsigned char image_ButtonLeftSmall_3x5_bits[] U8X8_PROGMEM;
    static const unsigned char image_ButtonRightSmall_3x5_bits[] U8X8_PROGMEM;
    static const unsigned char image_music_10px_bits[] U8X8_PROGMEM; 
};

#endif //OLED_MANAGER_HEADER