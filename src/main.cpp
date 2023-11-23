#include <Arduino.h>
#include <Wire.h>
#include <U8x8lib.h>
#include "audio_manager.h"

// #include <LittleFS.h>
// #include <ArduinoJson.h>
#include "pins.h"

#define DEFAULT_PRIORITY 5

AudioManager am;

void oledTask(void *params);
void blinkLed(void *params);
void updateAudioManager(void *params);

U8X8_SSD1306_128X64_NONAME_HW_I2C oled(/* reset=*/U8X8_PIN_NONE);

void setup()
{
  oled.begin();
  oled.setFont(u8x8_font_amstrad_cpc_extended_f);
  oled.clear();
  oled.setCursor(0, 0);

  am.init();
  am.playTest();

  TaskHandle_t oled_task = NULL;
  xTaskCreate(oledTask, "oled", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &oled_task);

  TaskHandle_t blink_led = NULL;
  xTaskCreate(blinkLed, "blink", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &blink_led);

  TaskHandle_t audio_manager_update = NULL;
  // xTaskCreate(updateAudioManager, "audio_manager_update", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &audio_manager_update);
}

void loop() {}

void oledTask(void *params)
{
  while (true)
  {
    oled.noInverse();
    oled.print("Fuck");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    oled.clear();
    oled.inverse();
    oled.print("You");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    oled.clear();
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
  while (true)
  {
    am.update();
  }
}
