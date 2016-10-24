#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Puzzle
{

	class DirectInput
	{
	private:
		static const int INPUT_BUFFER_SIZE = 256;

		HWND m_hWnd;

		LPDIRECTINPUT8 m_pDInput;
		
		LPDIRECTINPUTDEVICE8 m_pKeyDevice;
		BYTE m_keyInput[INPUT_BUFFER_SIZE];
		BYTE m_keyInputPrev[INPUT_BUFFER_SIZE];
		
		LPDIRECTINPUTDEVICE8 m_pMouseDevice;
		DIMOUSESTATE2 m_mouseInput;
		DIMOUSESTATE2 m_mouseInputPrev;

		HRESULT InitKey();

	public:
		DirectInput();
		~DirectInput();

		HRESULT Init(HWND hWnd);
		HRESULT Update();

		bool IsPressKey(int keyCode);
		bool IsTriggerKey(int keyCode);
		bool IsReleaseKey(int keyCode);
		BYTE GetPushKeyTime(int keyCode);
	};

}
