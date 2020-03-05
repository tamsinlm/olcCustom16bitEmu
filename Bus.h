#pragma once
#include <cstdint>

#include "cpuLM16c.h"
#include "gpuLM16v.h"
#include "apuLM8snd.h"

class Bus
{
public:
	float fClocksTotal = 0;
	cpuLM16c cpu;
	gpuLM16v gpu;
	apuLM8snd apu;
	uint8_t *ram;

private:
	bool dma;
	uint16_t dma_address;
	uint16_t dma_count;

	bool bFrameComplete;

public:
	uint8_t kb_buffer[256];
	uint8_t kb_head;
	uint8_t kb_tail;

	Bus();
	~Bus();

	void clock();

	void SetFrameComplete();
	bool IsFrameComplete();

	void PressKey(uint8_t key);

public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	uint8_t in(uint16_t address);
	void out(uint16_t address, uint8_t data);
};

