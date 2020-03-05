#define OLC_PGE_APPLICATION
#include "Console.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

Console *Console::me;

Console::Console()
{
	me = this;
	fResidualTime = 0;
}

float Console::SoundOut(int nChannel, float fGlobalTime, float fTimeStep)
{
	if (fGlobalTime > 0.1 && me->bus.apu.b_head != me->bus.apu.b_tail)
		return me->bus.apu.buffer[me->bus.apu.b_tail++] * 0.003f;
	return sin(fGlobalTime * 440 * 2 * 3.141592f) > 0 ? 0.3f : 0;
}

bool Console::OnUserCreate()
{
	keymaps[VK_ESCAPE] = CKEYS::KX_ESCAPE;
	keymaps[VK_SPACE] = CKEYS::KX_SPACE;
	// Top Number Keys
	keymaps[0x30] = CKEYS::KX_0;
	keymaps[0x31] = CKEYS::KX_1;	keymaps[0x32] = CKEYS::KX_2;	keymaps[0x33] = CKEYS::KX_3;
	keymaps[0x34] = CKEYS::KX_4;	keymaps[0x35] = CKEYS::KX_5;	keymaps[0x36] = CKEYS::KX_6;
	keymaps[0x37] = CKEYS::KX_7;	keymaps[0x38] = CKEYS::KX_8;	keymaps[0x39] = CKEYS::KX_9;
	// Letter Keys
	keymaps[0x41] = CKEYS::KX_A;	keymaps[0x42] = CKEYS::KX_B;	keymaps[0x43] = CKEYS::KX_C;	keymaps[0x44] = CKEYS::KX_D;
	keymaps[0x45] = CKEYS::KX_E;	keymaps[0x46] = CKEYS::KX_F;	keymaps[0x47] = CKEYS::KX_G;	keymaps[0x48] = CKEYS::KX_H;
	keymaps[0x49] = CKEYS::KX_I;	keymaps[0x4A] = CKEYS::KX_J;	keymaps[0x4B] = CKEYS::KX_K;	keymaps[0x4C] = CKEYS::KX_L;
	keymaps[0x4D] = CKEYS::KX_M;	keymaps[0x4E] = CKEYS::KX_N;	keymaps[0x4F] = CKEYS::KX_O;	keymaps[0x50] = CKEYS::KX_P;
	keymaps[0x51] = CKEYS::KX_Q;	keymaps[0x52] = CKEYS::KX_R;	keymaps[0x53] = CKEYS::KX_S;	keymaps[0x54] = CKEYS::KX_T;
	keymaps[0x55] = CKEYS::KX_U;	keymaps[0x56] = CKEYS::KX_V;	keymaps[0x57] = CKEYS::KX_W;	keymaps[0x58] = CKEYS::KX_X;
	keymaps[0x59] = CKEYS::KX_Y;	keymaps[0x5A] = CKEYS::KX_Z;
	// General Keys
	keymaps[VK_OEM_3] = CKEYS::KX_TILDE;
	keymaps[VK_OEM_MINUS] = CKEYS::KX_MINUS;	keymaps[VK_OEM_PLUS] = CKEYS::KX_EQUALS;	keymaps[VK_BACK] = CKEYS::KX_BACKSPACE;
	keymaps[VK_TAB] = CKEYS::KX_TAB;			keymaps[VK_OEM_4] = CKEYS::KX_LBRACKET;		keymaps[VK_OEM_6] = CKEYS::KX_RBRACKET;
	keymaps[VK_OEM_1] = CKEYS::KX_COLON;		keymaps[VK_OEM_7] = CKEYS::KX_QUOTE;		keymaps[VK_OEM_5] = CKEYS::KX_BACKSLASH;
	keymaps[VK_OEM_COMMA] = CKEYS::KX_COMMA;	keymaps[VK_OEM_PERIOD] = CKEYS::KX_PERIOD;	keymaps[VK_OEM_2] = CKEYS::KX_FORSLASH;
	keymaps[VK_CAPITAL] = CKEYS::KX_CAPSLOCK;
	keymaps[VK_SHIFT] = CKEYS::KX_SHIFT;		keymaps[VK_CONTROL] = CKEYS::KX_CTRL;
	// Function Keys
	keymaps[VK_F1] = CKEYS::KX_F1;	keymaps[VK_F2] = CKEYS::KX_F2;	keymaps[VK_F3] = CKEYS::KX_F3;	keymaps[VK_F4] = CKEYS::KX_F4;
	keymaps[VK_F5] = CKEYS::KX_F5;	keymaps[VK_F6] = CKEYS::KX_F6;	keymaps[VK_F7] = CKEYS::KX_F7;	keymaps[VK_F8] = CKEYS::KX_F8;
	keymaps[VK_F9] = CKEYS::KX_F9;	keymaps[VK_F10] = CKEYS::KX_F10;keymaps[VK_F11] = CKEYS::KX_F11;keymaps[VK_F12] = CKEYS::KX_F12;
	// Navigation Keys
	keymaps[VK_INSERT] = CKEYS::KX_INSERT;	keymaps[VK_HOME] = CKEYS::KX_HOME;	keymaps[VK_PRIOR] = CKEYS::KX_PAGEUP;
	keymaps[VK_DELETE] = CKEYS::KX_DELETE;	keymaps[VK_END] = CKEYS::KX_END;	keymaps[VK_NEXT] = CKEYS::KX_PAGEDOWN;
	// Arrow Keys
	keymaps[VK_UP] = CKEYS::KX_UP;		keymaps[VK_DOWN] = CKEYS::KX_DOWN;
	keymaps[VK_LEFT] = CKEYS::KX_LEFT;	keymaps[VK_RIGHT] = CKEYS::KX_RIGHT;
	// Number Pad Keys
	keymaps[VK_NUMLOCK] = CKEYS::KX_NUMLOCK;
	keymaps[VK_DIVIDE] = CKEYS::KX_NUMDIV;	keymaps[VK_MULTIPLY] = CKEYS::KX_NUMMUL;
	keymaps[VK_SUBTRACT] = CKEYS::KX_NUMSUB;keymaps[VK_ADD] = CKEYS::KX_NUMADD;
	keymaps[VK_NUMPAD7] = CKEYS::KX_NUM7;	keymaps[VK_NUMPAD8] = CKEYS::KX_NUM8;	keymaps[VK_NUMPAD9] = CKEYS::KX_NUM9;
	keymaps[VK_NUMPAD4] = CKEYS::KX_NUM4;	keymaps[VK_NUMPAD5] = CKEYS::KX_NUM5;	keymaps[VK_NUMPAD6] = CKEYS::KX_NUM6;
	keymaps[VK_NUMPAD1] = CKEYS::KX_NUM1;	keymaps[VK_NUMPAD2] = CKEYS::KX_NUM2;	keymaps[VK_NUMPAD3] = CKEYS::KX_NUM3;
	keymaps[VK_NUMPAD0] = CKEYS::KX_NUM0;	keymaps[VK_DECIMAL] = CKEYS::KX_NUMPERIOD;
	keymaps[VK_RETURN] = CKEYS::KX_ENTER;
	keymaps[VK_PRINT] = CKEYS::KX_PRINT;	keymaps[VK_SCROLL] = CKEYS::KX_SCROLL;	keymaps[VK_PAUSE] = CKEYS::KX_PAUSE;
	olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
	olc::SOUND::SetUserSynthFunction(SoundOut);
	return true;
}

