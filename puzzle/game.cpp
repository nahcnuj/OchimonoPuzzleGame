#include "game.h"
#include <queue>
#include <functional>
#include <tchar.h>

namespace Puzzle
{

	GameScene::GameScene(RECT rc)
		: m_nowState(PLAY),
		  m_backgroundTexture(nullptr),
		  m_pBackgroundSprite(nullptr),
		  m_pScoreSprite(nullptr),
		  m_pNextSprite(nullptr),
		  m_size(rc),
		  m_frameCount(0),
		  m_score(0),
		  m_field(new Field()),
		  m_nowBlockPos(0),
		  m_ranking() {
		m_nowBlocks[0] = new Block();
		m_nowBlocks[1] = new Block();
		m_nextBlocks[0] = new Block();
		m_nextBlocks[1] = new Block();
		CreateNextBlocks();
	}

	const int GameScene::DX[4] = {0, -1, 0, 1};
	const int GameScene::DY[4] = {1, 0, -1, 0};

	void GameScene::DoFrame(IDirect3DDevice9 *pDevice, DirectInput *dInput) {
		++m_frameCount;

		if (m_pBackgroundSprite == nullptr) {
			TCHAR fn[100];
			D3DXIMAGE_INFO info;

			#ifdef _DEBUG
				_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\titleback.png"));
			#else
				_stprintf_s(fn, _T(".\\resources\\image\\titleback.png"));
			#endif
			
			D3DXGetImageInfoFromFile(fn, &info);

			D3DXCreateTextureFromFileEx(pDevice, fn,
				info.Width, info.Height, info.MipLevels,
				0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, nullptr, nullptr, 
				m_backgroundTexture.ToCreator());
			
			LostTexture lostTexture;
			lostTexture.Regist(m_backgroundTexture);
			m_lostResource.push_back(&lostTexture);

			m_backgroundTextureCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			#ifdef _DEBUG
				_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\gameback.png"));
			#else
				_stprintf_s(fn, _T(".\\resources\\image\\gameback.png"));
			#endif

			D3DXGetImageInfoFromFile(fn, &info);

			D3DXCreateTextureFromFileEx(pDevice, fn,
				info.Width, info.Height, info.MipLevels,
				0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, nullptr, nullptr, 
				m_gamebackTexture.ToCreator());

			lostTexture.Regist(m_gamebackTexture);
			m_lostResource.push_back(&lostTexture);

			m_gamebackTextureCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			#ifdef _DEBUG
				_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\resultback.png"));
			#else
				_stprintf_s(fn, _T(".\\resources\\image\\resultback.png"));
			#endif

			D3DXGetImageInfoFromFile(fn, &info);

			D3DXCreateTextureFromFileEx(pDevice, fn,
				info.Width, info.Height, info.MipLevels,
				0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
				0, nullptr, nullptr, 
				m_resultbackTexture.ToCreator());

			lostTexture.Regist(m_resultbackTexture);
			m_lostResource.push_back(&lostTexture);

			if (m_pScoreSprite == nullptr) {
				#ifdef _DEBUG
					_stprintf_s(fn, _T(".\\..\\Release\\resources\\image\\numberfont.png"));
				#else
					_stprintf_s(fn, _T(".\\resources\\image\\numberfont.png"));
				#endif

				D3DXGetImageInfoFromFile(fn, &info);

				D3DXCreateTextureFromFileEx(pDevice, fn,
					info.Width, info.Height, info.MipLevels,
					0, info.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
					0, nullptr, nullptr, 
					m_numberTexture.ToCreator());
					
				lostTexture.Regist(m_numberTexture);
				m_lostResource.push_back(&lostTexture);

				D3DXCreateSprite(pDevice, &m_pScoreSprite);
			}

			if (m_pNextSprite == nullptr) {
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

				lostTexture.Regist(m_blockTexture);
				m_lostResource.push_back(&lostTexture);

				D3DXCreateSprite(pDevice, &m_pNextSprite);
			}

			D3DXCreateSprite(pDevice, &m_pBackgroundSprite);
		}

		switch (m_nowState) {
		case HOWTO:
			if (m_frameCount >= 30) {
				m_nowState = PLAY;
				m_frameCount = 0;
			}
			break;

		case PLAY:
			m_field->DoFrame(pDevice, dInput, &m_lostResource);
			
			if (m_score < 10*2500 && m_frameCount%(10-m_score/2500) == 0) {
				++m_nowBlocks[0]->m_y;
				++m_nowBlocks[1]->m_y;
			}
			else if (m_score >= 10*2500) {
				m_nowBlocks[0]->m_y += (m_score-10*2500) / 2500 + 2;
				m_nowBlocks[1]->m_y += (m_score-10*2500) / 2500 + 2;
			}

			if (m_nowBlockPos == 0) {
				int x = (m_nowBlocks[0]->m_x-60)/40, y = (m_nowBlocks[0]->m_y-40)/40;
				if (0 <= y && y+2 < Field::HEIGHT && m_field->m_block[y+2][x]->m_kind != Block::EMPTY) {
					m_field->m_block[y][x]->Copy(*m_nowBlocks[0]);
					m_field->m_block[y+1][x]->Copy(*m_nowBlocks[1]);
					m_nowBlocks[0]->m_kind = Block::EMPTY;
					m_nowBlocks[1]->m_kind = Block::EMPTY;
				}
			}
			else if (m_nowBlockPos == 1) {
				int x = (m_nowBlocks[0]->m_x-60)/40, y = (m_nowBlocks[0]->m_y-40)/40;
				if (0 <= y && y+1 <= Field::HEIGHT
					&& m_field->m_block[y+1][x]->m_kind != Block::EMPTY
					|| m_field->m_block[y+1][x-1]->m_kind != Block::EMPTY) {
					m_field->m_block[y][x]->Copy(*m_nowBlocks[0]);
					m_field->m_block[y][x-1]->Copy(*m_nowBlocks[1]);
					m_nowBlocks[0]->m_kind = Block::EMPTY;
					m_nowBlocks[1]->m_kind = Block::EMPTY;
				}
			}
			else if (m_nowBlockPos == 2) {
				int x = (m_nowBlocks[0]->m_x-60)/40, y = (m_nowBlocks[0]->m_y-40)/40;
				if (0 <= y-1 && y+1 <= Field::HEIGHT
					&& m_field->m_block[y+1][x]->m_kind != Block::EMPTY) {
					m_field->m_block[y][x]->Copy(*m_nowBlocks[0]);
					m_field->m_block[y-1][x]->Copy(*m_nowBlocks[1]);
					m_nowBlocks[0]->m_kind = Block::EMPTY;
					m_nowBlocks[1]->m_kind = Block::EMPTY;
				}
			}
			else if (m_nowBlockPos == 3) {
				int x = (m_nowBlocks[0]->m_x-60)/40, y = (m_nowBlocks[0]->m_y-40)/40;
				if (0 <= y && y+1 <= Field::HEIGHT
					&& m_field->m_block[y+1][x]->m_kind != Block::EMPTY
					|| m_field->m_block[y+1][x+1]->m_kind != Block::EMPTY) {
					m_field->m_block[y][x]->Copy(*m_nowBlocks[0]);
					m_field->m_block[y][x+1]->Copy(*m_nowBlocks[1]);
					m_nowBlocks[0]->m_kind = Block::EMPTY;
					m_nowBlocks[1]->m_kind = Block::EMPTY;
				}
			}

			if (dInput->IsTriggerKey(DIK_LEFT)) {
				if (m_nowBlockPos == 1) {
					if (m_nowBlocks[0]->m_x > 140) {
						m_nowBlocks[0]->m_x -= 40;
						m_nowBlocks[1]->m_x -= 40;
					}
				}
				else {
					if (m_nowBlocks[0]->m_x > 100) {
						m_nowBlocks[0]->m_x -= 40;
						m_nowBlocks[1]->m_x -= 40;
					}
				}
			}
			else if (dInput->IsTriggerKey(DIK_RIGHT)) {
				if (m_nowBlockPos == 3) {
					if (m_nowBlocks[0]->m_x < 260) {
						m_nowBlocks[0]->m_x += 40;
						m_nowBlocks[1]->m_x += 40;
					}
				}
				else {
					if (m_nowBlocks[0]->m_x < 300) {
						m_nowBlocks[0]->m_x += 40;
						m_nowBlocks[1]->m_x += 40;
					}
				}
			}
			else if (dInput->IsTriggerKey(DIK_UP)) {
				bool f[2] = {}, isFilled = false;
				for (int i = 0; i < m_field->HEIGHT; ++i) {
					if (!f[0] && m_field->m_block[i][(m_nowBlocks[0]->m_x-60)/40]->m_kind != Block::EMPTY) {
						if (m_nowBlockPos == 0) {
							if (i > 1) m_field->m_block[i-2][(m_nowBlocks[0]->m_x-60)/40]->Copy(*m_nowBlocks[0]);
							else isFilled = true;
							m_field->m_block[i-1][(m_nowBlocks[0]->m_x-60)/40]->Copy(*m_nowBlocks[1]);
							break;
						}
						else if (m_nowBlockPos == 2) {
							if (i > 1) m_field->m_block[i-2][(m_nowBlocks[0]->m_x-60)/40]->Copy(*m_nowBlocks[1]);
							else isFilled = true;
							m_field->m_block[i-1][(m_nowBlocks[0]->m_x-60)/40]->Copy(*m_nowBlocks[0]);
							break;
						}
						else if (m_nowBlockPos == 1 || m_nowBlockPos == 3) {
							m_field->m_block[i-1][(m_nowBlocks[0]->m_x-60)/40]->Copy(*m_nowBlocks[0]);
							f[0] = true;
						}
					}
					if (!f[1]) {
						if (m_nowBlockPos == 1 && m_field->m_block[i][(m_nowBlocks[0]->m_x-60)/40-1]->m_kind != Block::EMPTY) {
							m_field->m_block[i-1][(m_nowBlocks[0]->m_x-60)/40-1]->Copy(*m_nowBlocks[1]);
							f[1] = true;
						}
						else if (m_nowBlockPos == 3 && m_field->m_block[i][(m_nowBlocks[0]->m_x-60)/40+1]->m_kind != Block::EMPTY) {
							m_field->m_block[i-1][(m_nowBlocks[0]->m_x-60)/40+1]->Copy(*m_nowBlocks[1]);
							f[1] = true;
						}
					}
				}
				m_nowBlocks[0]->m_kind = Block::EMPTY;
				m_nowBlocks[1]->m_kind = Block::EMPTY;
				if (isFilled) {
					m_nowState = RESULT;
					m_frameCount = 0;
					break;
				}
			}
			else if (dInput->IsReleaseKey(DIK_DOWN)) {
				m_nowBlockPos = (m_nowBlockPos+1) % 4;
				if (m_nowBlockPos == 1 && m_nowBlocks[0]->m_x < 140) m_nowBlockPos = 2;
				else if (m_nowBlockPos == 3 && m_nowBlocks[0]->m_x > 260) m_nowBlockPos = 0;
			}

			ClearBlock();

			for (int i = 1; i < Field::WIDTH-1; ++i) {
				if (m_field->m_block[0][i]->m_kind != Block::EMPTY) {
					m_nowState = RESULT;
					m_frameCount = 0;
					break;
				}
			}

			if (m_nowBlocks[0]->m_kind == Block::EMPTY) {
				m_nowBlocks[0]->Copy(*m_nextBlocks[0]);
				m_nowBlocks[0]->ResetPos();
				m_nowBlocks[1]->Copy(*m_nextBlocks[1]);
				m_nowBlocks[1]->ResetPos();
				CreateNextBlocks();
			}
			break;

		case RESULT:
			if (m_ranking.empty()) {
				LoadRanking();
				m_ranking.push_back(m_score);
				sort(m_ranking.begin(), m_ranking.end(), std::greater<int>());
				SaveRanking();
			}

			if (dInput->IsReleaseKey(DIK_SPACE)) {
				m_destroyFlag = ALL_DESTROY;
				m_endScene = true;
				m_nextScene = TITLE;
				m_frameCount = 0;
			}
			break;
		}
	}

