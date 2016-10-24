#pragma once
#include <stack>
#include "basescene.h"
#include "directinput.h"

namespace Puzzle
{

	class SceneMgr
	{
	private:
		std::stack<SceneBase *> m_scene;
		DirectInput m_dInput;
		HWND m_hWnd;
		RECT m_sceneSize;
		
		void SetNextScene(SCENE scene, void* arg = nullptr);

	public:
		SceneMgr(HWND hWnd);
		~SceneMgr();
		bool IsEmpty();

		void DoFrame(IDirect3DDevice9 *pDevice);
		void Draw(Direct3DRendererBase *pRenderer);
		void RecoverResources(const IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS &d3dPresentParam);
	};

}
