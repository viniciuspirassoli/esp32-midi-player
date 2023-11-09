#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "../pins/Pins.h"

#define DEFAULT_STACK_SIZE 50 // words
#define DEFAULT_PRIORITY 5

void LCDTask(void *params);
void audioPlayerTask(void *params);

void IRAM_ATTR keyboardInterrupt();

LiquidCrystal_I2C LCD(LCD_ADDR, 16, 2);

void setup()
{

  // setup pins
  pinMode(OUTPUT_1, OUTPUT);
  pinMode(OUTPUT_2, OUTPUT);
  pinMode(OUTPUT_3, OUTPUT);
  pinMode(OUTPUT_4, OUTPUT);

  pinMode(LEFT_BUTTON, INPUT_PULLDOWN);
  pinMode(RIGHT_BUTTON, INPUT_PULLDOWN);
  pinMode(PAUSE_PLAY_BUTTON, INPUT_PULLDOWN);

  // setup LCD
  LCD.init();
  LCD.begin(16, 2);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.noCursor();

  // setup PWM channels
  ledcSetup(0, 100, 8);
  ledcSetup(1, 100, 8);
  ledcSetup(2, 100, 8);
  ledcSetup(3, 100, 8);

  ledcAttachPin(OUTPUT_1, 0);
  ledcAttachPin(OUTPUT_2, 1);
  ledcAttachPin(OUTPUT_3, 2);
  ledcAttachPin(OUTPUT_4, 3);

  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);

  if (LittleFS.begin())
    Serial.println("LittleFS Initialize....ok");
  else
  {
    Serial.println("LittleFS Initialize....error");
    delay(1000);
    ESP.restart();
  }

  // create Tasks
  // TODO: change priorities
  TaskHandle_t LCD = NULL;
  if (xTaskCreate(LCDTask, "LCD", DEFAULT_STACK_SIZE, NULL, DEFAULT_PRIORITY, &LCD) != pdPASS)
  {
    exit(-1);
  }

  TaskHandle_t audio_Player = NULL;
  if (xTaskCreate(audioPlayerTask, "audioPlayer", DEFAULT_STACK_SIZE, NULL, DEFAULT_PRIORITY, &audio_Player) != pdPASS)
  {
    exit(-1);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void LCDTask(void *params)
{
  while (true)
  {
    LCD.printstr("Sus amogus");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void audioPlayerTask(void *params)
{
}