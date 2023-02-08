#include "PCH.h"
#include <Graphics\DX11ContextExt.h>

DX11ContextExt::DX11ContextExt(uint cx, uint cy, HWND hwnd, bool vsync)
	: mWidth(cx), mHeight(cy), mHwnd(hwnd), mVsync(vsync)
{
	CreateFactoryCapablilities();
	CreateContext();
	CreateSwapChain();
	CreateRenderTarget();
}

void DX11ContextExt::ClearBuffer(float r, float g, float b, float a)
{
	float rgba[4] = { r,g,b,a };
	mContext->ClearRenderTargetView(mRtv.Get(), rgba);
	mContext->ClearDepthStencilView(mDsv.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

}

void DX11ContextExt::SwapChain()
{
	HRESULT hr = 0;
	if (mAllowTearing)
		mSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
	else
		mSwapChain->Present(1, 0);
}

void DX11ContextExt::CreateFactoryCapablilities()
{
	LOG_HR << CreateDXGIFactory1(IID_PPV_ARGS(mDxgiFactory.ReleaseAndGetAddressOf()));

	BOOL allowTearing = FALSE;

	HRESULT hr = S_OK;
	ComPtr<IDXGIFactory5> factory5;
	hr = mDxgiFactory.As(&factory5);

	if (SUCCEEDED(hr)) {
		hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
			&allowTearing, sizeof(allowTearing));

		if (SUCCEEDED(hr)) {
			mAllowTearing = true;
			OutputDebugString("INFO : allow tearing supported\n");
		}
	}

	ComPtr<IDXGIFactory4> factory4;
	if (SUCCEEDED(mDxgiFactory.As(&factory4))) {
		mFlipDiscard = true;
		OutputDebugString("INFO : flip swap effects supported\n");
	}

	bool isDXGI6Supported = false;
#if defined(__dxgi1_6_h__)
	isDXGI6Supported = true;
#endif
	
}

void DX11ContextExt::CreateContext()
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	D3D_FEATURE_LEVEL features[] = { D3D_FEATURE_LEVEL_11_0 };

	uint create_flags = 0;
	ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(adapter.GetAddressOf());

#ifdef _DEBUG
	create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	LOG_HR << D3D11CreateDevice(
		adapter ? adapter.Get() : nullptr,
		adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_WARP,
		nullptr,
		create_flags,
		features,
		ARRAYSIZE(features),
		D3D11_SDK_VERSION,
		device.GetAddressOf(),
		&mFeaturesLevel,
		context.GetAddressOf());

	//LOG_HR << device->QueryInterface(__uuidof(ID3D11Device4), (void**)&mDevice);
	LOG_HR << device.As(&mDevice);
	//LOG_HR << context->QueryInterface(__uuidof(ID3D11DeviceContext4),(void**)&mContext.GetAddressOf());
	LOG_HR << context.As(&mContext);	
}

void DX11ContextExt::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = mWidth;
	swapChainDesc.Height = mHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect =  mFlipDiscard ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;//DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags = mAllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd{};
	scfd.Windowed = true;

	LOG_HR << mDxgiFactory->CreateSwapChainForHwnd(
		mDevice.Get(),
		mHwnd,
		&swapChainDesc,
		&scfd,
		nullptr,
		mSwapChain.ReleaseAndGetAddressOf()
	);
}

void DX11ContextExt::CreateRenderTarget()
{
	mSwapChain->GetBuffer(0, IID_PPV_ARGS(mBackBuffer.ReleaseAndGetAddressOf()));

	CD3D11_RENDER_TARGET_VIEW_DESC backBufferInfo(D3D11_RTV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R8G8B8A8_UNORM);
	
	LOG_HR << mDevice->CreateRenderTargetView(mBackBuffer.Get(),
		&backBufferInfo, mRtv.ReleaseAndGetAddressOf());

	CD3D11_TEXTURE2D_DESC depthstencilBackBufferInfo(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		mWidth,
		mHeight,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);
	LOG_HR << mDevice->CreateTexture2D(&depthstencilBackBufferInfo, nullptr,
		mDepthstencilBackBuffer.ReleaseAndGetAddressOf());


	CD3D11_DEPTH_STENCIL_VIEW_DESC depthstencilViewInfo(D3D11_DSV_DIMENSION_TEXTURE2D);

	LOG_HR << mDevice->CreateDepthStencilView(mDepthstencilBackBuffer.Get(),
		&depthstencilViewInfo, mDsv.GetAddressOf());

	mContext->OMSetRenderTargets(1, mRtv.GetAddressOf(), mDsv.Get());

	CD3D11_VIEWPORT vp(0.f, 0.f, (float)mWidth, (float)mHeight);
	mContext->RSSetViewports(1, &vp);


}

void DX11ContextExt::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;

#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
	ComPtr<IDXGIFactory6> factory6;
	HRESULT hr = mDxgiFactory.As(&factory6);
	if (SUCCEEDED(hr))
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(
				adapterIndex,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			LOG_HR << adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}

#ifdef _DEBUG
			wchar_t buff[256] = {};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif

			break;
		}
	}
#endif
	if (!adapter)
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(mDxgiFactory->EnumAdapters1(
				adapterIndex,
				adapter.ReleaseAndGetAddressOf()));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			LOG_HR << adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}

#ifdef _DEBUG
			wchar_t buff[256] = {};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif

			break;
		}
	}

	*ppAdapter = adapter.Detach();
}