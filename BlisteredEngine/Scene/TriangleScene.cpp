#include <PCH.h>
#include <Scene\TriangleScene.h>
#include <Graphics\DX11_1Context.h>
#include <vector>
#include <fstream>

//std::vector<byte> LoadArray(LPCSTR code) {
//
//	std::vector<byte> data;
//	std::ifstream st(code, std::ios::in | std::ios::binary | std::ios::ate);
//
//
//	return nullptr;
//}

LPCSTR code = R"(
float4 VS(float4 pos : POSITION) : SV_POSITION
{
	return pos;
};

float4 PS() : SV_TARGET
{
	return float4(1.f,1.f,1.f,1.f);
};
)";

void Compile(LPCSTR src, LPCSTR entry, LPCSTR version, ID3DBlob** blob) {
	ID3DBlob* errBlob = nullptr;
	HRESULT res = D3DCompile(
		code,
		strlen(code),
		nullptr,
		nullptr,
		nullptr,
		entry,
		version,
		0,
		0,
		blob, &errBlob);

	if (FAILED(res))
	{
		if (!errBlob) {
			MessageBox(NULL, code, "failed no find filename", MB_OK);
			return;
		}
		MessageBox(NULL, (char*)errBlob->GetBufferPointer(), "compile error", MB_OK);
	}
	SAFE_RELEASE(errBlob);
}

TriangleScene::TriangleScene()
{
}

TriangleScene::~TriangleScene()
{
	SAFE_RELEASE(vs);
	SAFE_RELEASE(ps);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(vbo);
}

void TriangleScene::InitScene()
{
	ID3DBlob* vsCode;
	ID3DBlob* psCode;
	Compile(code, "VS", "vs_5_0", &vsCode);
	Compile(code, "PS", "ps_5_0", &psCode);
	
	LOG_HR << gDXDevice->CreateVertexShader(vsCode->GetBufferPointer(), 
		vsCode->GetBufferSize(), nullptr, &vs);

	LOG_HR << gDXDevice->CreatePixelShader(psCode->GetBufferPointer(),
		psCode->GetBufferSize(), nullptr, &ps);

	D3D11_INPUT_ELEMENT_DESC ed[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	LOG_HR << gDXDevice->CreateInputLayout(ed, ARRAYSIZE(ed),
		vsCode->GetBufferPointer(), vsCode->GetBufferSize(), &inputLayout);

	SAFE_RELEASE(vsCode);
	SAFE_RELEASE(psCode);

	float vtx[3][3] =
	{
		{ 0.0f, 0.5f, 0.0f },
		{ 0.45f, -0.5f, 0.0f },
		{ -0.45f, -0.5f, 0.0f }
	};

	D3D11_BUFFER_DESC bd{};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(vtx);

	bd.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vtx;
	LOG_HR << gDXDevice->CreateBuffer(&bd, &sd, &vbo);

	gDXContext->VSSetShader(vs, 0, 0);
	gDXContext->PSSetShader(ps, 0, 0);
	gDXContext->IASetInputLayout(inputLayout);
	gDXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	uint stride = sizeof(float) * 3;
	uint offset = 0;
	gDXContext->IASetVertexBuffers(0, 1, &vbo, &stride, &offset);
}

void TriangleScene::UpdateFrame(float delta)
{
	
}

void TriangleScene::RenderFrame()
{

	gDXContext->Draw(3, 0);
}
