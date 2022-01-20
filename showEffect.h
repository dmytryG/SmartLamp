#pragma once

#include "misc.h"
#include "stripInit.h"

class EffectShower
{
private:
	long int localTimestamp = 0;
	int tics = 0;
	int ticsOnEffect = 100;
	effects lastEffect;
	

	void EffectShower::fill(unsigned char color, unsigned char brightness = 255, unsigned char contrast = 255) {
		for(int i = 0; i < NUMLEDS; i++) {
			strip.set(i, mHSV(color, contrast, brightness));
		}
	}

	void EffectShower::fillRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char brightness = 255) {
		for(int i = 0; i < NUMLEDS; i++) {
			strip.set(i, mRGB(r, b, g));
		}
	}

	void EffectShower::fillPartRGB(int startIndex, int endIndex, unsigned char r, unsigned char g, unsigned char b, unsigned char brightness = 255) {
		for(int i = startIndex; i < endIndex; i++) {
			strip.set(i, mRGB(r, b, g));
		}
	}

	void clean() {
		fill(0,0);
	}


	void EffectShower::rainbow() {
		static byte counter = 0;
		for (int i = 0; i < NUMLEDS; i++) {
			strip.set(i, mWheel8(counter + i * 255 / NUMLEDS));   // counter смещает цвет
		}
		counter += 3;   // counter имеет тип byte и при достижении 255 сбросится в 0
	}

	void EffectShower::rainbowrangeupd(effectEntity& mode) {
		static short int counter = 0;
		static bool direction = false;
		
		fill(map(counter, 0, 255, mode.colorA, mode.colorB));

		if(counter >= 255 || counter <= 0) direction = !direction; 
		if(direction) counter+=3;
		else counter-=3;
	}

	void EffectShower::rainbowrangefreeze(effectEntity& mode) {
		static byte counter = 0;
		for (int i = 0; i < NUMLEDS; i++) {
			strip.set(i, mWheel8(map(i, 0, NUMLEDS, mode.colorA, mode.colorB)));   // counter смещает цвет
		}
		counter += 3;   // counter имеет тип byte и при достижении 255 сбросится в 0
	}

	void EffectShower::rainbowrange(effectEntity& mode) {
		static int counter = 0;

		
		for (int i = 0; i < NUMLEDS; i++) {
			strip.set(i, mHSV(map((i + counter)%(NUMLEDS), 0, NUMLEDS, mode.colorA, mode.colorB), 255, 255));   // counter смещает цвет
			//strip.set(NUMLEDS - i, mHSV(map((i + counter)%(NUMLEDS/2), 0, NUMLEDS/2, mode.colorA, mode.colorB), 255, 255));
		}
		
		/*
		for(int i = 0; i < NUMLEDS*2; i++) {
			strip.set(i%NUMLEDS, mHSV(map((i + counter)%(NUMLEDS*2), 0, NUMLEDS, mode.colorA, mode.colorB), 255, 255));
		}*/
		counter += 1;   // counter имеет тип byte и при достижении 255 сбросится в 0
	}

	void EffectShower::starfade(effectEntity& mode) {
		static byte counter = 0;
		// перемотка буфера со сдвигом (иллюзия движения пикселей)
		for (int i = 0; i < NUMLEDS - 1; i++) strip.leds[i] = strip.leds[i + 1];

		// каждый третий вызов - последний пиксель красным, иначе чёрным
		if (random(0, mode.power) == 0) {
			int traceLenght = random(2, 7);
			unsigned char color = random(mode.colorA, mode.colorB);

			strip.set(NUMLEDS - 2 - traceLenght, mHSV(color, 255, 255));
			strip.set(NUMLEDS - 1 - traceLenght, mHSV(color, 255, 0));

			for(int i = NUMLEDS - 2 - traceLenght; i < NUMLEDS - 2; i++) {
				strip.set(i, mHSV(color, 255, map(i,NUMLEDS - 2,NUMLEDS - 2 - traceLenght ,0, 255)));
			}
		} 
		counter++;
	}

	void EffectShower::strobe(effectEntity& mode) {
		static bool counter = false;
		if(counter) {
			fill(mode.colorA); 
		}
		else {
			fill(mode.colorB); 
		}
		counter = !counter;
	}
	void EffectShower::strobeWhite(effectEntity& mode) {
		static bool counter = false;
		if(counter) {
			fillRGB(255, 255, 255); 
		}
		else {
			fillRGB(0, 0, 0); 
		}
		counter = !counter;
	}

	void EffectShower::neon(effectEntity& mode) {
		if (random(0, mode.power) == 0) {
			fill(mode.colorA, random(170, 230));
		}
		else {
			fill(mode.colorA, 255);
		}
	}

	void EffectShower::rainRangeNew(effectEntity& mode) { //EFFECT_RAINRANGENEW
		static byte shift = 0;

		for (int i = 0; i < NUMLEDS/2; i++) {
			strip.set((i+shift)%NUMLEDS, mHSV(map(i, 0, NUMLEDS/2, mode.colorA, mode.colorB), 255, 255));   // counter смещает цвет
			strip.set((i+NUMLEDS/2+shift)%NUMLEDS, mHSV(map(i, 0, NUMLEDS/2, mode.colorB, mode.colorA), 255, 255));   // counter смещает цвет
		}

		if(++shift >= NUMLEDS) shift = 0;

		for(int i = 0; i < NUMLEDS; i++) {
			//swap(strip.getLnk(i), strip.getLnk((i+shift)%NUMLEDS));
			//swap(strip.leds[i], strip.leds[(i+shift)%NUMLEDS]);
			//strip.set((i+shift)%NUMLEDS, strip.get(i%NUMLEDS));
		}

		for(int i = 0 ; i < NUMLEDS; i++ ) {
			//strip.set((i+1)%NUMLEDS, strip.get(i%NUMLEDS));
		}
	}

	void swap(mData &a, mData &b) {
		mData &tmp = a;
		b = a;
		b = tmp;
	}

	void EffectShower::pride(){
		int ledsOnPart = NUMLEDS/6;
		for(int i = 0; i < 6; i++) {
			if(i == 0) fillPartRGB(i * ledsOnPart, (i+1) * ledsOnPart, 255, 0, 24);
			if(i == 1) fillPartRGB(i * ledsOnPart, (i+1) * ledsOnPart, 255, 165, 44);
			if(i == 2) fillPartRGB(i * ledsOnPart, (i+1) * ledsOnPart, 255, 255, 65);
			if(i == 3) fillPartRGB(i * ledsOnPart, (i+1) * ledsOnPart, 0, 128, 24);
			if(i == 4) fillPartRGB(i * ledsOnPart, (i+1) * ledsOnPart, 0, 0, 249);
			if(i == 5) fillPartRGB(i * ledsOnPart, (i+1) * ledsOnPart, 134, 0, 125);
		}
	}

	void EffectShower::drawOverlay(){
		fillPartRGB(0, 2, 255, 255, 255);
	}

