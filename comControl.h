#pragma once

#include <Arduino.h>
#include "misc.h"


class ComControl {
public:

    effectEntity * modes;
    EffectContainer * EContainer;
    unsigned char * effectIndex;
    bool * isTurnedOn;
    unsigned char * brightness;

    ComControl() {
        Serial.println("Com Initialization");
    }
    ComControl(EffectContainer * EContainer, unsigned char * effectIndex) {
        this->effectIndex= effectIndex;
        this->EContainer = EContainer;
        this->modes = EContainer->getEffectEntityContainer();
        this->isTurnedOn = new bool;
        this->brightness = new unsigned char;
        Serial.println("Com Initialization");
    }

    ComControl(EffectContainer * EContainer, unsigned char * effectIndex, bool * isTurnedOn, unsigned char * brightness) {
        this->effectIndex= effectIndex;
        this->EContainer = EContainer;
        this->modes = EContainer->getEffectEntityContainer();
        this->isTurnedOn = isTurnedOn;
        this->brightness = brightness;
        Serial.println("Com Initialization");
    }
    ~ComControl(){

    }


    virtual void parserLoop() {
        //Serial.println("Com Loop");
        if (Serial.available() > 0) {                 // если есть что то на вход
            char header = Serial.read();                // мы ждём префикс
            if (isAlpha(header)) {                      // если префикс БУКВА
                int intValue = 0;                         // обнуляем
                delay(2);
                while (Serial.available() > 0) {          // пока идут данные
                    char thisChar = Serial.read();          // читаем
                    if (!isDigit(thisChar)) break;         // если приходит НЕ ЦИФРА, покидаем парсинг
                    intValue = intValue * 10 + (thisChar - '0');  // с каждым принятым число растёт слева направо
                    delay(2);                                 // обязательно задержка, иначе вылетим из цикла раньше времени
                }
                switch (header) {             // раскидываем по переменным
                    case 'Q':                   // если заголовок а
                        Serial.println("Index + up to " + String((*effectIndex)+1));
                        (*effectIndex)++;
                        break;
                    case 'W':                   // если заголовок а
                        Serial.println("Index - up to " + String((*effectIndex)-1));
                        (*effectIndex)--;
                        break;
                    case 'E':                   // если заголовок а
                        modes[(*effectIndex)].effect = static_cast<effects>(intValue);
                        Serial.println("Changing mode up to " + String(effects(intValue)));
                        break;
                    case 'R':                   // если заголовок а
                        modes[(*effectIndex)].colorA = intValue;
                        break;
                    case 'T':                   // если заголовок а
                        modes[(*effectIndex)].colorB = intValue;
                        break;
                    case 'Y':                   // если заголовок а
                        modes[(*effectIndex)].speed = intValue;
                        break;
                    case 'U':                   // если заголовок а
                        modes[(*effectIndex)].power = intValue;
                        break;
                    case 'I':                   // если заголовок а
                        (*isTurnedOn) = intValue;
                        Serial.println("Is turned on: " + String((*isTurnedOn)));
                        break;
                    case 'O':                   // если заголовок а
                        (*brightness) = intValue;
                        Serial.println("Brightness: " + String((*brightness)));
                        break;
                }
                Serial.println(modes[(*effectIndex)].toString());
                Serial.println("INDEX: " + String((*effectIndex)));
                EContainer->writeToEEPROM();
            }
        }
    }

};