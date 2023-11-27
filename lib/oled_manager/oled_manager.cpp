#include "oled_manager.h"

void OLEDManager::begin(const int sda, const int scl) {
  Wire.setPins(sda, scl);
  Wire.begin(sda, scl);
  oled.begin();
  oled.setFont(u8x8_font_amstrad_cpc_extended_f);

}

void OLEDManager::displaySongName(const String songName){
  oled.clear();
  int screenWidth = oled.getCols();
  int textWidth = oled.getUTF8Len(songName.c_str());
  oled.setCursor((screenWidth - textWidth) / 2, oled.getRows() / 2);
  oled.print(songName);
}

void OLEDManager::scrollSongName(String songName, const unsigned short delayTime){
  oled.clear();
  int totalColums = oled.getCols();
  for (int i=0; i < oled.getCols(); i++) {
    songName = " " + songName;  
  } 
  songName = " " + songName; 
  for (int position = 0; position < songName.length(); position++) {
    oled.setCursor(0, oled.getRows() / 2);
    oled.print(songName.substring(position, position + oled.getCols()));
    delay(delayTime);
  }
}

void OLEDManager::displayPauseIcon(){
  oled.setCursor((oled.getCols() - 3) / 2, oled.getRows() / 2 + 1);
  oled.print("||");
}