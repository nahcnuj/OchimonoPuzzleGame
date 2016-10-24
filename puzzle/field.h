#pragma once
#include "rendererbase.h"
#include "lostresource.h"
#include "directinput.h"
#include "block.h"

namespace Puzzle
{

	class Field
	{

		Com_ptr<IDirect3DTexture9> m_blockTexture;
		LPD3DXSPRITE m_pFieldSprite;

	public:
		static const int WIDTH = 8;
		static const int HEIGHT = 10;
		Block *m_block[HEIGHT][WIDTH];

		Field();

		void DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput, std::list<LostResource *> *lostResource);
		void Draw(LPD3DXSPRITE pSprite);
	};

}
