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

String current_song_name;

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

      oled.displayAllComponents(current_song_name.c_str(), isPlaying_g, elapsedTimeInSeconds, totalTimeInSeconds);

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
    if (is_to_change_g)
    {
      current_track = (current_track + player_increment_global) % NUMBER_OF_TRACKS;
      am.playSong(current_track);
    }
  }

  vTaskDelete(NULL);
}

void buttonsTask(void *params)
{

  while (true)
  {
    switch (buttons.checkButtons())
    {
    case -1:
      current_track = --current_track % NUMBER_OF_TRACKS;
      am.playSong(current_track, &current_song_name);
      break;
    case 1:
      current_track = ++current_track % NUMBER_OF_TRACKS;
      am.playSong(current_track, &current_song_name);
      break;
    case 0:
      am.pauseSong();
      break;

    case default:
      break;
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}