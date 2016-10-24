#include "basescene.h"

namespace Puzzle
{

	SceneBase::SceneBase()
		: m_endScene(false), m_destroyFlag(STACKED), m_nextScene(UNKNOWN) {
	}

	SceneBase::SceneBase(RECT rc)
		: m_endScene(false), m_destroyFlag(STACKED), m_nextScene(UNKNOWN), m_size(rc) {
	}
	
	bool SceneBase::IsEndScene() {
		return m_endScene;
	}

	SC_DESTROY_FLAG SceneBase::GetDestroyFlag() {
		return m_destroyFlag;
	}

	SCENE SceneBase::GetNextScene() {
		return m_nextScene;
	}
	
	void SceneBase::RecoverResource(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS &d3dPresentParam) {
		for (auto itr = m_lostResource.begin(); itr != m_lostResource.end(); ++itr)
			(*itr)->Backup();

		if (pDevice->TestCooperativeLevel() != D3DERR_DEVICENOTRESET)
			return;

		if (pDevice->Reset(&d3dPresentParam) != D3D_OK)
			PostQuitMessage(0);

		for (auto itr = m_lostResource.begin(); itr != m_lostResource.end(); ++itr)
			(*itr)->Recover(pDevice);
	}

}
