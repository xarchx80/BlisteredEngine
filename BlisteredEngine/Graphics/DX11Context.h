#pragma once

#include <Graphics\Context.h>
#include <Graphics\DX11Config.h>

class DX11Context : public Context
{
public:
	DX11Context(uint cx, uint cy, HWND hwnd, bool vsync);
	~DX11Context();

	void ClearBuffer(float r, float g, float b, float a);
	void SwapChain();

	ID3D11Device* GetDX11Device() const override { return mDevice; }
	ID3D11DeviceContext* GetDX11Context() const override { return mContext; }
protected:
	void CreateDeviceContext();
	void CreateSwapChain();
	void CreateRenderTarget();
	void CreateDepthStencilView();
	void InitViewport();
	void CreateStates();



	IDXGISwapChain*			mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D*		mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	//STATE
	ID3D11RasterizerState* mRSCullBack;
	ID3D11RasterizerState* mRSCullNone;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;

};

