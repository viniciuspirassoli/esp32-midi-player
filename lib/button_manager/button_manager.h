#ifndef BUTTON_MANAGER_HEADER
#define BUTTON_MANAGER_HEADER

#include <Arduino.h>

enum CHECK_BUTTON_RETURN
{
    ERROR_CHECK_BUTTONS = -1,
    DO_NOTHING,
    GO_PREVIOUS_SONG,
    PAUSE_SONG,
    UNPAUSE_SONG,
    GO_NEXT_SONG
};

class ButtonManager
{
public:
    ButtonManager(unsigned short leftButtonPin, unsigned short rightButtonPin, unsigned short playPauseButtonPin);
    void begin();
    unsigned short checkButtons();

private:
    unsigned short leftButtonPin;
    unsigned short rightButtonPin;
    unsigned short playPauseButtonPin;
    bool lastPauseButton = true;
    bool lastRightButton = true;
    bool lastLeftButton = true;
    bool paused = false;
};

#endif // BUTTON_MANAGER_HEADER