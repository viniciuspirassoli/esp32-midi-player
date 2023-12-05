#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "audio_manager.h"
#include "oled_manager.h"
#include "button_manager.h"
#include "pins.h"

#define DISPLAY_TASK_PERIOD_MS 200
#define BUTTONS_TASK_PERIOD_MS 50
#define MUSIC_TASK_PERIOD_MS 5

#define DISPLAY_TASK_PRIORITY 1
#define BUTTONS_TASK_PRIORITY 2
#define MUSIC_TASK_PRIORITY 3

int elapsedTimeInSeconds = 0, totalTimeInSeconds = 30; // Global variables

AudioManager am;
OLEDManager oled;
ButtonManager buttons(LEFT_BUTTON, RIGHT_BUTTON, PAUSE_PLAY_BUTTON);

void oledTask(void *params);
void buttonsTask(void *params);
void updateAudioManager(void *params);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting the midi player...");

  am.init();
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  unsigned int song_number = 0;
  if (!am.playSong(song_number))
  {
    Serial.println("Error in loading the song!");
    am.stopSong(false);
    ESP.restart();
  }

  oled.begin();
  buttons.begin();

  xTaskCreate(updateAudioManager, "audio_manager_update", 1024 * 15, NULL, 5, NULL);
  xTaskCreate(buttonsTask, "buttons_manager", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, 6, NULL);
  xTaskCreate(oledTask, "oled", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, NULL);
}

void loop()
{
  /*Nothing here*/
}

void oledTask(void *params)
{
  unsigned long lastTime = millis();
  while (true)
  {
    unsigned long currTime = millis();
    if (currTime >= lastTime + 1000UL && am.isPlaying())
    {

      if (elapsedTimeInSeconds > am.getSong().get_song_duration())
      {
        elapsedTimeInSeconds = 0;
      }
      else
        elapsedTimeInSeconds++;

      lastTime = currTime;
    }
    // TODO OPT THIS
    oled.displayAllComponents(am.getSong().getName().c_str(), am.isPlaying(), am.getCurrentTime(), am.getSong().get_song_duration());
    vTaskDelay(DISPLAY_TASK_PERIOD_MS / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void updateAudioManager(void *params)
{

  while (true)
  {
    am.update();
    vTaskDelay(8 / portTICK_PERIOD_MS);
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
      Serial.println("am.skipSongs");
      am.pauseSong();
      break;
    default:
      break;
    }
    vTaskDelay(DISPLAY_TASK_PERIOD_MS / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}