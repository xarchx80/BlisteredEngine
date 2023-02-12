#include <PCH.h>
#include <Graphics\DX11_1Context.h>

DX11_1Context::DX11_1Context(uint cx, uint cy, HWND hwnd, bool vsync)
	: Context(cx, cy, hwnd, vsync), mIsFlipModelSupported(false)
{
	CreateDevice();
	CreateSwapChain();
}

DX11_1Context::~DX11_1Context()
{
	
}

void DX11_1Context::ClearBuffer(float r, float g, float b, float a)
{
	if(mIsFlipModelSupported)
		mContext->OMSetRenderTargets(1, mRtv.GetAddressOf(), mDsv.Get());
	float rgba[] = { r,g,b,a };
	mContext->ClearRenderTargetView(mRtv.Get(), rgba);
	mContext->ClearDepthStencilView(mDsv.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DX11_1Context::SwapChain()
{
	HRESULT hr = mSwapChain->Present(mVsync ? 1 : 0, 0);
}

void DX11_1Context::CreateDevice()
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	uint createFlags = 0;
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLvls[] ={
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	uint nFeatureLvls = ARRAYSIZE(featureLvls);
	D3D_FEATURE_LEVEL featureLvl{};
	LOG_HR << D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		featureLvls,
		nFeatureLvls,
		D3D11_SDK_VERSION,
		&device,
		&featureLvl,
		&context
	);
	if (featureLvl == D3D_FEATURE_LEVEL_11_1) {
		printf("DirectX Device 11.1 Supported\n");
	}
	else {
		printf("DirectX Device 11.1 dosent supported!\n");
	}
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
	swapChainInfo.Width              = mWidth;
	swapChainInfo.Height             = mHeight;
	swapChainInfo.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainInfo.BufferCount        = 2;
	swapChainInfo.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainInfo.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;//DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainInfo.SampleDesc.Count   = 1;
	swapChainInfo.SampleDesc.Quality = 0;

	LOG_HR << dxgiFactory->CreateSwapChainForHwnd(
		mDevice.Get(),
		mHwnd,
		&swapChainInfo,
		nullptr,
		nullptr,
		mSwapChain.ReleaseAndGetAddressOf()
	);

	//create rendertargetview
	ComPtr<ID3D11Texture2D> backBuffer;
	LOG_HR << mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	LOG_HR << mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRtv.ReleaseAndGetAddressOf());
	//create depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = mWidth;
	depthStencilDesc.Height = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	LOG_HR << mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDsBuffer.GetAddressOf());
	LOG_HR << mDevice->CreateDepthStencilView(mDsBuffer.Get(), nullptr, mDsv.GetAddressOf());


	
	CD3D11_VIEWPORT vp(0.f, 0.f, (float)mWidth, (float)mHeight);
	mContext->RSSetViewports(1, &vp);

	mContext->OMSetRenderTargets(1, mRtv.GetAddressOf(), mDsv.Get());
}
