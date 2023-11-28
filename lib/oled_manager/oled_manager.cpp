#include "oled_manager.h"

OLEDManager::OLEDManager() : oled(0){
  oled = new U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0,U8X8_PIN_NONE);
}

OLEDManager::~OLEDManager(){
  if (oled){
    delete oled;
  }
}

void OLEDManager::begin(const int sda, const int scl) {
  Wire.begin(sda, scl);
  oled->begin();
  oled->clearBuffer();
  this->lastPressedButton = 0;
}

void OLEDManager::displayAllComponents(const char* songName, bool isPlayButtonPressed, int elapsedTimeInSeconds, int totalDurationInSeconds){
    lastPressedButton = isPlayButtonPressed ? !lastPressedButton : lastPressedButton;
    oled->firstPage();
    do{
        oled->setFont(u8g2_font_helvB08_tr);
        oled->setBitmapMode(1);
        oled->setFontMode(1);
        drawSongName(songName);
        /*if (lastPressedButton == 0) {drawPlayIcon();} 
        else {drawPauseIcon();}*/
        drawPauseIcon();
        drawPlayLeftRightButtons();
        drawProgressBar(elapsedTimeInSeconds, totalDurationInSeconds);
    }while(oled->nextPage());
}

void OLEDManager::drawSongName(const char* songName){
  int screenWidth = oled->getWidth();
  int textWidth = oled->getUTF8Width(songName);
  oled->drawStr((screenWidth - textWidth) / 2, oled->getHeight() / 2 - 10, songName);
}

void OLEDManager::drawPauseIcon(){
  oled->drawLine(64, 51, 64, 55); //TODO change these values
  oled->drawLine(61, 51, 61, 55); //TODO change these values
}

void OLEDManager::drawPlayIcon(){
  oled->drawXBMP(61, 51, 3, 5, image_ButtonRightSmall_3x5_bits); //TODO change these values 
}

void OLEDManager::drawPlayLeftRightButtons()
{
  oled->drawXBMP(50, 51, 3, 5, image_ButtonLeftSmall_3x5_bits); //TODO change these values
  oled->drawXBMP(47, 51, 3, 5, image_ButtonLeftSmall_3x5_bits); //TODO change these values

  oled->drawXBMP(71, 51, 3, 5, image_ButtonRightSmall_3x5_bits); //TODO change these values
  oled->drawXBMP(74, 51, 3, 5, image_ButtonRightSmall_3x5_bits); //TODO change these values
}

void OLEDManager::drawProgressBar(int elapsedTimeInSecond , int totalDurationInSecond)
{
  oled->drawFrame(36, 33, 54, 10); //TODO change these values
  int progress = map(elapsedTimeInSecond, 0, totalDurationInSecond, 0, 50);
  oled->drawBox(38, 35, progress, 6);
}

const unsigned char OLEDManager::image_ButtonLeftSmall_3x5_bits[] U8X8_PROGMEM = {0x04, 0x06, 0x07, 0x06, 0x04};
const unsigned char OLEDManager::image_ButtonRightSmall_3x5_bits[] U8X8_PROGMEM = {0x01, 0x03, 0x07, 0x03, 0x01};