bool Console::OnUserDestroy()
{
	olc::SOUND::DestroyAudio();
	return true;
}

bool Console::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::DARK_BLUE);
	if (bus.gpu.IsActive())
		DrawSprite(0, 0, &bus.gpu.GetScreen(), 1);
	if (fResidualTime <= 0)
	{
		fResidualTime += (1 / 60.0f) - fElapsedTime;
		while (!bus.IsFrameComplete()) { bus.clock(); }
	}
	else
		fResidualTime -= fElapsedTime;
	DrawString(800, 20, "pc: " + hex(bus.cpu.pc, 4), olc::WHITE);
	DrawString(800, 30, "sp: " + hex(bus.cpu.sp, 4), olc::WHITE);
	DrawString(800, 40, "ra: " + hex(bus.cpu.ra.r, 4), olc::WHITE);
	DrawString(800, 50, "rb: " + hex(bus.cpu.rb.r, 4), olc::WHITE);
	DrawString(800, 60, "rc: " + hex(bus.cpu.rc.r, 4), olc::WHITE);
	DrawString(800, 70, "rd: " + hex(bus.cpu.rd.r, 4), olc::WHITE);
	DrawString(880, 40, "opc: " + hex(bus.cpu.opcode, 2) + " : " + bus.cpu.mnemonic, olc::WHITE);
	DrawString(880, 50, "aux: " + hex(bus.cpu.aux, 2), olc::WHITE);
	DrawString(880, 60, "add: " + hex(bus.cpu.address, 4), olc::WHITE);
	DrawString(880, 70, "cyc: " + hex(bus.cpu.cycle, 4), olc::WHITE);
	DrawString(800, 80, "Z", bus.cpu.flags.Zero ? olc::GREEN : olc::RED);
	DrawString(810, 80, "S", bus.cpu.flags.Sign ? olc::GREEN : olc::RED);
	DrawString(820, 80, "O", bus.cpu.flags.Overflow ? olc::GREEN : olc::RED);
	DrawString(830, 80, "I", bus.cpu.flags.Interrupt ? olc::GREEN : olc::RED);
	for (int y=0;y<16;y++)
		for (int x = 0; x < 16; x++)
		{
			uint16_t m = (y << 4) + x;
			DrawString(800 + x * 20, 100 + y * 10, hex(bus.ram[m], 2), m == bus.cpu.pc ? olc::CYAN : olc::WHITE);
			//DrawString(800 + x * 20, 300 + y * 10, hex(bus.kb_buffer[m], 2), m == bus.kb_head ? olc::RED : olc::WHITE);
			DrawString(800 + x * 20, 300 + y * 10, hex(bus.apu.buffer[m], 2), m == bus.apu.b_head ? olc::GREEN : olc::WHITE);
		}
	//DrawString(800,500,);
	return true;
}

int Console::OnUserKeyDown(int key)
{
	if (keymaps.find(key) == keymaps.end())
		bus.PressKey(0x7F);
	else
		bus.PressKey(keymaps[key]);
	return 0;
}

int Console::OnUserKeyUp(int key)
{
	if (keymaps.find(key) == keymaps.end())
		bus.PressKey(0xFF);
	else
		bus.PressKey(keymaps[key] | 0x80);
	return 0;
}

Console console;

int main()
{
	if (console.Construct(1200, 600, 1, 1))
		console.Start();
	return 0;
}