public:
	bool lightsOverlay = false;


	EffectShower::EffectShower() {
		localTimestamp = millis();
	}
	EffectShower::~EffectShower();


	void EffectShower::setEffect(effectEntity& mode) { //Loop
		//Serial.print("<setEffect> ");
		//Serial.println(mode.effect);
		if(tics % mode.speed == 0) {
			if(mode.effect == EFFECT_RAINBOW) {
				this->rainbow();
			}
			else if(mode.effect == EFFECT_STARFADE) {
				this->starfade(mode);
			}
			else if(mode.effect == EFFECT_NEON) {
				this->neon(mode);
			}
			else if(mode.effect == EFFECT_RAINBOWRANGE) {
				this->rainbowrange(mode);
			}
			else if(mode.effect == EFFECT_RAINBOWRANGEFREEZE) {
				this->rainbowrange(mode);
			}
			else if(mode.effect == EFFECT_STANDALONE) {
				this->fill(mode.colorA);
			}
			else if(mode.effect == EFFECT_TURNEDOFF) {
				this->fill(255, 0);
			}
			else if(mode.effect == EFFECT_STROBE) {
				this->strobe(mode);
			}
			else if(mode.effect == EFFECT_STROBEWHITE) {
				this->strobeWhite(mode);
			}
			else if(mode.effect == EFFECT_PRIDE) {
				this->pride();
			}
			else if(mode.effect == EFFECT_RAINBOWRANGE_UPD) {
				this->rainbowrangeupd(mode);
			}
			else if(mode.effect == EFFECT_RAINRANGENEW) {
				this->rainRangeNew(mode);
			}
		}
		tics++;
		if(lastEffect != mode.effect) {
			clean();
		}

		if(lightsOverlay) {
			drawOverlay();
		}
		
		strip.show();
		delay(1);


		lastEffect = mode.effect;
	}	
	void dropInits() {
		
	}
};


