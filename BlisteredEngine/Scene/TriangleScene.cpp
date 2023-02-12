#include <PCH.h>
#include <Scene\TriangleScene.h>
#include <Graphics\DX11_1Context.h>
#include <vector>
#include <fstream>
#include <Graphics\DX11Shader.h>
#include <Graphics\DX11InputLayout.h>
#include <Graphics\DX11Buffer.h>
#include <string>

LPCSTR code = R"(
struct vs_in{
	float3 pos : POSITION;
	float2 st : TEXCOORD0;
	float3 col : TEXCOORD1;
};

struct ps_in{
	float4 pos : SV_POSITION;
	float2 st : ST;
	float3 col : COLOR;
};

ps_in VS(vs_in vs)
{
	ps_in ps;
	ps.pos = float4(vs.pos,1);
	ps.st = vs.st;
	ps.col = vs.col;
	return ps;
};

float4 PS(ps_in ps) : SV_TARGET
{
	return float4(ps.col,1);
};
)";

TriangleScene::TriangleScene()
{
}

TriangleScene::~TriangleScene()
{
	SAFE_RELEASE(vs);
	SAFE_RELEASE(ps);
	SAFE_RELEASE(ilo);
	//SAFE_RELEASE(vbo);
}

void TriangleScene::InitScene()
{
	
	shader = std::make_unique<DX11Shader>();
	shader->CreateFromCode(code, code);


	Vertex vtx[] = {
		{vec3f{ 0.0f, 0.5f, 0.0f},vec2f{},vec3f{1,0,0} },
		{vec3f{ 0.45f, -0.5f, 0.0f},vec2f{},vec3f{0,1,0} },
		{vec3f{-0.45f, -0.5f, 0.0f},vec2f{},vec3f{0,0,1} }
	};

	//vbo = std::make_unique<DX11VertexBuffer>(sizeof(Vertex), sizeof(vtx), vtx);
	vbo = std::unique_ptr<DX11VertexBuffer>(new DX11VertexBuffer(
		sizeof(Vertex), sizeof(Vertex) * 3, vtx
	));

	vbo->BindPipeline(0);
	shader->BindPipeline();

	std::string dummy;

	dummy = R"(
	struct VS_In{
		float3 pos : POSITION;
		float2 param0 : TEXCOORD0;
		float3 nor : TEXCOORD1;
	};
	float4 VS(VS_In vs) : SV_POSITION
	{
		return vs.pos.xxxx;
	};
	)";

	ID3DBlob* blob;
	DX11Shader::CompileCode(dummy.c_str(), VS_ENTRY, VS_VERSION, &blob);

	D3D11_INPUT_ELEMENT_DESC info[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	LOG_HR << gDXDevice->CreateInputLayout(
		info, 3,
		blob->GetBufferPointer(), 
		blob->GetBufferSize(), 
		&ilo);
		

	gDXContext->IASetInputLayout(ilo);
	gDXContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
}

void TriangleScene::UpdateFrame(float delta)
{
	
}

void TriangleScene::RenderFrame()
{

	gDXContext->Draw(3, 0);
}
