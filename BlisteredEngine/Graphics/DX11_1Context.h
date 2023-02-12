#pragma once

#include <Graphics\Context.h>
#include <Graphics\DX11Config.h>
#include <wrl.h>

//using namespace Microsoft::WRL;

class DX11_1Context : public Context
{
public:
	DX11_1Context(uint cx, uint cy, HWND hwnd, bool vsync);
	~DX11_1Context();

	void ClearBuffer(float r, float g, float b, float a) override;
	void SwapChain();

	ID3D11Device* GetDX11Device() const override { return mDevice.Get(); }
	ID3D11DeviceContext* GetDX11Context() const override{ return mContext.Get(); }
protected:
	void CreateDevice();
	void CreateSwapChain();
private:
	ComPtr<ID3D11Device1>			mDevice;
	ComPtr<ID3D11DeviceContext1>	mContext;
	ComPtr<IDXGISwapChain1>			mSwapChain;
	ComPtr<ID3D11RenderTargetView>  mRtv;
	ComPtr<ID3D11DepthStencilView>	mDsv;
	ComPtr<ID3D11Texture2D>			mDsBuffer;

	bool mIsFlipModelSupported;
};