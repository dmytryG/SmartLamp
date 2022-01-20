

#include "stripInit.h"
#include "misc.h"
#include "showEffect.h"
#include "Button.h"
#include "comApi.h"
#include "Timer.h"
#include "AnalogController.h"
//#include <seria>


unsigned char currentMode = 0;
effectEntity * modes;
EffectShower * effectShower;
unsigned char brightness = 255;
bool isTurnedOn = true;
EffectContainer * effectContainer;
ComApi * comControl;
AnalogController * analogController;

Timer watchDogTimer(true, 1000);
Timer stripUpdate(true, 150);

Button * mainButton;

void setup() {
  Serial.begin(19200);
  Serial.println("HUY");

  strip.setBrightness(255);

  effectContainer = new EffectContainer();

  modes = effectContainer->getEffectEntityContainer();
  effectShower = new EffectShower();

  pinMode(13, OUTPUT);
  digitalWrite(13, 0);

/*
  modes[0].setup(EFFECT_RAINBOW, 0, 0);
  modes[1].setup(EFFECT_STARFADE, 135, 180, 30, 20);
  modes[2].setup(EFFECT_NEON, 145, 255, 10, 20);
  modes[3].setup(EFFECT_RAINBOWRANGE, 135, 180, 50, 10);
*/

  effectContainer->loadFromEEPROM();
  effectContainer->normalizeContainer();

  comControl = new ComApi(effectContainer, &currentMode, &isTurnedOn, &brightness);
  analogController = new AnalogController(effectContainer, &currentMode, &isTurnedOn, &brightness);

  mainButton = analogController->getMainButtonForLoop();

}

void setLEDBrightness() {
  if(isTurnedOn) {
    strip.setBrightness(brightness);
  }
  else {
    strip.setBrightness(0);
  }
}

void loop() {
  mainButton->buttonLoop();

  comControl->parserLoop();

  effectShower->setEffect(modes[currentMode]);

  if(stripUpdate.checkTimer()) {
    setLEDBrightness();
    analogController->loop();
  }

  
}
