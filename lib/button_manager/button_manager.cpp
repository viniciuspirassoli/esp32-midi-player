#include "button_manager.h"

ButtonManager::ButtonManager(int leftPin, int rightPin, int playPausePin) {
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
}
