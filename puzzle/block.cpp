#include "block.h"

namespace Puzzle {

	Block::Block() : m_kind(EMPTY) {
		m_x = 220;
		m_y = 40;
	}

	Block::Block(bool isWall) : m_kind(isWall ? WALL : EMPTY) {
		m_x = 220;
		m_y = 40;
	}

	Block::Block(int number) : m_kind(NUMBER), m_number(number), m_state(REST) {
		m_x = 220;
		m_y = 40;
	}

	Block::Block(enum OPERATOR ope) : m_kind(OPERATOR), m_operator(ope), m_state(REST) {
		m_x = 220;
		m_y = 40;
	}

	Block::Block(const Block& block) {
		m_kind = EMPTY;
		m_number = 0;
		m_operator = PLUS;
		m_state = REST;
		m_x = 0;
		m_y = 0;

		this->Copy(block);
	}

	void Block::Copy(const Block& block) {
		m_kind = block.m_kind;
		m_number = block.m_number;
		m_operator = block.m_operator;
		m_state = block.m_state;
		m_x = block.m_x;
		m_y = block.m_y;
	}

	void Block::ResetPos() {
		m_x = 220;
		m_y = 40;
	}

	Block& Block::operator=(const Block& block) {
		this->Copy(block);

		return *this;
	}

}