#pragma once

#include <windows.h>
#include <list>
#include "rendererbase.h"
#include "lostresource.h"
#include "directinput.h"

namespace Puzzle
{

	enum SCENE {
		UNKNOWN,
		TITLE,
		GAME
	};

	enum SC_DESTROY_FLAG {
		DESTROY,		//���̃V�[����j��
		ALL_DESTROY,	//���ׂẴV�[����j��
		STACKED,		//�V�[�����X�^�b�N����
	};

	class SceneBase
	{
	protected:
		bool m_endScene;
		SC_DESTROY_FLAG m_destroyFlag;
		SCENE m_nextScene;
		//void *m_nextSceneArg;	//init�֐��ɓn������
		std::list<LostResource *> m_lostResource;
		RECT m_size;

	public:
		SceneBase();
		SceneBase(RECT rc);
		bool IsEndScene();
		SC_DESTROY_FLAG GetDestroyFlag();
		SCENE GetNextScene();
		//void *GetNextSceneArg();

		virtual void DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput) = 0;
		virtual void Draw(Direct3DRendererBase *pRenderer) = 0;
		void RecoverResource(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS &d3dPresentParam);
	};
	
}
