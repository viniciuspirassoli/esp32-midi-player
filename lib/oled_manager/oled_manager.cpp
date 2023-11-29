#include "oled_manager.h"

OLEDManager::OLEDManager() : oled(0){
  oled = new U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0,U8X8_PIN_NONE);
}

OLEDManager::~OLEDManager(){
  if (oled){
    delete oled;
  }
}

void OLEDManager::begin() {
  oled->setI2CAddress(0x3C * 2);
  oled->begin();
}

void OLEDManager::displayAllComponents(const char* songName, bool isPlaying, int elapsedTimeInSeconds, int totalDurationInSeconds){
    oled->firstPage();
    do{
        oled->setFont(u8g2_font_helvB08_tr);
        oled->setBitmapMode(1);
        oled->setFontMode(1);
        drawSongName(songName);
        if (isPlaying) {drawPauseIcon();} 
        else {drawPlayIcon();}
        drawPlayLeftRightButtons();
        oled->setFont(u8g2_font_4x6_tr);
        drawProgressBar(elapsedTimeInSeconds, totalDurationInSeconds);
    }while(oled->nextPage());
}

void OLEDManager::drawSongName(const char* songName){
  int screenWidth = oled->getWidth();
  int textWidth = oled->getUTF8Width(songName);
  oled->drawStr((screenWidth - textWidth) / 2, oled->getHeight() / 2 - 10, songName);
}

void OLEDManager::drawPauseIcon(){
  oled->drawLine(64, 51, 64, 55); 
  oled->drawLine(61, 51, 61, 55); 
}

void OLEDManager::drawPlayIcon(){
  oled->drawXBMP(61, 51, 3, 5, image_ButtonRightSmall_3x5_bits); 
}

void OLEDManager::drawPlayLeftRightButtons()
{
  oled->drawXBMP(50, 51, 3, 5, image_ButtonLeftSmall_3x5_bits); 
  oled->drawXBMP(47, 51, 3, 5, image_ButtonLeftSmall_3x5_bits); 

  oled->drawXBMP(71, 51, 3, 5, image_ButtonRightSmall_3x5_bits); 
  oled->drawXBMP(74, 51, 3, 5, image_ButtonRightSmall_3x5_bits); 

  oled->drawXBMP( 118, 1, 10, 10, image_music_10px_bits);
}

void OLEDManager::drawProgressBar(int elapsedTimeInSecond , int totalDurationInSecond)
{
  oled->drawFrame(36, 33, 54, 10);
  int progress = map(elapsedTimeInSecond, 0, totalDurationInSecond, 0, 50);
  oled->drawBox(38, 35, progress, 6);

  char buffer1[10], buffer2[10];
  secondsToMMSS(elapsedTimeInSecond, buffer1);
  secondsToMMSS(totalDurationInSecond - elapsedTimeInSecond, buffer2);
  oled->drawStr(13, 41, buffer1);
  oled->drawStr(95, 41, buffer2);
}

void OLEDManager::drawProgressBarVolume(){ //TODO later
  oled->drawFrame(0, 43, 8, 20);
  oled->drawFrame(10, 43, 8, 20);
  oled->drawFrame(110, 43, 8, 20);
  oled->drawFrame(120, 43, 8, 20);

  oled->drawBox(2, 48, 4, 13);
  oled->drawBox(12, 48, 4, 13);
  oled->drawBox(112, 48, 4, 13);
  oled->drawBox(122, 48, 4, 13);
}

void OLEDManager::secondsToMMSS(int timeInSeconds, char* buffer){
  sprintf(buffer, "%02d:%02d", timeInSeconds / 60, timeInSeconds % 60);
}

const unsigned char OLEDManager::image_ButtonLeftSmall_3x5_bits[] U8X8_PROGMEM = {0x04, 0x06, 0x07, 0x06, 0x04};
const unsigned char OLEDManager::image_ButtonRightSmall_3x5_bits[] U8X8_PROGMEM = {0x01, 0x03, 0x07, 0x03, 0x01};
const unsigned char OLEDManager::image_music_10px_bits[] U8X8_PROGMEM = {0xe0,0x00,0xe0,0x01,0x20,0x00,0x20,0x00,0x20,0x00,0x3e,0x00,0x3f,0x00,0x3f,0x00,0x1e,0x00,0x00,0x00};





/*static const unsigned char image_ButtonLeftSmall_3x5_bits[] U8X8_PROGMEM = {0x04,0x06,0x07,0x06,0x04};
static const unsigned char image_ButtonRightSmall_3x5_bits[] U8X8_PROGMEM = {0x01,0x03,0x07,0x03,0x01};
static const unsigned char image_music_10px_bits[] U8X8_PROGMEM = {0xe0,0x00,0xe0,0x01,0x20,0x00,0x20,0x00,0x20,0x00,0x3e,0x00,0x3f,0x00,0x3f,0x00,0x1e,0x00,0x00,0x00};
u8g2.setBitmapMode(1);
u8g2.setFontMode(1);
u8g2.drawXBMP( 50, 51, 3, 5, image_ButtonLeftSmall_3x5_bits);
u8g2.drawXBMP( 71, 51, 3, 5, image_ButtonRightSmall_3x5_bits);
u8g2.setFont(u8g2_font_helvB08_tr);
u8g2.drawStr(20, 23, "mii channel theme");
u8g2.drawBox(38, 35, 50, 6);
u8g2.drawXBMP( 47, 51, 3, 5, image_ButtonLeftSmall_3x5_bits);
u8g2.drawXBMP( 61, 51, 3, 5, image_ButtonRightSmall_3x5_bits);
u8g2.drawXBMP( 74, 51, 3, 5, image_ButtonRightSmall_3x5_bits);
u8g2.drawFrame(36, 33, 54, 10);
u8g2.drawXBMP( 118, 1, 10, 10, image_music_10px_bits);
u8g2.drawFrame(0, 44, 8, 20);
u8g2.drawFrame(10, 44, 8, 20);
u8g2.drawFrame(120, 44, 8, 20);
u8g2.drawFrame(110, 44, 8, 20);
u8g2.setFont(u8g2_font_4x6_tr);
u8g2.drawStr(93, 41, "3:23");
u8g2.setFont(u8g2_font_4x6_tr);
u8g2.drawStr(22, 41, "0:0");
u8g2.drawBox(2, 49, 4, 13);
u8g2.drawBox(12, 49, 4, 13);
u8g2.drawBox(112, 49, 4, 13);
u8g2.drawBox(122, 49, 4, 13);*/