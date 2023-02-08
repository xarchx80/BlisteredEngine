#include <PCH.h>
#include <Graphics\DX11_1Context.h>

DX11_1Context::DX11_1Context(uint cx, uint cy, HWND hwnd, bool vsync)
	: Context(cx, cy, hwnd, vsync), mIsFlipModelSupported(false)
{
	CreateDevice();
	CreateSwapChain();
}

void DX11_1Context::ClearBuffer(float r, float g, float b, float a)
{
	if(mIsFlipModelSupported)
		mContext->OMSetRenderTargets(1, mRtv.GetAddressOf(), nullptr);
	float rgba[] = { r,g,b,a };
	mContext->ClearRenderTargetView(mRtv.Get(), rgba);
	//mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DX11_1Context::SwapChain()
{
	mSwapChain->Present(0, 0);
}

void DX11_1Context::CreateDevice()
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	uint createFlags = 0;
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	LOG_HR << D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&device,
		nullptr,
		&context
	);

	device.As(&mDevice);
	context.As(&mContext);
}

void DX11_1Context::CreateSwapChain()
{
	ComPtr<IDXGIDevice1> dxgiDevice;
	LOG_HR << mDevice.As(&dxgiDevice);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	LOG_HR << dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());

	//swapchain flip model supported over 4version
	ComPtr<IDXGIFactory4> dxgiFactory;
	HRESULT hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory4),
		(void**)&dxgiFactory);

	if (SUCCEEDED(hr)) {
		mIsFlipModelSupported = true;
	}

	DXGI_SWAP_CHAIN_DESC1 swapChainInfo{};
	swapChainInfo.Width = mWidth;
	swapChainInfo.Height = mHeight;
	swapChainInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainInfo.BufferCount = 2;
	swapChainInfo.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;//DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainInfo.SampleDesc.Count = 1;
	swapChainInfo.SampleDesc.Quality = 0;

	LOG_HR << dxgiFactory->CreateSwapChainForHwnd(
		mDevice.Get(),
		mHwnd,
		&swapChainInfo,
		nullptr,
		nullptr,
		mSwapChain.ReleaseAndGetAddressOf()
	);

	ComPtr<ID3D11Texture2D> backBuffer;
	LOG_HR << mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	LOG_HR << mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRtv);

	CD3D11_VIEWPORT vp(0.f, 0.f, (float)mWidth, (float)mHeight);

	mContext->RSSetViewports(1, &vp);

	mContext->OMSetRenderTargets(1, mRtv.GetAddressOf(), nullptr);
}
