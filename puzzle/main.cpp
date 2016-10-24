#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <clocale>
#include <cstdlib>
#include <ctime>
#include "scenemgr.h"
#include "gamewindow.h"
#include "fpscontrol.h"

const WCHAR* APP_TITLE = L"落ち物パズルゲーム";
const WCHAR* APP_VERSION = L"1.1";
const int WIDTH = 640;
const int HEIGHT = 480;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	srand(time(NULL));
	setlocale(LC_ALL, "japanese");	//char* を WCHAR* に変換するのに使う

	//ウィンドウタイトル
	WCHAR wndtitle[100];
	_stprintf_s(wndtitle, L"%s (Version %s)", APP_TITLE, APP_VERSION);

	Puzzle::GameWindow *window = new Puzzle::GameWindow(hInst, WIDTH, HEIGHT, true);
	window->SetTitle(wndtitle);

	Puzzle::Direct3DRendererBase *renderer = new Puzzle::Direct3DRendererBase(true);
	renderer->Resize(window->GetHandle(), WIDTH, HEIGHT);

	window->Show(nCmdShow);

	Puzzle::SceneMgr *sceneMgr = new Puzzle::SceneMgr(window->GetHandle());

	Puzzle::FPSControl *fpsControl = new Puzzle::FPSControl(60);

	MSG msg;

	while (true) {
		if ( PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) ) {
			if ( GetMessage(&msg, nullptr, 0, 0) == 0 ) return msg.wParam;	//WM_QUIT

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			sceneMgr->DoFrame(renderer->GetDirect3DDevice());
			
			fpsControl->TimeControl();

			if ( fpsControl->GetDrawFlag() ) {
				sceneMgr->Draw(renderer);
				
				_stprintf_s(wndtitle, L"%s(Version %s)", APP_TITLE, APP_VERSION, fpsControl->GetFPS());
				window->SetTitle(wndtitle);
			}
		}
	}

	return msg.wParam;
}
