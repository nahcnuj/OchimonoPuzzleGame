#include "rendererbase.h"

namespace Puzzle
{

	Direct3DRendererBase::Direct3DRendererBase(bool isWindowed = false)
		: m_pD3D(nullptr), m_pD3DDevice(nullptr), m_pD3DXLineAxis(nullptr),
		  m_clientWidth(640), m_clientHeight(480), m_isWindowed(isWindowed) {
	}
	
	Direct3DRendererBase::~Direct3DRendererBase() {
		DestroyDirect3DResource();

		if (m_pD3DDevice) {
			m_pD3DDevice->Release();
			m_pD3DDevice = nullptr;
		}
		if (m_pD3D) {
			m_pD3D->Release();
			m_pD3D = nullptr;
		}
	}

	bool Direct3DRendererBase::SetupViewport() {
		DestroyDirect3DResource();

		m_d3dPresentParam.BackBufferWidth = m_clientWidth;
		m_d3dPresentParam.BackBufferHeight = m_clientHeight;
		if ( FAILED( m_pD3DDevice->Reset(&m_d3dPresentParam) ) ) 
			return false;

		RestoreDirect3DResource();

		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		D3DVIEWPORT9 viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = m_clientWidth;
		viewport.Height = m_clientHeight;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		if ( FAILED( m_pD3DDevice->SetViewport(&viewport) ) )
			return false;

		return true;
	}

	bool Direct3DRendererBase::SetupViewingFrustum() {
		D3DXMATRIX matrixProj;
		D3DXMatrixOrthoOffCenterRH(
			&matrixProj,
			-m_clientWidth * 0.5f / 10.0f, m_clientWidth * 0.5f / 10.0f,
			-m_clientHeight * 0.5f / 10.0f, m_clientHeight * 0.5f / 10.0f,
			0.1f, 1000.0f
		);

		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matrixProj);
		return true;
	}

	bool Direct3DRendererBase::SetupViewingTransform() {
		D3DXMATRIX matrixView;
		D3DXMatrixLookAtRH(
			&matrixView,
			&D3DXVECTOR3(0.0f, 0.0f, 500.0f),	//視点の場所
			&D3DXVECTOR3(0.0f, 0.0f,   0.0f),	//注視点
			&D3DXVECTOR3(0.0f, 1.0f,   0.0f)	//上向きのベクトル
		);
		m_pD3DDevice->SetTransform(D3DTS_VIEW, &matrixView);
		return true;
	}

	void Direct3DRendererBase::Resize(HWND hWnd, int width, int height) {
		if (width <= 0 || height <= 0) return;

		if (m_pD3D == nullptr) Init(hWnd);

		if (m_pD3D != nullptr) {
			m_clientWidth = width;
			m_clientHeight = height;
			
			SetupViewport();
			SetupViewingFrustum();
			SetupViewingTransform();
			//SetupModelingTransform();
			//SetupLight();
		}
	}

	bool Direct3DRendererBase::RestoreDirect3DResource() {
		if ( FAILED( D3DXCreateLine(m_pD3DDevice, &m_pD3DXLineAxis) ) )
			return false;

		m_pD3DXLineAxis->SetWidth(2);
		return true;
	}

	void Direct3DRendererBase::DestroyDirect3DResource() {
		if (m_pD3DXLineAxis) {
			m_pD3DXLineAxis->Release();
			m_pD3DXLineAxis = nullptr;
		}
	}

	bool Direct3DRendererBase::Init(HWND hWnd) {
		if ( (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr )
			return false;
		
		ZeroMemory(&m_d3dPresentParam, sizeof(m_d3dPresentParam));
		m_d3dPresentParam.BackBufferWidth = m_clientWidth;
		m_d3dPresentParam.BackBufferHeight = m_clientHeight;
		m_d3dPresentParam.BackBufferFormat = D3DFMT_A8R8G8B8;
		m_d3dPresentParam.BackBufferCount = 1;
		m_d3dPresentParam.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_d3dPresentParam.MultiSampleQuality = 0;
		m_d3dPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dPresentParam.hDeviceWindow = hWnd;
		m_d3dPresentParam.Windowed = m_isWindowed ? TRUE : FALSE;
		m_d3dPresentParam.EnableAutoDepthStencil = TRUE;
		m_d3dPresentParam.AutoDepthStencilFormat = D3DFMT_D16;
		m_d3dPresentParam.Flags = D3DPRESENT_RATE_DEFAULT;
		m_d3dPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		HRESULT hr;
		hr = m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&m_d3dPresentParam,
			&m_pD3DDevice
		);
		if ( FAILED(hr) ) {
			hr = m_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dPresentParam,
				&m_pD3DDevice
			);
			if ( FAILED(hr) ) {
				hr = m_pD3D->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					hWnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&m_d3dPresentParam,
					&m_pD3DDevice
				);
				if ( FAILED(hr) ) {
					hr = m_pD3D->CreateDevice(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_REF,
						hWnd,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						&m_d3dPresentParam,
						&m_pD3DDevice
					);
					if ( FAILED(hr) ) {
						return false;
					}
				}
			}
		}

		return true;
	}

	D3DXMATRIX Direct3DRendererBase::GetTransformMatrix() {
		D3DXMATRIX matrixWV, matrixWVP, matrixWorld, matrixView, matrixProj;
		
		m_pD3DDevice->GetTransform(D3DTS_WORLD, &matrixWorld);
		m_pD3DDevice->GetTransform(D3DTS_VIEW, &matrixView);
		m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matrixProj);

		D3DXMatrixMultiply(&matrixWV, &matrixWorld, &matrixView);
		D3DXMatrixMultiply(&matrixWVP, &matrixWV, &matrixProj);

		return matrixWVP;
	}

	void Direct3DRendererBase::RenderAxis() {
		const float length = 10.0f;
		D3DXMATRIX transformMatrix = GetTransformMatrix();

		D3DXVECTOR3 origin = D3DXVECTOR3( 0.0f,  0.0f,  0.0f);

		D3DXVECTOR3 axisX[2] = { origin, D3DXVECTOR3(length,  0.0f,  0.0f) };
		D3DXVECTOR3 axisY[2] = { origin, D3DXVECTOR3(0.0f,  length,  0.0f) };
		D3DXVECTOR3 axisZ[2] = { origin, D3DXVECTOR3(0.0f,  0.0f,  length) };

		if (m_pD3DXLineAxis->Begin() == D3D_OK) {
			HRESULT hr = m_pD3DXLineAxis->DrawTransform(axisX, 2, &transformMatrix, D3DCOLOR_XRGB(0xff, 0x00, 0x00));
			m_pD3DXLineAxis->DrawTransform(axisY, 2, &transformMatrix, D3DCOLOR_XRGB(0x00, 0xff, 0x00));
			m_pD3DXLineAxis->DrawTransform(axisZ, 2, &transformMatrix, D3DCOLOR_XRGB(0x00, 0x00, 0xff));
			m_pD3DXLineAxis->End();
		}
	}

}
