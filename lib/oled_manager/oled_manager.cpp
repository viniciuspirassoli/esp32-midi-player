#include "oled_manager.h"

void OLEDManager::begin(const int sda, const int scl) {
  Wire.setPins(sda, scl);
  Wire.begin(sda, scl);
  oled.setDisplayRotation(U8G2_R0);
  oled.begin();
  oled.clear();
  this->lastPressedButton = 0;
}

void OLEDManager::displayAllComponents(const char* songName, bool isPlayButtonPressed, int elapsedTimeInSeconds, int totalDurationInSeconds) {
    oled.clear();

    lastPressedButton = isPlayButtonPressed ? !lastPressedButton : lastPressedButton;

    displaySongName(songName);
    if (lastPressedButton == 0) {
        displayPlayIcon();
    } else {
        displayPauseIcon();
    }

    displayPlayLeftRightButtons();

    displayProgressBar(elapsedTimeInSeconds, totalDurationInSeconds);

    oled.display();
}

void OLEDManager::displaySongName(const char* songName){
  oled.setFont(u8g2_font_helvB08_tr);
  int screenWidth = oled.getCols();
  int textWidth = oled.getUTF8Width(songName);
  oled.drawStr((screenWidth - textWidth) / 2, 20, songName);
}

void OLEDManager::displayPauseIcon(){
  oled.drawLine(63, 51, 63, 55); //TODO change these values
  oled.drawLine(60, 51, 60, 55); //TODO change these values
}

void OLEDManager::displayPlayIcon(){
  oled.drawXBMP(61, 51, 3, 5, image_ButtonRightSmall_3x5_bits); //TODO change these values 
}

void OLEDManager::displayPlayLeftRightButtons()
{
  oled.drawXBMP(50, 51, 3, 5, image_ButtonLeftSmall_3x5_bits); //TODO change these values
  oled.drawXBMP(47, 51, 3, 5, image_ButtonLeftSmall_3x5_bits); //TODO change these values

  oled.drawXBMP(71, 51, 3, 5, image_ButtonRightSmall_3x5_bits); //TODO change these values
  oled.drawXBMP(74, 51, 3, 5, image_ButtonRightSmall_3x5_bits); //TODO change these values
}

void OLEDManager::displayProgressBar(int elapsedTimeInSecond , int totalDurationInSecond)
{
  int progress = (elapsedTimeInSecond / totalDurationInSecond) * 50; //TODO change these values
  oled.drawBox(38, 35, progress, 6);
}

const unsigned char OLEDManager::image_ButtonLeftSmall_3x5_bits[] U8X8_PROGMEM = {0x04, 0x06, 0x07, 0x06, 0x04};
const unsigned char OLEDManager::image_ButtonRightSmall_3x5_bits[] U8X8_PROGMEM = {0x01, 0x03, 0x07, 0x03, 0x01};
