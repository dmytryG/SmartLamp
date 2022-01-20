#pragma once

#include <Arduino.h>
#include "misc.h"
#include "comControl.h"


    enum API_MODE{
        API_MODE_INIT,   //Передача всех режимов
        API_MODE_READ,   //Передача режима по индексу
        API_MODE_PREV_INDEX,
        API_MODE_NEXT_INDEX,
        API_MODE_SET_EFFECT,
        API_MODE_SET_COLOR_A,
        API_MODE_SET_COLOR_B,
        API_MODE_SET_SPEED,
        API_MODE_SET_POWER,
        API_MODE_SET_ON_OFF,
        API_MODE_SET_BRIGHTNESS,
    };
    

class ComApi : public ComControl{
public:


    void init() {

    }
    ComApi() {
        init();
    }
    ComApi(EffectContainer * EContainer, unsigned char * effectIndex) : ComControl(EContainer, effectIndex){
        init();
    }

    ComApi(EffectContainer * EContainer, unsigned char * effectIndex, bool * isTurnedOn, unsigned char * brightness) : ComControl(EContainer, effectIndex, isTurnedOn, brightness) {
        Serial.println("Api Initialization");
        init();
    }
    ~ComApi(){

    }


    void parserLoop() override {
        //Serial.println("API Loop");
        if (Serial.available() > 0) {                 // если есть что то на вход
            byte header = Serial.read();
            byte body = Serial.read();
            byte endofrec = Serial.read();
            Serial.print(char(header));
            Serial.print(char(body));
            switch (header) {             // раскидываем по переменным
                case API_MODE_NEXT_INDEX:                   // если заголовок а
                    (*effectIndex)++;
                    break;
                case API_MODE_PREV_INDEX:                   // если заголовок а
                    (*effectIndex)--;
                    break;
                case API_MODE_SET_EFFECT:                   // если заголовок а
                    modes[(*effectIndex)].effect = static_cast<effects>(body);
                    break;
                case API_MODE_SET_COLOR_A:                   // если заголовок а
                    modes[(*effectIndex)].colorA = body;
                    break;
                case API_MODE_SET_COLOR_B:                   // если заголовок а
                    modes[(*effectIndex)].colorB = body;
                    break;
                case API_MODE_SET_SPEED:                   // если заголовок а
                    modes[(*effectIndex)].speed = body;
                    break;
                case API_MODE_SET_POWER:                   // если заголовок а
                    modes[(*effectIndex)].power = body;
                    break;
                case API_MODE_SET_ON_OFF:                   // если заголовок а
                    (*isTurnedOn) = body;
                    break;
                case API_MODE_SET_BRIGHTNESS:                   // если заголовок а
                    (*brightness) = body;
                    break;
                case API_MODE_INIT:

                    break;
                case API_MODE_READ:
                    byte * res = modes[(*effectIndex)].toByteArray();
                    for(int i = 0; i < effectEntity::FIELDS; i++) {
                        Serial.print(res[i]);
                    }
                    delete res;
                    
                    break;
            }
            //Serial.println(modes[(*effectIndex)].toString());
            //Serial.println("INDEX: " + String((*effectIndex)));
            //EContainer->writeToEEPROM();
            
        }
    }

};