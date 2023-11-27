#include "oled_manager.h"

void OLEDManager::begin(const int sda, const int scl) {
  Wire.setPins(sda, scl);
  Wire.begin(sda, scl);
  oled.begin();
  oled.setFont(u8x8_font_amstrad_cpc_extended_f);

}

void OLEDManager::displaySongName(const char* songName)
{
  oled.setFont(u8x8_font_amstrad_cpc_extended_f);
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(songName);
}