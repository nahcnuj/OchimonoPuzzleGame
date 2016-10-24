#pragma once

#include <string>
#include <d3dx9.h>
#include <windows.h>

namespace Puzzle
{

	class GameWindow
	{
	private:
		int m_width;
		int m_height;
		LPCTSTR m_title;
		HWND m_handle;
		WNDCLASSEX m_wc;
		bool m_isWindowed;

	public:
		GameWindow(HINSTANCE hInst, bool isWindowed);
		GameWindow(HINSTANCE hInst, int width, int height, bool isWindowed);
		~GameWindow();
		bool Show(int nCmdShow);
		static LRESULT CALLBACK Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		void SetTitle(LPCTSTR title);
	};

}
