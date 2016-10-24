#pragma once
#include <cstdlib>
#include <list>
#include "basescene.h"
#include "directinput.h"
#include "lostresource.h"

namespace Puzzle
{

	class TitleScene : public SceneBase
	{
	private:
		static const int ANIMATION_FRAME = 60;

		enum STATE {
			START_ANIME,	//ãNìÆíºå„
			WAIT_KEY,		//ë“ã@èÛë‘
			END_ANIME		//ëJà⁄íÜ
		} m_nowState;
		Com_ptr<IDirect3DTexture9> m_backgroundTexture;
		Com_ptr<IDirect3DTexture9> m_titleTexture;
		LPD3DXSPRITE m_pBackgroundSprite;
		D3DXVECTOR3 m_backgroundTextureCenter;
		D3DXVECTOR3 m_titleTextureCenter;

		RECT m_backgroundTextureSize;
		RECT m_size;
		int m_frameCount;

	public:
		TitleScene(RECT rc);
		bool IsEndScene();
		
		void DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput);
		void Draw(Direct3DRendererBase *pRenderer);
	};

}
