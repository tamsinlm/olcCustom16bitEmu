#include "gpuLM16v.h"
#include "Bus.h"

#define VRAM_SIZE	0x8000
#define CHRAM_SIZE	0xA00

gpuLM16v::gpuLM16v()
{
	bus = NULL;
	x = y = 0;
	v_address = 0;
	v_mode = 0;
	status.status = 0;
	palidx = 0;
	scanline = 0;
	vram.resize(VRAM_SIZE);
	chram.resize(CHRAM_SIZE);
	pal[0] = olc::Pixel(0, 0, 0);		pal[1] = olc::Pixel(122, 0, 0);		pal[2] = olc::Pixel(0, 122, 0);		pal[3] = olc::Pixel(0, 0, 122);
	pal[4] = olc::Pixel(122, 122, 0);	pal[5] = olc::Pixel(122, 0, 122);	pal[6] = olc::Pixel(0, 122, 122);	pal[7] = olc::Pixel(170, 170, 170);
	pal[8] = olc::Pixel(85, 85, 85);	pal[9] = olc::Pixel(255, 0, 0);		pal[10] = olc::Pixel(0, 255, 0);	pal[11] = olc::Pixel(0, 0, 255);
	pal[12] = olc::Pixel(255, 255, 0);	pal[13] = olc::Pixel(255, 0, 255);	pal[14] = olc::Pixel(0, 255, 255);	pal[15] = olc::Pixel(255, 255, 255);
	SetMode(0);
	status.Active = 1;
}

gpuLM16v::~gpuLM16v()
{
}

olc::Sprite& gpuLM16v::GetScreen()
{
	return sprScreenBuffer;
}

bool gpuLM16v::IsActive()
{
	return status.Active;
}

void gpuLM16v::ConnectBus(Bus* b)
{
	bus = b;
}

void gpuLM16v::clock()
{
	int m, c;
	x++;
	if (x < 200 && y < 150)
	{
		switch (v_mode) {
		case 0:
			m = ((y/10) * 25) + (x >> 3);
			c = vram[m] * 10;
			break;
		case 1:
			m = (y * 100) + (x >> 1);
			sprScreenBuffer.SetPixel(x, y, pal[vram[m] >> ((x & 1) << 4)]);
			break;
		}
	}
	if (x >= 264)
	{
		x = 0;
		y++;
		if (y == 602)
		{
			bus->SetFrameComplete();
			status.Active = 1;
		}
		if (y >= 628)
			y = 0;
	}
}

uint8_t gpuLM16v::in(uint16_t address)
{
	return uint8_t();
}

void gpuLM16v::out(uint16_t address, uint8_t data)
{
	switch (address & 0x0FF) {
	case 0x10:	//Set Address High
		v_address &= 0x00FF;
		v_address |= data << 8;
		break;
	case 0x11:	//Set Address Low
		v_address &= 0xFF00;
		v_address |= data & 0x0FF;
		break;
	case 0x12:	//Write to VRAM
		vram[v_address & (VRAM_SIZE - 1)] = data;
		v_address++;
		break;
	case 0x20:
		palidx = data;
		break;
	case 0x21:
		uint8_t color = palidx % 3;
		uint8_t idx = palidx / 3;
		switch (color)
		{
		case 0:
			pal[idx].r = data << 3;
			break;
		case 1:
			pal[idx].g = data << 3;
			break;
		case 2:
			pal[idx].b = data << 3;
			break;
		}
	}
}

void gpuLM16v::SetMode(uint8_t mode)
{
	status.Active = 0;
}

