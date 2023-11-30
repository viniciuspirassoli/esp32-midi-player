#include "button_manager.h"

ButtonManager::ButtonManager(unsigned short leftPin, unsigned short rightPin, unsigned short playPausePin) {
    leftButtonPin = leftPin;
    rightButtonPin = rightPin;
    playPauseButtonPin = playPausePin;
}

void ButtonManager::begin() {
    pinMode(leftButtonPin, INPUT);
    pinMode(rightButtonPin, INPUT);
    pinMode(playPauseButtonPin, INPUT);
}

unsigned short ButtonManager::checkButtons() {
    if (digitalRead(leftButtonPin) == HIGH) {
        return leftButtonPin;
    }

    if (digitalRead(rightButtonPin) == HIGH) {
        return rightButtonPin;
    }

    if (digitalRead(playPauseButtonPin) == HIGH) {
        return playPauseButtonPin;
    }
    
    else {
        return 0;
    }
}
