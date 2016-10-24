#include <d3dx9.h>
#include <tchar.h>
#include <cstdio>
#include "title.h"

namespace Puzzle
{

	TitleScene::TitleScene(RECT rc)
		: m_nowState(START_ANIME),
		  m_backgroundTexture(nullptr),
		  m_pBackgroundSprite(nullptr),
		  m_size(rc),
		  m_frameCount(0) {
	}

	void TitleScene::DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput) {
		++m_frameCount;

		if (m_pBackgroundSprite == nullptr) {
			TCHAR fn[100];

			#ifdef _DEBUG
				_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\titleback.png"));
			#else
				_stprintf_s(fn, _T(".\\resources\\image\\titleback.png"));
			#endif

			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(fn, &info);

			D3DXCreateTextureFromFileEx(pDevice, fn,
				info.Width, info.Height, info.MipLevels,
				0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, nullptr, nullptr, 
				m_backgroundTexture.ToCreator());

		
			LostTexture lostTexture;
			lostTexture.Regist(m_backgroundTexture);
			m_lostResource.push_back(&lostTexture);

			#ifdef _DEBUG
				_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\title.png"));
			#else
				_stprintf_s(fn, _T(".\\resources\\image\\title.png"));
			#endif

			D3DXGetImageInfoFromFile(fn, &info);

			D3DXCreateTextureFromFileEx(pDevice, fn,
				info.Width, info.Height, info.MipLevels,
				0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, nullptr, nullptr, 
				m_titleTexture.ToCreator());
			
			lostTexture.Regist(m_titleTexture);
			m_lostResource.push_back(&lostTexture);

			m_backgroundTextureCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_titleTextureCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
			D3DXCreateSprite(pDevice, &m_pBackgroundSprite);
		}

		switch (m_nowState) {
		case START_ANIME:
			if (m_frameCount >= ANIMATION_FRAME) {
				m_nowState = WAIT_KEY;
				m_frameCount = 0;
			}
			break;

		case WAIT_KEY:
			if (dInput->IsPressKey(DIK_SPACE)) {
				m_nowState = END_ANIME;
				m_frameCount = 0;
			}
			break;

		case END_ANIME:
			if (m_frameCount >= ANIMATION_FRAME) {
				m_destroyFlag = STACKED;
				m_endScene = true;
				m_nextScene = GAME;
				m_frameCount = 0;
			}
			break;
		}
	}

	void TitleScene::Draw(Direct3DRendererBase *pRenderer) {
		pRenderer->Clear();

		if (m_pBackgroundSprite != nullptr) m_pBackgroundSprite->Begin(D3DXSPRITE_ALPHABLEND);
		else return;
		switch (m_nowState) {
		case START_ANIME:
			m_pBackgroundSprite->Draw(m_backgroundTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(255, 255, 255, (int)(255.0f * m_frameCount/ANIMATION_FRAME + 0.5f)));
			m_pBackgroundSprite->Draw(m_titleTexture.GetPtr(), nullptr, &m_titleTextureCenter, nullptr, D3DCOLOR_RGBA(255, 255, 255, (int)(255.0f * m_frameCount/ANIMATION_FRAME + 0.5f)));
			break;

		case WAIT_KEY:
			m_pBackgroundSprite->Draw(m_backgroundTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(255, 255, 255, 255));
			m_pBackgroundSprite->Draw(m_titleTexture.GetPtr(), nullptr, &m_titleTextureCenter, nullptr, D3DCOLOR_RGBA(255, 255, 255, 255));
			break;

		case END_ANIME:
			auto a = (int)(255.0f * (1.0f - m_frameCount/ANIMATION_FRAME) + 0.5f);
			m_pBackgroundSprite->Draw(m_backgroundTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(255, 255, 255, 255));
			m_pBackgroundSprite->Draw(m_titleTexture.GetPtr(), nullptr, &m_titleTextureCenter, nullptr, D3DCOLOR_RGBA(255, 255, 255, (int)(255.0f - 255.0f * m_frameCount/ANIMATION_FRAME + 0.5f)));
			break;
		}
		m_pBackgroundSprite->End();
	}

}