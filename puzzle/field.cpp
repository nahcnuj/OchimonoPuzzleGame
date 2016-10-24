#include "field.h"
#include <cassert>

namespace Puzzle
{

	Field::Field() : m_pFieldSprite(nullptr) {
		for (int i = 1; i <= HEIGHT; ++i) {
			for (int j = 1; j <= WIDTH; ++j) {
				if (i == HEIGHT) m_block[i-1][j-1] = new Block(true);
				else if (j == 1 || j == WIDTH) m_block[i-1][j-1] = new Block(true);
				else m_block[i-1][j-1] = new Block(false);
			}
		}
	}

	void Field::DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput, std::list<LostResource *> *lostResource) {
		if (m_blockTexture.GetPtr() == nullptr) {	
			TCHAR fn[100];
			D3DXIMAGE_INFO info;
			
			#ifdef _DEBUG
				_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\block.png"));
			#else
				_stprintf_s(fn, _T(".\\resources\\image\\block.png"));
			#endif

			D3DXGetImageInfoFromFile(fn, &info);

			D3DXCreateTextureFromFileEx(pDevice, fn,
				info.Width, info.Height, info.MipLevels,
				0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, nullptr, nullptr, 
				m_blockTexture.ToCreator());
					
			LostTexture lostTexture;
			lostTexture.Regist(m_blockTexture);
			lostResource->push_back(&lostTexture);
		}
	}

	void Field::Draw(LPD3DXSPRITE pSprite) {
		for (int i = 1; i <= HEIGHT; ++i) {
			for (int j = 1; j <= WIDTH; ++j) {
				D3DXVECTOR3* pos = new D3DXVECTOR3(60 + 40*(j-1), 40 + 40*(i-1), 0);

				if (m_block[i-1][j-1]->m_kind == Block::EMPTY) {
				}
				else if (m_block[i-1][j-1]->m_kind == Block::WALL) {
					RECT rc = {40, 0, 80, 40};
					pSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
				}
				else if (m_block[i-1][j-1]->m_kind == Block::NUMBER) {
					RECT rc = {80 + (m_block[i-1][j-1]->m_number-1)*40, 0, 80 + m_block[i-1][j-1]->m_number*40, 40};
					pSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
				}
				else if (m_block[i-1][j-1]->m_kind == Block::OPERATOR) {
					if (m_block[i-1][j-1]->m_operator == Block::PLUS) {
						RECT rc = {440, 0, 480, 40};
						pSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_block[i-1][j-1]->m_operator == Block::MINUS) {
						RECT rc = {480, 0, 520, 40};
						pSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_block[i-1][j-1]->m_operator == Block::MULTIPLY) {
						RECT rc = {520, 0, 560, 40};
						pSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_block[i-1][j-1]->m_operator == Block::DIVIDE) {
						RECT rc = {560, 0, 600, 40};
						pSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
				}

				delete pos;
			}
		}
	}

}
