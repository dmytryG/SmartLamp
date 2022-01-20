#pragma once
#include <avr/eeprom.h>

enum effects
{
	EFFECT_RAINBOW,
	EFFECT_STARFADE,
	EFFECT_NEON,
	EFFECT_RAINBOWRANGE,
	EFFECT_RAINBOWRANGEFREEZE,
	EFFECT_STANDALONE,
	EFFECT_STROBE,
	EFFECT_STROBEWHITE,
	EFFECT_TURNEDOFF,
	EFFECT_PRIDE,
	EFFECT_RAINBOWRANGE_UPD,
	EFFECT_RAINRANGENEW,
};

struct effectEntity
{
	effects effect;
	unsigned char colorA;
	unsigned char colorB;	
	unsigned char speed;
	unsigned char power;
	static const byte effectEntity::FIELDS = 5;
	

	void setup(effects effect, unsigned char colorA=0, unsigned char colorB=255, unsigned char speed = 30,unsigned char power = 10) {
		this->effect = effect;
		this->colorA = colorA;
		this->colorB = colorB;
		this->speed = speed;
		this->power = power;
	}

	String toString() {
		String res = "effects " + String((int)effect);
		res+= "\ncolorA " + String(colorA);
		res+= "\ncolorB " + String(colorB);
		res+= "\nspeed " + String(speed);
		res+= "\npower " + String(power);
		res+="\n";
		return res;
	}
	byte* toByteArray() {
		byte * res = new byte[FIELDS];
		res[0] = effect;
		res[1] = colorA;
		res[2] = colorB;
		res[3] = speed;
		res[4] = power;
		return res;
	}
	void normalize() {
		if(speed <= 0) speed = 10;
		if(power <= 0) power = 10;
	}
};

class EffectContainer {

private:
	effectEntity * modes;

public:

	static const int CONTAINER_CAPACITY = 8;

	EffectContainer() {
		modes = new effectEntity[CONTAINER_CAPACITY];
	}

	effectEntity * getEffectEntityContainer() {
		return modes;
	}

	void loadFromEEPROM() {
		eeprom_read_block(modes, 0, sizeof(effectEntity)*CONTAINER_CAPACITY);
		for(int i = 0; i < CONTAINER_CAPACITY; i++) {
			Serial.println("[READ] index: " + String(i) + "\n" + modes[i].toString());
		}
	}

	void normalizeContainer() {
		for(int i = 0; i < CONTAINER_CAPACITY; i++) {
			modes[i].normalize();
		}
	}

	void writeToEEPROM() {
		eeprom_write_block(modes, 0, sizeof(effectEntity)*CONTAINER_CAPACITY);
		for(int i = 0; i < CONTAINER_CAPACITY; i++) {
			
		}
	}

};

