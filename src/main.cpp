#include <Arduino.h>
#include <Wire.h>
#include <U8x8lib.h>
#include "audio_manager.h"
#include <LittleFS.h>
#include "oled_manager.h"
#include <ArduinoJson.h>
#include "pins.h"

#define DEFAULT_PRIORITY 5

AudioManager am;
OLEDManager oledMan;

int player_increment_global = 0;

void oledTask(void *params);
void blinkLed(void *params);
void updateAudioManager(void *params);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  Wire.setPins((int)SDA, (int)SCL);
  Wire.begin((int)SDA, (int)SCL);

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

  oledMan.begin();

  TaskHandle_t oled_task = NULL;
  xTaskCreate(oledTask, "oled", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &oled_task);

  TaskHandle_t blink_led = NULL;
  xTaskCreate(blinkLed, "blink", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &blink_led);

  TaskHandle_t audio_manager_update = NULL;
  xTaskCreate(updateAudioManager, "audio_manager_update", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &audio_manager_update);
}

void loop()
{
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

      oledMan.displayAllComponents("Test 1@&", false, elapsedTimeInSeconds, totalTimeInSeconds);

      lastTime = currTime;
    }
  }

  vTaskDelete(NULL);
}

void blinkLed(void *params)
{
  pinMode(2, OUTPUT);
  while (true)
  {
    digitalWrite(2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
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