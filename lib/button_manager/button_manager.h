#ifndef BUTTON_MANAGER_HEADER
#define BUTTON_MANAGER_HEADER

#include <ArduinoJson.h>

class ButtonManager {
public:
    ButtonManager(int leftButtonPin, int rightButtonPin, int playPauseButtonPin);
    void begin();
    unsigned short checkButtons();

private:
    int leftButtonPin;
    int rightButtonPin;
    int playPauseButtonPin;

};

#endif //BUTTON_MANAGER_HEADER