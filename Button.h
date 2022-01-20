#pragma once
#include "Arduino.h"


class Button
{
private:
	unsigned char buttonPin;
	unsigned long int currentTimeClick = 0;
	bool isInverted;

	void ButtonReleased() {
		if(millis() - currentTimeClick > 200 && millis() - currentTimeClick < 1000) {
			isButtonClicked = true;
			isButtonHold = false;
			isButtonDeepHold = false;
		}
		if(millis() - currentTimeClick > 1000 && millis() - currentTimeClick < 5000) {
			isButtonHold = true;
			isButtonClicked = false;
			isButtonDeepHold = false;
		}
		if(millis() - currentTimeClick > 5000 && millis() - currentTimeClick < 10000) {
			isButtonHold = false;
			isButtonClicked = false;
			isButtonDeepHold = true;
		}
  }

	void onButtonProcessed() {
		isButtonClicked = false;
		isButtonHold = false;
		isButtonDeepHold = false;
	}

	bool isButtonClicked = false;
	bool isButtonHold = false;
	bool isButtonDeepHold = false;

public:
	

	void makeInits() {
		currentTimeClick = millis();
	}

	Button(int pin) {
		this->buttonPin = pin;
		pinMode(pin, INPUT);
		makeInits();
	}
	Button(int pin, bool isInverted, bool usePullUp) {
		this->buttonPin = pin;
		if(!usePullUp) {
			pinMode(pin, INPUT);
		}
		else {
			pinMode(pin, INPUT_PULLUP);
		}
		this->isInverted = isInverted;
		makeInits();
	}
	~Button();

	void buttonLoop() {
		if((!isInverted && (digitalRead(buttonPin)==HIGH)) || (isInverted && (digitalRead(buttonPin)==LOW))) {

		}
		else {
			ButtonReleased();
			currentTimeClick = millis();
		}
	}

	boolean getIsButtonClicked() {
		if(isButtonClicked) {
			onButtonProcessed();
			return true;
		}
		else return false;
	}

	boolean getIsButtonHold() {
		if(isButtonHold) {
			onButtonProcessed();
			return true;
		}
		else return false;
	}

	boolean getIsButtonDeepHold() {
		if(isButtonDeepHold) {
			onButtonProcessed();
			return true;
		}
		else return false;
	}
};
