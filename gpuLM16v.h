#pragma once
#include <cstdint>
#include <vector>

#include "olcPixelGameEngine.h"

class Bus;

class gpuLM16v
{
private:
	Bus* bus;
	int x, y;
	union {
		struct {
			uint8_t Active : 1;
			uint8_t Blanking : 1;
		};
		uint8_t status;
	} status;
	uint16_t v_address, scanline;
	uint8_t palidx, v_mode;
	std::vector<uint8_t> vram;
	std::vector<uint8_t> chram;
	olc::Pixel pal[16];
	olc::Sprite sprScreenBuffer = olc::Sprite(200, 150);

public:
	gpuLM16v();
	~gpuLM16v();

	olc::Sprite& GetScreen();
	bool IsActive();

	void ConnectBus(Bus* b);
	void clock();

	uint8_t in(uint16_t address);
	void out(uint16_t address, uint8_t data);

	void SetMode(uint8_t mode);
};

