#pragma once
#include <vector>
#include <fstream>
#include "basescene.h"
#include "field.h"
#include "block.h"
#include "directinput.h"
#include "lostresource.h"

namespace Puzzle
{

	class GameScene : public SceneBase
	{
		static const int START_ANIME_FRAME = 1*60;

		Field* m_field;
		Block* m_nowBlocks[2];
		Block* m_nextBlocks[2];
		int m_score;
		int m_nowBlockPos;	//0Ç©ÇÁèáÇ…â∫ç∂è„âE
		static const int DX[4], DY[4];
		std::vector<unsigned int> m_ranking;

		enum STATE {
			HOWTO,
			PLAY,
			PAUSE,
			RESULT,
		} m_nowState;

		Com_ptr<IDirect3DTexture9> m_backgroundTexture;
		Com_ptr<IDirect3DTexture9> m_howtoTexture;
		Com_ptr<IDirect3DTexture9> m_gamebackTexture;
		Com_ptr<IDirect3DTexture9> m_resultbackTexture;
		Com_ptr<IDirect3DTexture9> m_numberTexture;
		Com_ptr<IDirect3DTexture9> m_blockTexture;
		LPD3DXSPRITE m_pBackgroundSprite;
		LPD3DXSPRITE m_pScoreSprite;
		LPD3DXSPRITE m_pFieldSprite;
		LPD3DXSPRITE m_pNextSprite;
		D3DXVECTOR3 m_backgroundTextureCenter;
		D3DXVECTOR3 m_howtoTextureCenter;
		D3DXVECTOR3 m_gamebackTextureCenter;
		RECT m_backgroundTextureSize;
		RECT m_size;
		int m_frameCount;

		void DropBlocks();
		void CreateNextBlocks();
		void ClearBlock();
		void LoadRanking();
		void SaveRanking();

	public:
		GameScene(RECT rc);
		bool IsEndScene();
		
		void DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput);
		void Draw(Direct3DRendererBase *pRenderer);
	};

}
