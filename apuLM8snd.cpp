#include "apuLM8snd.h"

std::vector<int8_t> apuLM8snd::buffer;
uint16_t apuLM8snd::b_head;
uint16_t apuLM8snd::b_tail;

apuLM8snd::apuLM8snd()
{
	buffer.resize(65536);
	b_head = b_tail = 0;

	active_voice = 0;
}

apuLM8snd::~apuLM8snd()
{
}

void apuLM8snd::clock()
{
	buffer[b_head++] = 0;
}

uint8_t apuLM8snd::read(uint16_t address)
{
	return uint8_t();
}

void apuLM8snd::write(uint16_t address, uint8_t data)
{
}

uint8_t apuLM8snd::in(uint16_t address)
{
	return uint8_t();
}

void apuLM8snd::out(uint16_t address, uint8_t data)
{
}
