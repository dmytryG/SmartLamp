#include <Arduino.h>
#include "misc.h"
#include "Button.h"

class AnalogController
{
private:
    effectEntity * modes;
    EffectContainer * EContainer;
    unsigned char * effectIndex;
    bool * isTurnedOn;
    unsigned char * brightness;
    uint8_t resistorPin = A0;
    uint8_t buttonPin = 7;
    Button * mainButton;
    unsigned char lastBrightness;

    void init() {
        mainButton = new Button(buttonPin, true, true);
        lastBrightness = 0;
    }
public:

    AnalogController() {
        
    }
    AnalogController(EffectContainer * EContainer, unsigned char * effectIndex) {
        this->effectIndex= effectIndex;
        this->EContainer = EContainer;
        this->modes = EContainer->getEffectEntityContainer();
        this->isTurnedOn = new bool;
        this->brightness = new unsigned char;
        init();
    }

    AnalogController(EffectContainer * EContainer, unsigned char * effectIndex, bool * isTurnedOn, unsigned char * brightness) {
        this->effectIndex= effectIndex;
        this->EContainer = EContainer;
        this->modes = EContainer->getEffectEntityContainer();
        this->isTurnedOn = isTurnedOn;
        this->brightness = brightness;
        init();
    }

    AnalogController(EffectContainer * EContainer, unsigned char * effectIndex, bool * isTurnedOn, unsigned char * brightness, uint8_t resistorPin, uint8_t buttonPin) {
        this->effectIndex= effectIndex;
        this->EContainer = EContainer;
        this->modes = EContainer->getEffectEntityContainer();
        this->isTurnedOn = isTurnedOn;
        this->brightness = brightness;
        this->buttonPin = buttonPin;
        this->resistorPin = resistorPin;
        init();
    }
    ~AnalogController(){

    }

    void loop() {
        unsigned char btmp = map(analogRead(resistorPin), 0, 1024, 0, 255);
        if(!(abs(btmp - lastBrightness) < 5)) {
            (*brightness) = btmp;
            lastBrightness = btmp;
        }
        if(mainButton->getIsButtonClicked()) {
            (*isTurnedOn) = !(*isTurnedOn);
        }
        if(mainButton->getIsButtonHold()) {
            (*effectIndex)++;
            if((*effectIndex) >= EffectContainer::CONTAINER_CAPACITY-1) {
                (*effectIndex) = 0;
            }
            Serial.println("Index + up to " + String((*effectIndex)));
        }

    }

    Button * getMainButtonForLoop() {
        return mainButton;
    }

};
