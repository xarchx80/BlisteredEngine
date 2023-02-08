#include "PCH.h"
#include "DX11Context.h"

DX11Context::DX11Context(uint cx, uint cy, HWND hwnd, bool vsync)
	: Context(cx, cy, hwnd, vsync)
{
	CreateDeviceContext();
	CreateSwapChain();
	CreateRenderTarget();
	CreateDepthStencilView();
	InitViewport();
	CreateStates();
}

DX11Context::~DX11Context()
{
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilView);
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mDepthStencilBuffer);
	if (mContext) mContext->ClearState();
	SAFE_RELEASE(mRSCullBack);
	SAFE_RELEASE(mRSCullNone);
	SAFE_RELEASE(mDevice);
	SAFE_RELEASE(mContext);
}

void DX11Context::ClearBuffer(float r, float g, float b, float a)
{
	float rgba[] = { r,g,b,a };
	mContext->ClearRenderTargetView(mRenderTargetView, rgba);
	mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DX11Context::SwapChain()
{
	mSwapChain->Present(mVsync ? 1 : 0, 0);
}

void DX11Context::CreateDeviceContext()
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&mDevice,
		&featureLevel,
		&mContext);

	if (FAILED(hr)) {
		LOG_ERROR("D3D11CreateDevice Failed");
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		LOG_ERROR("Direct3D Feature Level 11 unsupported");
	}
}

void DX11Context::CreateSwapChain()
{
	uint msaa = 0;
	LOG_HR << mDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa);
	if (msaa < 0)
		LOG_ERROR("unknown msaa levels");

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mWidth;
	sd.BufferDesc.Height = mHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mHwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	LOG_HR << mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	LOG_HR << dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	LOG_HR << dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	LOG_HR << dxgiFactory->CreateSwapChain(mDevice, &sd, &mSwapChain);

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);
}

void DX11Context::CreateRenderTarget()
{
	ID3D11Texture2D* backBuffer = nullptr;
	LOG_HR << mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	LOG_HR << mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
	SAFE_RELEASE(backBuffer);

	//mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void DX11Context::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mWidth;
	depthStencilDesc.Height = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//if (mEnableMsaa) {
	//	depthStencilDesc.SampleDesc.Count = 4;
	//	depthStencilDesc.SampleDesc.Quality = mMsaa - 1;
	//}
	//else {
	//	depthStencilDesc.SampleDesc.Count = 1;
	//	depthStencilDesc.SampleDesc.Quality = 0;
	//}

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	LOG_HR << mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	LOG_HR << mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void DX11Context::InitViewport()
{
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<float>(mWidth);
	vp.Height = static_cast<float>(mHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	mContext->RSSetViewports(1, &vp);
}

void DX11Context::CreateStates()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;
	LOG_HR << mDevice->CreateRasterizerState(&desc, &mRSCullBack);
	desc.CullMode = D3D11_CULL_NONE;
	LOG_HR << mDevice->CreateRasterizerState(&desc, &mRSCullNone);

	mContext->RSSetState(mRSCullBack);
}
