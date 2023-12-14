#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "audio_manager.h"
#include "oled_manager.h"
#include "button_manager.h"
#include "pins.h"

// Uncomment for task execution time stats
// #define MEASURE_STATS

#define DISPLAY_TASK_PERIOD_MS 200
#define BUTTONS_TASK_PERIOD_MS 50
#define TRACK_TASK_PERIOD_MS 8

AudioManager am;
OLEDManager oled;
ButtonManager buttons(LEFT_BUTTON, RIGHT_BUTTON, PAUSE_PLAY_BUTTON);

void oledTask(void *params);
void buttonsTask(void *params);
void trackUpdate(void *params);
int param1 = 0, param2 = 1, param3 = 2, param4 = 3;

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

  xTaskCreate(trackUpdate, "track_1_update", 1024 * 4, (void *)&param1, 5, NULL);
  xTaskCreate(trackUpdate, "track_2_update", 1024 * 4, (void *)&param2, 5, NULL);
  xTaskCreate(trackUpdate, "track_3_update", 1024 * 4, (void *)&param3, 5, NULL);
  xTaskCreate(trackUpdate, "track_4_update", 1024 * 4, (void *)&param4, 5, NULL);

  xTaskCreate(buttonsTask, "buttons_manager", 2 * CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, 6, NULL);
  xTaskCreate(oledTask, "oled", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, 1, NULL);

}

void loop()
{
  /*Nothing here*/
}

void oledTask(void *params)
{
  TickType_t previousWakeTime = xTaskGetTickCount();
  while (true)
  {
    #ifdef MEASURE_STATS
    TickType_t currTime = xTaskGetTickCount();
    #endif

    oled.displayAllComponents(am.getSong().getName().c_str(), 
                              am.isPlaying(), (int)(am.getCurrentTime() / 1000), 
                              am.getSong().get_song_duration());

    #ifdef MEASURE_STATS                          
    TickType_t execTime = xTaskGetTickCount() - currTime;
    Serial.print("oledTask "); Serial.println(execTime);
    #endif

    vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
  }
  vTaskDelete(NULL);
}

void trackUpdate(void *params)
{
  int i = *((int *)params);
  TickType_t previousWakeTime = xTaskGetTickCount();
  while (true)
  {
    #ifdef MEASURE_STATS
    TickType_t currTime = xTaskGetTickCount();
    #endif

    am.update_track(i);

    #ifdef MEASURE_STATS                          
    TickType_t execTime = xTaskGetTickCount() - currTime;
    Serial.print("trackUpdate"); Serial.print(i); Serial.print(" "); Serial.println(execTime);
    #endif

    vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(TRACK_TASK_PERIOD_MS));
  }
  vTaskDelete(NULL);
}

void buttonsTask(void *params)
{
  TickType_t previousWakeTime = xTaskGetTickCount();
  while (true)
  {
    #ifdef MEASURE_STATS
    TickType_t currTime = xTaskGetTickCount();
    #endif

    switch (buttons.checkButtons())
    {
    case GO_NEXT_SONG:
      am.skipSongs(1);
      // Serial.println("go next song");
      break;
    case GO_PREVIOUS_SONG:
      am.skipSongs(NUMBER_OF_SONGS - 1);
      // Serial.println("go prev song");
      break;
    case PAUSE_SONG:
      am.pauseSong();
      // Serial.println("pausing song");
      break;
    case UNPAUSE_SONG:
      am.unpauseSong();
      // Serial.println("unpausing song");
      break;
    default:
      break;
    }
    
    #ifdef MEASURE_STATS
    TickType_t execTime = xTaskGetTickCount() - currTime;
    Serial.print("buttonsTask "); Serial.println(execTime);
    #endif

    vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(BUTTONS_TASK_PERIOD_MS));
  }
  vTaskDelete(NULL);
}