#include <PCH.h>
#include <Graphics\DX11Shader.h>
#include <Graphics\DX11_1Context.h>

DX11Shader::DX11Shader()
{
}

DX11Shader::~DX11Shader()
{
	SAFE_RELEASE(mPS);
	SAFE_RELEASE(mVS);
	//SAFE_RELEASE(mByteBuffer);
}

void DX11Shader::CreateFromCode(LPCSTR vs, LPCSTR ps)
{
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;

	CompileCode(vs, VS_ENTRY, VS_VERSION, &vsBlob);
	CompileCode(ps, PS_ENTRY, PS_VERSION, &psBlob);

	LOG_HR <<  gDXDevice->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr, &mVS);

	LOG_HR << gDXDevice->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr, &mPS);

	mVertexBinayBuffer.resize(vsBlob->GetBufferSize());
	memcpy(mVertexBinayBuffer.data(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize());

	//using buffer_t = std::vector<byte>;
	//auto* p = (byte*)vsBlob->GetBufferPointer();
	//size_t n = vsBlob->GetBufferSize();
	//buffer_t buffer;
	//buffer.reserve(n);
	//std::copy(p, p + n, std::back_inserter(buffer));


	SAFE_RELEASE(vsBlob);
	SAFE_RELEASE(psBlob);
}

void DX11Shader::CompileCode(LPCSTR code, LPCSTR entry, LPCSTR version, ID3DBlob** blob)
{
	ID3DBlob* err;
	HRESULT hr = D3DCompile(
		code, strlen(code),
		nullptr, nullptr, nullptr,
		entry, version, 0, 0, blob, &err);
	if (FAILED(hr)) {
		MessageBox(nullptr, (LPCSTR)err->GetBufferPointer(), "Compile Error.", MB_OK);
	}
	SAFE_RELEASE(err);
}

void DX11Shader::BindPipeline()
{
	gDXContext->VSSetShader(mVS, 0, 0);
	gDXContext->PSSetShader(mPS, 0, 0);
}
