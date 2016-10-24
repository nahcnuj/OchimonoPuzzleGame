#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "comptr.h"

using namespace IKD;

namespace Puzzle
{

	class LostResource
	{
	public:
		virtual void Backup() = 0;
		virtual void Recover(IDirect3DDevice9* pDevice) = 0;
	};

	class LostTexture : public LostResource
	{
	private:
		Com_ptr<IDirect3DTexture9> m_cpTex;
		D3DSURFACE_DESC m_desc;

	public:
		void Regist(Com_ptr<IDirect3DTexture9> cpTex);
		virtual void Backup();
		virtual void Recover(IDirect3DDevice9* pDevice);
	};

	class LostVolumeTexture : public LostResource
	{
	private:
		Com_ptr<IDirect3DVolumeTexture9> m_cpTex;
		D3DVOLUME_DESC m_desc;

	public:
		void Regist(Com_ptr<IDirect3DVolumeTexture9> cpTex);
		virtual void Backup();
		virtual void Recover(IDirect3DDevice9* pDevice);
	};

	class LostCubeTexture : public LostResource
	{
	private:
		Com_ptr<IDirect3DCubeTexture9> m_cpTex;
		D3DSURFACE_DESC m_desc;

	public:
		void Regist(Com_ptr<IDirect3DCubeTexture9> cpTex);
		virtual void Backup();
		virtual void Recover(IDirect3DDevice9* pDevice);
	};

	class LostSurface : public LostResource
	{
	private:
		Com_ptr<IDirect3DSurface9> m_cpTex;
		D3DSURFACE_DESC m_desc;

	public:
		void Regist(Com_ptr<IDirect3DSurface9> cpTex);
		virtual void Backup();
		virtual void Recover(IDirect3DDevice9* pDevice);
	};

	class LostVertexBuffer : public LostResource
	{
	private:
		Com_ptr<IDirect3DVertexBuffer9> m_cpTex;
		D3DVERTEXBUFFER_DESC m_desc;

	public:
		void Regist(Com_ptr<IDirect3DVertexBuffer9> cpTex);
		virtual void Backup();
		virtual void Recover(IDirect3DDevice9* pDevice);
	};

	class LostIndexBuffer : public LostResource
	{
	private:
		Com_ptr<IDirect3DIndexBuffer9> m_cpTex;
		D3DINDEXBUFFER_DESC m_desc;

	public:
		void Regist(Com_ptr<IDirect3DIndexBuffer9> cpTex);
		virtual void Backup();
		virtual void Recover(IDirect3DDevice9* pDevice);
	};

}
