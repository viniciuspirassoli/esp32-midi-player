#include <Arduino.h>
#include <Wire.h>
#include <U8x8lib.h>
#include "audio_manager.h"

// #include <LittleFS.h>
#include <ArduinoJson.h>
#include "pins.h"

#define DEFAULT_PRIORITY 5

AudioManager am;

void oledTask(void *params);
void blinkLed(void *params);
void updateAudioManager(void *params);

U8X8_SSD1306_128X64_NONAME_HW_I2C oled(/* reset=*/U8X8_PIN_NONE);

void setup()
{
  Wire.setPins((int) SDA, (int) SCL);
  Wire.begin((int) SDA, (int) SCL);
  oled.begin();
  oled.setFont(u8x8_font_amstrad_cpc_extended_f);
  oled.clear();
  oled.setCursor(0, 0);

  am.init();
  // am.playTest();

  TaskHandle_t oled_task = NULL;
  xTaskCreate(oledTask, "oled", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &oled_task);

  TaskHandle_t blink_led = NULL;
  xTaskCreate(blinkLed, "blink", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &blink_led);

  TaskHandle_t audio_manager_update = NULL;
  xTaskCreate(updateAudioManager, "audio_manager_update", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, DEFAULT_PRIORITY, &audio_manager_update);
}

void loop() {
  am.update();
}

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
  note_t notes[8] = {NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A, NOTE_B, NOTE_C};

  int note = 0;

  while (true)
  {
    ledcWriteNote(1, notes[note], 4);
    ledcWriteNote(2, notes[(note+2)%8], 4);
    ledcWriteNote(3, notes[(note+4)%8], 4);
    ledcWriteNote(4, notes[(note+6)%8], 4);


    vTaskDelay(300 / portTICK_PERIOD_MS);
    ledcWriteTone(2, 0);
    ledcWriteTone(1, 0);
    ledcWriteTone(3, 0);
    ledcWriteTone(4, 0);
    vTaskDelay(300 / portTICK_PERIOD_MS);

    note++;
    note %= 8;
  
  }

  vTaskDelete(NULL);
}