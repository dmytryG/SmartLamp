#pragma once

#include <Arduino.h>
#define STRIP_PIN 11     // пин ленты
#define NUMLEDS 61      // кол-во светодиодов

#define COLOR_DEBTH 3
#include "microLED.h"   // подключаем библу

microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;
