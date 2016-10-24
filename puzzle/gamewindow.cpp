#include "gamewindow.h"
#include "tchar.h"

namespace Puzzle
{

	GameWindow::GameWindow(HINSTANCE hInst, bool isWindowed) {
		GameWindow(hInst, 640, 480, isWindowed);
	}

	GameWindow::GameWindow(HINSTANCE hInst, int width, int height, bool isWindowed)
		: m_title(_T("Application")), m_width(width), m_height(height), m_isWindowed(isWindowed) {
		ZeroMemory(&m_wc, sizeof(WNDCLASSEX));
		m_wc.cbSize = sizeof(WNDCLASSEX);
		m_wc.style = CS_NOCLOSE | CS_HREDRAW | CS_VREDRAW;
		m_wc.lpfnWndProc = Proc;
		m_wc.hInstance = hInst;
		m_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		m_wc.lpszClassName = _T("GameWindow");

		RegisterClassEx(&m_wc);
		
		RECT rc = {0, 0, m_width, m_height};

		if (m_isWindowed) {
			m_handle = CreateWindowEx(
				NULL,
				m_wc.lpszClassName,
				m_title,
				#ifdef _DEBUG
					WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				#else
					WS_OVERLAPPED | WS_EX_TOPMOST | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				#endif
				CW_USEDEFAULT, CW_USEDEFAULT,
				m_width, m_height,
				NULL,
				NULL,
				hInst,
				nullptr
			);
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		}
		else {
			m_handle = CreateWindowEx(
				NULL,
				m_wc.lpszClassName,
				m_title,
				#ifdef _DEBUG
					WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				#else
					WS_VISIBLE | WS_POPUP | WS_EX_TOPMOST | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				#endif
				CW_USEDEFAULT, CW_USEDEFAULT,
				m_width, m_height,
				NULL,
				NULL,
				hInst,
				nullptr
			);
			AdjustWindowRect(&rc, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, FALSE);
		}

		//ディスプレイの中心に移動
		SetWindowPos(
			m_handle,
			#ifdef _DEBUG
				HWND_TOP,
			#else
				HWND_TOPMOST,
			#endif
			( GetSystemMetrics(SM_CXSCREEN) - (rc.right) ) / 2,
			( GetSystemMetrics(SM_CYSCREEN) - (rc.bottom) ) / 2,
			rc.right,
			rc.bottom,
			SWP_HIDEWINDOW
		);
	}

	GameWindow::~GameWindow() {
	}
	
	//以前から表示されていた:true
	bool GameWindow::Show(int nCmdShow) {
		return ShowWindow(m_handle, nCmdShow) ? true : false;
	}

	LRESULT CALLBACK GameWindow::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_KEYUP:
			if (wParam == VK_ESCAPE) {
				PostQuitMessage(0);
				return 0;
			}
			break;

		case WM_ERASEBKGND:
			return 1;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	HWND GameWindow::GetHandle() {
		return m_handle;
	}

	void GameWindow::SetTitle(LPCTSTR title) {
		m_title = title;
		SetWindowText(m_handle, m_title);
	}

}
