#pragma once

#include <Core\Type.h>

#define gDXDevice	DX11_1Context::gContext->GetDX11Device()
#define gDXContext  DX11_1Context::gContext->GetDX11Context()

class Context
{
public:
	Context(uint w, uint h, HWND hwnd, bool vsync);
	virtual ~Context() {}

	virtual void ClearBuffer(float, float, float, float) {};
	virtual void SwapChain() {};
	static Context* gContext;

	//DX11_1
	virtual ID3D11Device* GetDX11Device() const { return nullptr; };
	virtual ID3D11DeviceContext* GetDX11Context() const { return nullptr; };
protected:
	uint mWidth;
	uint mHeight;
	HWND mHwnd;
	bool mVsync;
	bool mEnableMsaa;
};

//Context* Context::gContext = nullptr;