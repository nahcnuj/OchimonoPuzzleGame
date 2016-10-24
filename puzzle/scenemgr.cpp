#include <tchar.h>
#include "scenemgr.h"
#include "title.h"
#include "game.h"

namespace Puzzle
{
	SceneMgr::SceneMgr(HWND hWnd) : m_hWnd(hWnd) {
		m_dInput.Init(m_hWnd);
		GetClientRect(m_hWnd, &m_sceneSize);

		SetNextScene(TITLE, nullptr);
	}

	SceneMgr::~SceneMgr() {
		while (!m_scene.empty()) {
			delete m_scene.top();
			m_scene.pop();
		}
	}

	void SceneMgr::SetNextScene(SCENE scene, void *arg) {
		SceneBase *nextInst = nullptr;
		
		switch (scene) {
		case TITLE:
			m_scene.push(new TitleScene(m_sceneSize));
			break;
		case GAME:
			m_scene.push(new GameScene(m_sceneSize));
			break;
		default:
			MessageBox(m_hWnd, _T("不正なシーンです。"), _T("Error"), MB_OK | MB_ICONERROR);
			std::abort();
			break;
		}
	}
	
	void SceneMgr::DoFrame(IDirect3DDevice9 *pDevice) {
		if (m_scene.empty()) return;

		m_dInput.Update();
		m_scene.top()->DoFrame(pDevice, &m_dInput);

		if (m_scene.top()->IsEndScene()) {
			SCENE nextScene = m_scene.top()->GetNextScene();

			switch (m_scene.top()->GetDestroyFlag()) {
			case DESTROY:
				delete m_scene.top();
				m_scene.pop();
				break;
			case ALL_DESTROY:
				while (!m_scene.empty()) {
					delete m_scene.top();
					m_scene.pop();
				}
				break;
			case STACKED:
				break;
			default:
				MessageBox(m_hWnd, _T("不正なシーンです。"), _T("Error"), MB_OK | MB_ICONERROR);
				std::abort();
				break;
			}

			SetNextScene(nextScene);
		}
	}

	void SceneMgr::Draw(Direct3DRendererBase *pRenderer) {
		if (m_scene.empty()) return;

		if (pRenderer->BeginScene() == S_OK) {
			m_scene.top()->Draw(pRenderer);
			pRenderer->EndScene();
			if ( pRenderer->Present(nullptr, nullptr, NULL, nullptr) == D3DERR_DEVICELOST) {
				m_scene.top()->RecoverResource(pRenderer->GetDirect3DDevice(), pRenderer->GetPresentParam());
			}
		}
	}

}
