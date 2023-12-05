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
    if (digitalRead(leftButtonPin) == LOW && lastLeftButton == HIGH)
    {
        lastLeftButton = LOW;
        return GO_PREVIOUS_SONG;
    }
    else if (digitalRead(leftButtonPin) == HIGH) lastLeftButton = HIGH;
 
    if (digitalRead(rightButtonPin) == LOW && lastRightButton == HIGH)
    {
        lastRightButton = LOW;
        return GO_NEXT_SONG;
    }
    else if (digitalRead(rightButtonPin) == HIGH) lastRightButton = HIGH;

    if (digitalRead(playPauseButtonPin) == LOW && lastPauseButton == HIGH) // FE playPauseButton
    {   
        lastPauseButton = LOW;
        if (!this->paused) {
            this->paused = true;
            return PAUSE_SONG;
        }
        
        else
        {
            this->paused = false;
            return UNPAUSE_SONG;
        }
    }
    else if (digitalRead(playPauseButtonPin) == HIGH) lastPauseButton = HIGH;  

    return DO_NOTHING;
}
