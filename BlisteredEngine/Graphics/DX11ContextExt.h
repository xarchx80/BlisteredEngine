#pragma once

#pragma once

#include <Graphics\DX11Config.h>

#include <wrl.h>

using namespace Microsoft::WRL;

class DX11ContextExt
{
public:
	DX11ContextExt(uint cx, uint cy, HWND hwnd, bool vsync);

	ID3D11Device4* GetDevice() { return mDevice.Get(); }
	ID3D11DeviceContext4* GetContext() { return mContext.Get(); }

	void ClearBuffer(float r, float g, float b, float a);
	void SwapChain();
protected:
	void CreateFactoryCapablilities();
	void CreateContext();
	void CreateSwapChain();
	void CreateRenderTarget();

	void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);

	ComPtr<ID3D11Device4> mDevice = nullptr;
	ComPtr<ID3D11DeviceContext4> mContext = nullptr;
	ComPtr<IDXGIFactory2> mDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain1> mSwapChain = nullptr;

	ComPtr<ID3D11Texture2D> mBackBuffer;
	ComPtr<ID3D11Texture2D> mDepthstencilBackBuffer;
	ComPtr<ID3D11RenderTargetView> mRtv;
	ComPtr<ID3D11DepthStencilView> mDsv;

	uint mWidth;
	uint mHeight;
	HWND mHwnd;
	bool mVsync;
	bool mFlipDiscard = false;
	bool mAllowTearing = false;
	D3D_FEATURE_LEVEL mFeaturesLevel;
};

