#include <cstdio>
#include <tchar.h>
#include "fpscontrol.h"

namespace Puzzle
{

	FPSControl::FPSControl() : m_nowFrameCount(0), m_nowFPSCount(0), m_stableFrame(60), m_nowFPS(-1) {
		int now = GetTickCount();
		m_timeResetFrameCount = now;
		m_timeResetFPSCount = now;
	}

	FPSControl::FPSControl(int stableFrame) : m_nowFrameCount(0), m_nowFPSCount(0), m_stableFrame(stableFrame) {
		int now = GetTickCount();
		m_timeResetFrameCount = now;
		m_timeResetFPSCount = now;
	}

	void FPSControl::TimeControl() {
		++m_nowFrameCount;
		++m_nowFPSCount;

		int now = GetTickCount();
		int diff = now - m_timeResetFrameCount;
		int diff2 = now - m_timeResetFPSCount;

		m_drawFlag = true;
		if ( diff > 1000/m_stableFrame * m_nowFrameCount ) m_drawFlag = false;
		else Sleep(1000/m_stableFrame);

		if ( diff2 >= 1000) {
			m_nowFPS = 1000/((now-m_timeResetFPSCount)/m_stableFrame);

			#ifdef _DEBUG
				wchar_t tmp[256] = L"";
				_stprintf_s(tmp, L"%3d FPS (%5.3f sec)\n", m_nowFPS, diff2/1000.0f);
				OutputDebugString(tmp);
			#endif

			m_nowFPSCount = 0;
			m_timeResetFPSCount = GetTickCount();
		}
	}

	bool FPSControl::GetDrawFlag() {
		return m_drawFlag;
	}

	int FPSControl::GetFPS() {
		if (m_nowFPS < 0) return -1;
		return m_nowFPS < 0 ? -1 : m_nowFPS;
	}

}
