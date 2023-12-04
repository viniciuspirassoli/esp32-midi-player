#include "button_manager.h"

ButtonManager::ButtonManager(unsigned short leftPin, unsigned short rightPin, unsigned short playPausePin)
{
    leftButtonPin = leftPin;
    rightButtonPin = rightPin;
    playPauseButtonPin = playPausePin;
}

void ButtonManager::begin()
{
    pinMode(leftButtonPin, INPUT_PULLUP);
    pinMode(rightButtonPin, INPUT_PULLUP);
    pinMode(playPauseButtonPin, INPUT_PULLUP);
}

unsigned short ButtonManager::checkButtons()
{
    if (digitalRead(leftButtonPin) == LOW)
    {
        return 1;
    }

    if (digitalRead(rightButtonPin) == LOW)
    {
        return 2;
    }

    if (digitalRead(playPauseButtonPin) == LOW)
    {
        return 3;
    }

    else
    {
        return 0;
    }
}
