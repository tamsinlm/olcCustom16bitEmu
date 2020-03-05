#pragma once
#include <map>
#include <string>

#include "olcPixelGameEngine.h"

#include "Bus.h"

class Console : public olc::PixelGameEngine
{
private:
	static Console *me;
	Bus bus;
	float fResidualTime;
	std::map<size_t, uint8_t> keymaps;

	enum CKEYS {
		KX_ESCAPE = 0,
		KX_1, KX_2, KX_3, KX_4, KX_5, KX_6, KX_7, KX_8, KX_9, KX_0,
		KX_TILDE, KX_MINUS, KX_EQUALS, KX_BACKSPACE, KX_TAB,
		KX_SPACE,
		KX_A, KX_B, KX_C, KX_D, KX_E, KX_F, KX_G, KX_H, KX_I, KX_J,
		KX_K, KX_L, KX_M, KX_N, KX_O, KX_P, KX_Q, KX_R, KX_S, KX_T,
		KX_U, KX_V, KX_W, KX_X, KX_Y, KX_Z,
		KX_CAPSLOCK,
		KX_LBRACKET, KX_RBRACKET, KX_BACKSLASH, KX_FORSLASH,
		KX_COLON, KX_QUOTE, KX_COMMA, KX_PERIOD,
		KX_SHIFT, KX_CTRL,
		KX_F1, KX_F2, KX_F3, KX_F4, KX_F5, KX_F6, KX_F7, KX_F8,
		KX_F9, KX_F10, KX_F11, KX_F12,
		KX_INSERT, KX_DELETE, KX_HOME, KX_END, KX_PAGEUP, KX_PAGEDOWN,
		KX_UP, KX_DOWN, KX_LEFT, KX_RIGHT,
		KX_NUMLOCK, KX_ENTER, KX_NUMPERIOD,
		KX_NUMDIV, KX_NUMMUL, KX_NUMSUB, KX_NUMADD,
		KX_NUM0, KX_NUM1, KX_NUM2, KX_NUM3, KX_NUM4,
		KX_NUM5, KX_NUM6, KX_NUM7, KX_NUM8, KX_NUM9,
		KX_PRINT, KX_SCROLL, KX_PAUSE,
	};

public:
	Console();

	static float SoundOut(int nChannel, float fGlobalTime, float fTimeStep);

	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float fElapsedTime);

	int OnUserKeyDown(int key) override;
	int OnUserKeyUp(int key) override;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0x0F];
		return s;
	};
};

