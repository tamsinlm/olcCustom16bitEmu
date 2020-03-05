#include "Bus.h"

Bus::Bus()
{
	cpu.ConnectBus(this);
	gpu.ConnectBus(this);
	ram = new uint8_t[0x8000];
	dma = false;
	dma_address = 0;
	dma_count = 0;
	kb_head = 0;
	kb_tail = 0;
	bFrameComplete = false;
}

Bus::~Bus()
{
}

void Bus::clock()
{
	//float fClocksTotal = 0;
	if (dma)
	{
		if (dma_count == 0)
			dma = false;
		else
		{
			gpu.out(0x12, ram[dma_address++]);
			dma_count--;
		}
	}
	else
		cpu.clock();
	gpu.clock();
	if (fClocksTotal > 226.757)
	{
		apu.clock();
		fClocksTotal -= 226.757f;
	}
	fClocksTotal += 1;
}

void Bus::SetFrameComplete()
{
	bFrameComplete = true;
	cpu.nmi();
}

bool Bus::IsFrameComplete()
{
	bool fc = bFrameComplete;
	bFrameComplete = false;
	return fc;
}

void Bus::PressKey(uint8_t key)
{
	kb_buffer[kb_head++] = key;
}

uint8_t Bus::read(uint16_t address)
{
	if (address < 0x8000)//read from memory
		return ram[address];
	return uint8_t();//read from cartridge
}

void Bus::write(uint16_t address, uint8_t data)
{
	if (address < 0x8000)//write to memory
		ram[address] = data;
	else {}//write to cartridge?
}

uint8_t Bus::in(uint16_t address)
{
	switch (address >> 8) {
	case 0x00:
		switch (address & 0x0FF) {
		case 0x20:
			return kb_head != kb_tail;
		case 0x21:
			if (kb_head == kb_tail)
				return 0xFF;
			return kb_buffer[kb_tail++];
		}
	case 0x01:
		return gpu.in(address);
	case 0x02:
		return apu.in(address);
	}
	return uint8_t();
}

void Bus::out(uint16_t address, uint8_t data)
{
	switch (address >> 8) {
	case 0x00:	//Motherboard signals
		switch (address & 0x0FF)
		{
		case 0x40://DMA Counter High
			dma_count = (dma_count & 0x00FF) | (data << 8);
			break;
		case 0x41://DMA Counter Low
			dma_count = (dma_count & 0xFF00) | data;
			break;
		case 0x42://DMA Address High
			dma_address = (dma_address & 0x00FF) | (data << 8);
			break;
		case 0x43://DMA Address Low
			dma_address = (dma_address & 0xFF00) | data;
			break;
		case 0x44://DMA Enable
			dma = true;
		}
		break;
	case 0x01:
		gpu.out(address, data);
		break;
	case 0x02:
		apu.out(address, data);
		break;
	}
}

