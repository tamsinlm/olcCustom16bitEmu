#pragma once
#include <vector>

class apuLM8snd
{
public:
	static std::vector<int8_t> buffer;
	static uint16_t b_head, b_tail;

private:
	uint8_t active_voice;
#define MAX_VOICES	4
	struct Voice {
		uint8_t note;
		uint8_t state;
		uint8_t time;
		uint8_t attack;
		uint8_t sustain;
		uint8_t decay;
		uint8_t release;

		Voice() {
			note = state = time = 0;
			attack = sustain = decay = release = 0;
		}
	} voices[MAX_VOICES];

public:
	apuLM8snd();
	~apuLM8snd();

	void clock();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	uint8_t in(uint16_t address);
	void out(uint16_t address, uint8_t data);

};

