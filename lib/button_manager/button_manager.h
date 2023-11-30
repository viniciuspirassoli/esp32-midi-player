#ifndef BUTTON_MANAGER_HEADER
#define BUTTON_MANAGER_HEADER

#include <ArduinoJson.h>

class ButtonManager {
public:
    ButtonManager(unsigned short leftButtonPin, unsigned short rightButtonPin, unsigned short playPauseButtonPin);
    void begin();
    unsigned short checkButtons();

private:
    unsigned short leftButtonPin;
    unsigned short rightButtonPin;
    unsigned short playPauseButtonPin;
};

#endif //BUTTON_MANAGER_HEADER