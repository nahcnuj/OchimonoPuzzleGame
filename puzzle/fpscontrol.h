#pragma once

#include "windows.h"

namespace Puzzle
{

	class FPSControl
	{
	private:
		int m_nowFrameCount;
		int m_nowFPSCount;
		bool m_drawFlag;
		int m_timeResetFrameCount;
		int m_timeResetFPSCount;
		int m_stableFrame;
		int m_nowFPS;

	public:
		FPSControl();
		FPSControl(int);
		void TimeControl();
		bool GetDrawFlag();
		int GetFPS();
	};

}
