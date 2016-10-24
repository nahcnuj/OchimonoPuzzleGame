#pragma once

#include <d3d9.h>
#include <d3dx9.h>

namespace Puzzle {

	class Direct3DRendererBase {
	private:
		LPDIRECT3D9 m_pD3D;
		D3DPRESENT_PARAMETERS m_d3dPresentParam;
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		ID3DXLine* m_pD3DXLineAxis;
		int m_clientWidth, m_clientHeight;
		bool m_isWindowed;
		
	public:
		Direct3DRendererBase(bool isWindowed);
		~Direct3DRendererBase();

		void Resize(HWND hWnd, int width, int height);
		inline HRESULT SetFVF(DWORD fvf) {
			m_pD3DDevice->SetFVF(fvf);
		}
		
		inline HRESULT BeginScene() {
			return m_pD3DDevice->BeginScene();
		}
		inline HRESULT EndScene() {
			return m_pD3DDevice->EndScene();
		}
		inline HRESULT Present(
			const RECT *pSourceRect,
			const RECT *pDestRect,
			HWND hDestWindowOverride,
			const RGNDATA *pDirtyRegion
		) {
			return m_pD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}

		inline HRESULT Clear() {
			return m_pD3DDevice->Clear(
				0,
				nullptr,
				D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				D3DCOLOR_XRGB(0, 0, 0),
				0.0,
				0
			);
		}
		void Render();
		
		inline IDirect3D9* GetDirect3DObject() {
			return m_pD3D;
		}
		inline IDirect3DDevice9* GetDirect3DDevice() {
			return m_pD3DDevice;
		}
		inline D3DPRESENT_PARAMETERS GetPresentParam() {
			return m_d3dPresentParam;
		}
		D3DXMATRIX GetTransformMatrix();

		void RenderAxis();

	private:
		bool Init(HWND hWnd);
		bool SetupViewport();
		bool SetupViewingFrustum();
		bool SetupViewingTransform();
		bool SetupModelingTransform();
		bool SetupLight();

		bool PreRenderScene() {return true;}
		bool RenderScene();
		bool PostRenderScene() {return true;}

		bool RestoreDirect3DResource();
		void DestroyDirect3DResource();
	};

}
