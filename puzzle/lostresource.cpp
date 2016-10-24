#include "lostresource.h"
#include <algorithm>

void Puzzle::LostTexture::Regist(Com_ptr<IDirect3DTexture9> cpTex) {
	cpTex->GetLevelDesc(0, &m_desc);
	m_cpTex = cpTex;
}
void Puzzle::LostTexture::Backup() {
	if (m_cpTex.GetPtr()) {
		Com_ptr<IDirect3DTexture9> cpTmp;
		m_cpTex.Swap(cpTmp);
	}
}
void Puzzle::LostTexture::Recover(IDirect3DDevice9* pDevice) {
	Com_ptr<IDirect3DTexture9> cpTmp;
	D3DXCreateTexture(pDevice, m_desc.Width, m_desc.Height, 0,
		m_desc.Usage, m_desc.Format, m_desc.Pool, cpTmp.ToCreator());
	m_cpTex.Swap(cpTmp);
}

void Puzzle::LostVolumeTexture::Regist(Com_ptr<IDirect3DVolumeTexture9> cpTex) {
	cpTex->GetLevelDesc(0, &m_desc);
	m_cpTex = cpTex;
}
void Puzzle::LostVolumeTexture::Backup() {
	if (m_cpTex.GetPtr()) {
		Com_ptr<IDirect3DVolumeTexture9> cpTmp;
		m_cpTex.Swap(cpTmp);
	}
}
void Puzzle::LostVolumeTexture::Recover(IDirect3DDevice9* pDevice) {
	Com_ptr<IDirect3DVolumeTexture9> cpTmp;
	D3DXCreateVolumeTexture(pDevice, m_desc.Width, m_desc.Height, 0,
		D3DX_DEFAULT, m_desc.Usage, m_desc.Format, m_desc.Pool, cpTmp.ToCreator());
	m_cpTex.Swap(cpTmp);
}

void Puzzle::LostCubeTexture::Regist(Com_ptr<IDirect3DCubeTexture9> cpTex) {
	cpTex->GetLevelDesc(0, &m_desc);
	m_cpTex = cpTex;
}
void Puzzle::LostCubeTexture::Backup() {
	if (m_cpTex.GetPtr()) {
		Com_ptr<IDirect3DCubeTexture9> cpTmp;
		m_cpTex.Swap(cpTmp);
	}
}
void Puzzle::LostCubeTexture::Recover(IDirect3DDevice9* pDevice) {
	Com_ptr<IDirect3DCubeTexture9> cpTmp;
	D3DXCreateCubeTexture(pDevice, m_desc.Width, D3DX_DEFAULT, m_desc.Usage,
		m_desc.Format, m_desc.Pool, cpTmp.ToCreator());
	m_cpTex.Swap(cpTmp);
}

void Puzzle::LostSurface::Regist(Com_ptr<IDirect3DSurface9> cpTex) {
	cpTex->GetDesc(&m_desc);
	m_cpTex = cpTex;
}
void Puzzle::LostSurface::Backup() {
	if (m_cpTex.GetPtr()) {
		Com_ptr<IDirect3DSurface9> cpTmp;
		m_cpTex.Swap(cpTmp);
	}
}
void Puzzle::LostSurface::Recover(IDirect3DDevice9* pDevice) {
	Com_ptr<IDirect3DSurface9> cpTmp;
	pDevice->CreateDepthStencilSurface(m_desc.Width, m_desc.Height, m_desc.Format,
		D3DMULTISAMPLE_NONE, 0, FALSE, cpTmp.ToCreator(), nullptr);
	m_cpTex.Swap(cpTmp);
}

void Puzzle::LostVertexBuffer::Regist(Com_ptr<IDirect3DVertexBuffer9> cpTex) {
	cpTex->GetDesc(&m_desc);
	m_cpTex = cpTex;
}
void Puzzle::LostVertexBuffer::Backup() {
	if (m_cpTex.GetPtr()) {
		Com_ptr<IDirect3DVertexBuffer9> cpTmp;
		m_cpTex.Swap(cpTmp);
	}
}
void Puzzle::LostVertexBuffer::Recover(IDirect3DDevice9* pDevice) {
	Com_ptr<IDirect3DVertexBuffer9> cpTmp;
	pDevice->CreateVertexBuffer(m_desc.Size, m_desc.Usage, m_desc.FVF, m_desc.Pool, cpTmp.ToCreator(), nullptr);
	m_cpTex.Swap(cpTmp);
}

void Puzzle::LostIndexBuffer::Regist(Com_ptr<IDirect3DIndexBuffer9> cpTex) {
	cpTex->GetDesc(&m_desc);
	m_cpTex = cpTex;
}
void Puzzle::LostIndexBuffer::Backup() {
	if (m_cpTex.GetPtr()) {
		Com_ptr<IDirect3DIndexBuffer9> cpTmp;
		m_cpTex.Swap(cpTmp);
	}
}
void Puzzle::LostIndexBuffer::Recover(IDirect3DDevice9* pDevice) {
	Com_ptr<IDirect3DIndexBuffer9> cpTmp;
	pDevice->CreateIndexBuffer(m_desc.Size, m_desc.Usage, m_desc.Format, m_desc.Pool, cpTmp.ToCreator(), nullptr);
	m_cpTex.Swap(cpTmp);
}
