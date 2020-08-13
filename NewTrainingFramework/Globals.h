#pragma once

class Globals
{
public:
	static const int screenWidth = 960;
	static const int screenHeight = 720;

	static enum Key
	{
		//nums 0-9    48-57
		//numpad 0-9  96-105
		
		KEY_UP = 38,
		KEY_DOWN = 40,
		KEY_LEFT = 37,
		KEY_RIGHT = 39,
		KEY_A = 65,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_NUMPAD_0 = 96,
		KEY_NUMPAD_1,
		KEY_NUMPAD_2,
		KEY_NUMPAD_3,
		KEY_NUMPAD_4,
		_KEY_W  = 1,
		_KEY_S,
		_KEY_A,
		_KEY_D,
		_KEY_UP,
		_KEY_DOWN,
		_KEY_LEFT,
		_KEY_RIGHT,
		_KEY_NUMPAD_0,
		_KEY_NUMPAD_1,
		_KEY_NUMPAD_2,
		_KEY_NUMPAD_3,
		_KEY_NUMPAD_4,
	};
};