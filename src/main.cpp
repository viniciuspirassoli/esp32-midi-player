#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "audio_manager.h"
#include "oled_manager.h"
#include "button_manager.h"
#include "pins.h"

#define DEFAULT_PRIORITY 5

AudioManager am;
OLEDManager oled;
ButtonManager buttons(LEFT_BUTTON, RIGHT_BUTTON, PAUSE_PLAY_BUTTON);

void oledTask(void *params);
void buttonsTask(void *params);
void updateAudioManager(void *params);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  am.init();
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  int song_number = 3;
  if (!am.playSong(song_number))
  {
    Serial.println("Error in loading the song!");
    am.stopSong();
  }

  oled.begin();
  buttons.begin();

  xTaskCreate(oledTask, "oled", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, NULL);
  xTaskCreate(updateAudioManager, "audio_manager_update", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, NULL);
  xTaskCreate(buttonsTask, "buttons_manager", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, NULL);
}

void loop()
{
  /*Nothing here*/
}

void oledTask(void *params)
{
  int elapsedTimeInSeconds = 0, totalTimeInSeconds = 30;
  unsigned long lastTime = millis();

  while (true)
  {
    unsigned long currTime = millis();
    if (currTime >= lastTime + 1000UL)
    {

      if (elapsedTimeInSeconds > totalTimeInSeconds - 1)
      {
        elapsedTimeInSeconds = 0;
      }
      else
        elapsedTimeInSeconds++;

      oled.displayAllComponents(am.getSong().getName().c_str(), am.isPlaying(), elapsedTimeInSeconds, totalTimeInSeconds);

      lastTime = currTime;
    }
  }

  vTaskDelete(NULL);
}

void updateAudioManager(void *params)
{
  static int current_track = 0;

  while (true)
  {
    am.update();
  }

  vTaskDelete(NULL);
}

void buttonsTask(void *params)
{
  while (true)
  {
    switch (buttons.checkButtons())
    {
    case 1:
      am.skipSongs(1);
      break;
    case 2:
      am.skipSongs(-1);
      break;
    case 3:
      am.pauseSong();
      break;
    default:
      Serial.print("Error: in buttonsTask");
      break;
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}