#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "audio_manager.h"
#include "oled_manager.h"
#include "button_manager.h"
#include "pins.h"

#define DISPLAY_TASK_PERIOD_MS 200 
#define BUTTONS_TASK_PERIOD_MS 50   
#define MUSIC_TASK_PERIOD_MS   5

#define DISPLAY_TASK_PRIORITY  1
#define BUTTONS_TASK_PRIORITY  2
#define MUSIC_TASK_PRIORITY    3

int elapsedTimeInSeconds = 0, totalTimeInSeconds = 30; //Global variables


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

  xTaskCreate(updateAudioManager, "audio_manager_update", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, MUSIC_TASK_PRIORITY, NULL);
  xTaskCreate(buttonsTask, "buttons_manager", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, BUTTONS_TASK_PRIORITY, NULL);
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
    TickType_t previousWakeTime = xTaskGetTickCount();
    unsigned long currTime = millis();
    if (currTime >= lastTime + 1000UL && am.isPlaying())
    {

      if (elapsedTimeInSeconds > totalTimeInSeconds - 1)
      {
        elapsedTimeInSeconds = 0;
      }
      else
        elapsedTimeInSeconds++;

      lastTime = currTime;
    }
    oled.displayAllComponents(am.getSong().getName().c_str(), am.isPlaying(), elapsedTimeInSeconds, totalTimeInSeconds);
    vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
  }
  vTaskDelete(NULL);
}


void updateAudioManager(void *params)
{

  while (true)
  {
    TickType_t previousWakeTime = xTaskGetTickCount();
    am.update();
    vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(MUSIC_TASK_PERIOD_MS));
  }
  vTaskDelete(NULL);
}

void buttonsTask(void *params)
{
  while (true)
  {
    TickType_t previousWakeTime = xTaskGetTickCount();
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
      Serial.println("No pressed buttons");
      break;
    }
    vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(BUTTONS_TASK_PERIOD_MS));
  }
  vTaskDelete(NULL);
}