	void GameScene::Draw(Direct3DRendererBase *pRenderer) {
		pRenderer->Clear();

		if (m_pBackgroundSprite) m_pBackgroundSprite->Begin(D3DXSPRITE_ALPHABLEND);
		else return;
		if (m_pNextSprite) m_pNextSprite->Begin(D3DXSPRITE_ALPHABLEND);
		else return;
		switch (m_nowState) {
		case HOWTO:
			m_pBackgroundSprite->Draw(m_backgroundTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
			m_pBackgroundSprite->Draw(m_gamebackTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(0xff, 0xff, 0xff, (int)(255.0f * m_frameCount/60 + 0.5f)));
			break;

		case PLAY:
			m_pBackgroundSprite->Draw(m_backgroundTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
			m_pBackgroundSprite->Draw(m_gamebackTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));

			//フィールド
			m_field->Draw(m_pBackgroundSprite);
			//次
			for (int i = 0; i < 2; ++i) {
				D3DXVECTOR3* pos = new D3DXVECTOR3(480, 100 + 40*i, 0);
				if (m_nextBlocks[i]->m_kind == Block::NUMBER) {
					RECT rc = {80 + (m_nextBlocks[i]->m_number-1) * 40, 0, 80 + m_nextBlocks[i]->m_number * 40, 40};
					m_pNextSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
				}
				else if (m_nextBlocks[i]->m_kind == Block::OPERATOR) {
					if (m_nextBlocks[i]->m_operator == Block::PLUS) {
						RECT rc = {440, 0, 480, 40};
						m_pNextSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_nextBlocks[i]->m_operator == Block::MINUS) {
						RECT rc = {480, 0, 520, 40};
						m_pNextSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_nextBlocks[i]->m_operator == Block::MULTIPLY) {
						RECT rc = {520, 0, 560, 40};
						m_pNextSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_nextBlocks[i]->m_operator == Block::DIVIDE) {
						RECT rc = {560, 0, 600, 40};
						m_pNextSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
				}
			}
			//落下中のブロック
			for (int i = 0; i < 2; ++i) {
				D3DXVECTOR3* pos = new D3DXVECTOR3(m_nowBlocks[i]->m_x + i*DX[m_nowBlockPos]*40, m_nowBlocks[i]->m_y + i*DY[m_nowBlockPos]*40, 0);
				if (m_nowBlocks[i]->m_kind == Block::NUMBER) {
					RECT rc = {80 + (m_nowBlocks[i]->m_number-1) * 40, 0, 80 + m_nowBlocks[i]->m_number * 40, 40};
					m_pBackgroundSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
				}
				else if (m_nowBlocks[i]->m_kind == Block::OPERATOR) {
					if (m_nowBlocks[i]->m_operator == Block::PLUS) {
						RECT rc = {440, 0, 480, 40};
						m_pBackgroundSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_nowBlocks[i]->m_operator == Block::MINUS) {
						RECT rc = {480, 0, 520, 40};
						m_pBackgroundSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_nowBlocks[i]->m_operator == Block::MULTIPLY) {
						RECT rc = {520, 0, 560, 40};
						m_pBackgroundSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
					else if (m_nowBlocks[i]->m_operator == Block::DIVIDE) {
						RECT rc = {560, 0, 600, 40};
						m_pBackgroundSprite->Draw(m_blockTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
					}
				}
			}
			//スコア
			char tmp[20];
			sprintf_s(tmp, "% 8.1f", (float)m_score/10.0f);
			for (int i = 0; i < strlen(tmp); ++i) {
				if (tmp[i] == ' ') continue;
				RECT rc = {(tmp[i]!='.' ? tmp[i]-'0' : 10)*20, 0, (tmp[i]!='.' ? tmp[i]-'0'+1 : 11)*20, 40};
				D3DXVECTOR3* pos = new D3DXVECTOR3(420 + 20*i, 280, 0);
				m_pBackgroundSprite->Draw(m_numberTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
			}
			//レベル
			sprintf_s(tmp, "% 8d", m_score/2500+1);
			for (int i = 0; i < strlen(tmp); ++i) {
				if (tmp[i] == ' ') continue;
				RECT rc = {(tmp[i]-'0')*20, 0, (tmp[i]-'0'+1)*20, 40};
				D3DXVECTOR3* pos = new D3DXVECTOR3(420 + 20*i, 400, 0);
				m_pBackgroundSprite->Draw(m_numberTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
			}
			break;

		case RESULT:
			m_pBackgroundSprite->Draw(m_backgroundTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
			m_pBackgroundSprite->Draw(m_resultbackTexture.GetPtr(), nullptr, &m_backgroundTextureCenter, nullptr, D3DCOLOR_RGBA(0xff, 0xff, 0xff, m_frameCount>=30 ? 0xff : (int)(255.0f * m_frameCount/30 + 0.5f)));
			
			//スコア
			sprintf_s(tmp, "% 8.1f", (float)m_score/10.0f);
			for (int i = 0; i < strlen(tmp); ++i) {
				if (tmp[i] == ' ') continue;
				RECT rc = {(tmp[i]!='.' ? tmp[i]-'0' : 10)*20, 0, (tmp[i]!='.' ? tmp[i]-'0'+1 : 11)*20, 40};
				D3DXVECTOR3* pos = new D3DXVECTOR3(400 + 20*i, 94, 0);
				m_pBackgroundSprite->Draw(m_numberTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
			}
			//ランキング
			if (m_ranking.empty()) {
				LoadRanking();
				m_ranking.push_back(m_score);
				sort(m_ranking.begin(), m_ranking.end(), std::greater<int>());
				SaveRanking();
			}
			for (int i = 0; i < 5; ++i) {
				sprintf_s(tmp, "% 8.1f", (float)m_ranking[i]/10.0f);
				for (int j = 0; j < strlen(tmp); ++j) {
					if (tmp[j] == ' ') continue;
					RECT rc = {(tmp[j]!='.' ? tmp[j]-'0' : 10)*20, 0, (tmp[j]!='.' ? tmp[j]-'0'+1 : 11)*20, 40};
					D3DXVECTOR3* pos = new D3DXVECTOR3(280 + 20*j, 147 + 56*i, 0);
					m_pBackgroundSprite->Draw(m_numberTexture.GetPtr(), &rc, nullptr, pos, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
				}
			}
			break;
		}
		m_pBackgroundSprite->End();
		m_pNextSprite->End();
	}

	void GameScene::CreateNextBlocks() {
		for (int i = 0; i < 2; ++i) {
			int a = rand() % 13 + 1;
			if (1 <= a && a <= 9) {
				m_nextBlocks[i]->m_kind = Block::NUMBER;
				m_nextBlocks[i]->m_number = a;
				m_nextBlocks[i]->m_state = Block::REST;
			}
			else if (a == 10) {
				m_nextBlocks[i]->m_kind = Block::OPERATOR;
				m_nextBlocks[i]->m_operator = Block::PLUS;
				m_nextBlocks[i]->m_state = Block::REST;
			}
			else if (a == 11) {
				m_nextBlocks[i]->m_kind = Block::OPERATOR;
				m_nextBlocks[i]->m_operator = Block::MINUS;
				m_nextBlocks[i]->m_state = Block::REST;
			}
			else if (a == 12) {
				m_nextBlocks[i]->m_kind = Block::OPERATOR;
				m_nextBlocks[i]->m_operator = Block::MULTIPLY;
				m_nextBlocks[i]->m_state = Block::REST;
			}
			else if (a == 13) {
				m_nextBlocks[i]->m_kind = Block::OPERATOR;
				m_nextBlocks[i]->m_operator = Block::DIVIDE;
				m_nextBlocks[i]->m_state = Block::REST;
			}
		}
	}

	void GameScene::ClearBlock() {
		std::queue<Block *> q;	//削除するブロックを入れる
		for (int i = 0; i < Field::HEIGHT-1; ++i) {
			for (int j = 1; j < Field::WIDTH-1; ++j) {
				if (m_field->m_block[i][j]->m_kind != Block::NUMBER) continue;

				if (j <= 4) {	//横方向
					if (m_field->m_block[i][j+1]->m_kind == Block::OPERATOR
						&& m_field->m_block[i][j+2]->m_kind == Block::NUMBER) {
						switch (m_field->m_block[i][j+1]->m_operator) {
						case Block::PLUS:
							m_score += (m_field->m_block[i][j]->m_number + m_field->m_block[i][j+2]->m_number) * 10;
							break;
						case Block::MINUS:
							m_score += (m_field->m_block[i][j]->m_number - m_field->m_block[i][j+2]->m_number) * 10;
							if (m_score < 0) m_score = 0;
							break;
						case Block::MULTIPLY:
							m_score += (m_field->m_block[i][j]->m_number * m_field->m_block[i][j+2]->m_number) * 10;
							break;
						case Block::DIVIDE:
							m_score += ((float)m_field->m_block[i][j]->m_number / m_field->m_block[i][j+2]->m_number) * 10;
							break;
						}

						for (int k = 0; k < 3; ++k) {
							q.push(m_field->m_block[i][j+k]);
						}
					}
				}

				if (i <= 6) {	//縦方向
					if (m_field->m_block[i+1][j]->m_kind == Block::OPERATOR
						&& m_field->m_block[i+2][j]->m_kind == Block::NUMBER) {
						switch (m_field->m_block[i+1][j]->m_operator) {
						case Block::PLUS:
							m_score += (m_field->m_block[i][j]->m_number + m_field->m_block[i+2][j]->m_number) * 10;
							break;
						case Block::MINUS:
							m_score += (m_field->m_block[i][j]->m_number - m_field->m_block[i+2][j]->m_number) * 10;
							if (m_score < 0) m_score = 0;
							break;
						case Block::MULTIPLY:
							m_score += (m_field->m_block[i][j]->m_number * m_field->m_block[i+2][j]->m_number) * 10;
							break;
						case Block::DIVIDE:
							m_score += ((float)m_field->m_block[i][j]->m_number / m_field->m_block[i+2][j]->m_number) * 10;
							break;
						}

						for (int k = 0; k < 3; ++k) {
							q.push(m_field->m_block[i+k][j]);
						}
					}
				}
			}
		}

		while (!q.empty()) {
			q.front()->m_kind = Block::EMPTY;
			q.pop();
		}

		bool needCheck;
		do {
			needCheck = false;
			for (int i = Field::HEIGHT-2; i > 0; --i) {
				for (int j = Field::WIDTH-2; j > 0; --j) {

					if (m_field->m_block[i][j]->m_kind == Block::EMPTY
						&& m_field->m_block[i-1][j]->m_kind != Block::EMPTY) {
						std::swap(*m_field->m_block[i][j], *m_field->m_block[i-1][j]);
						needCheck = true;
					}
				}
			}
		} while (needCheck);
	}

	void GameScene::LoadRanking() {
		#ifdef _DEBUG
			std::ifstream ifs(".\\..\\Release\\resources\\data\\ranking.txt");
		#else
			std::ifstream ifs(".\\resources\\data\\ranking.txt");
		#endif
		unsigned int data;
		while (ifs >> data) {
			m_ranking.push_back(data);
		}
	}

	void GameScene::SaveRanking() {
		#ifdef _DEBUG
			std::ofstream ofs(".\\..\\Release\\resources\\data\\ranking.txt", std::ios::out | std::ios::trunc);
		#else
			std::ofstream ofs(".\\resources\\data\\ranking.txt", std::ios::out | std::ios::trunc);
		#endif

		for (int i = 0; i < 5; ++i) {
			ofs << m_ranking[i] << std::endl;
		}
	}

}
