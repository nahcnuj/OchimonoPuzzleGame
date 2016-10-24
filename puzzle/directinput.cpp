#include "directinput.h"
#include <tchar.h>
namespace Puzzle
{

	DirectInput::DirectInput() {
		m_pDInput = nullptr;
		m_pKeyDevice = nullptr;
		m_pMouseDevice = nullptr;

		ZeroMemory(&m_keyInput, sizeof(BYTE)*INPUT_BUFFER_SIZE);
		ZeroMemory(&m_keyInputPrev, sizeof(BYTE)*INPUT_BUFFER_SIZE);
		ZeroMemory(&m_mouseInput, sizeof(DIMOUSESTATE2));
		ZeroMemory(&m_mouseInputPrev, sizeof(DIMOUSESTATE2));
	}

	DirectInput::~DirectInput() {
		if (m_pKeyDevice) {
			m_pKeyDevice->Unacquire();
			m_pKeyDevice->Release();
			m_pKeyDevice = nullptr;
		}

		if (m_pMouseDevice) {
			m_pMouseDevice->Unacquire();
			m_pMouseDevice->Release();
			m_pMouseDevice = nullptr;
		}

		if (m_pDInput) {
			m_pDInput->Release();
			m_pDInput = nullptr;
		}
	}

	HRESULT DirectInput::Init(HWND hWnd) {
		m_hWnd = hWnd;

		if ( FAILED( DirectInput8Create( GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, nullptr ) ) )
			return E_FAIL;

		if ( FAILED( InitKey() ) )
			return E_FAIL;

		return S_OK;
	}

	HRESULT DirectInput::InitKey() {
		if ( FAILED( m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, nullptr) ) )
			return E_FAIL;

		if ( FAILED( m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard) ) )
			return E_FAIL;

		if ( FAILED( m_pKeyDevice->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) ) )
			return E_FAIL;

		m_pKeyDevice->Acquire();

		return S_OK;
	}

	HRESULT DirectInput::Update() {
		HRESULT hResult = m_pKeyDevice->Acquire();

		if ( hResult == DI_OK || hResult == S_FALSE ) {
			memcpy(m_keyInputPrev, m_keyInput, sizeof(BYTE)*INPUT_BUFFER_SIZE);	

			BYTE nowDeviceState[INPUT_BUFFER_SIZE];
			m_pKeyDevice->GetDeviceState(sizeof(m_keyInput), &nowDeviceState);
			for (int i = 0; i < INPUT_BUFFER_SIZE; ++i) {
				if (nowDeviceState[i] & 0x80) {
					if (m_keyInput[i] < 255) ++m_keyInput[i];
					else m_keyInput[i] = 255;
				}
				else {
					m_keyInput[i] = 0;
				}
			}
		}

		return S_OK;
	}

	bool DirectInput::IsPressKey(int keyCode) {
		return m_keyInput[keyCode] > 0;
	}

	bool DirectInput::IsTriggerKey(int keyCode) {
		return m_keyInputPrev[keyCode] == 0 && m_keyInput[keyCode] > 0;
	}

	bool DirectInput::IsReleaseKey(int keyCode) {
		return m_keyInputPrev[keyCode] > 0 && m_keyInput[keyCode] == 0;
	}

	BYTE DirectInput::GetPushKeyTime(int keyCode) {
		return m_keyInput[keyCode];
	}

}
