#pragma once
#include <list>
#include "directinput.h"
#include "rendererbase.h"
#include "lostresource.h"
#include <tchar.h>

namespace Puzzle
{

	struct Block
	{
		enum KIND {
			EMPTY,
			WALL,
			NUMBER,
			OPERATOR
		} m_kind;

		int m_number;	//0Å`9
		enum OPERATOR {
			PLUS,
			MINUS,
			MULTIPLY,
			DIVIDE
		} m_operator;
		
		enum STATE{
			FALL,	//óéâ∫íÜ
			MOVING, //à⁄ìÆíÜ
			REST,	//ê√é~íÜ
			VANISH	//è¡ñ≈íÜ
		} m_state;

		int m_x, m_y;

		Block();
		Block(bool isWall);
		Block(int number);
		Block(enum OPERATOR ope);
		Block(const Block& block);

		void Copy(const Block& block);
		Block& operator=(const Block& block);

		void ResetPos();
	};

}
