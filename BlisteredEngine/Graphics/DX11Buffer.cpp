#include <PCH.h>
#include <Graphics\DX11Buffer.h>
#include <Graphics\DX11_1Context.h>

DX11Buffer::DX11Buffer() : buffer(nullptr)
{
	static uint counter = 0;
	id = counter;
	counter++;
}

DX11Buffer::~DX11Buffer()
{
	SAFE_RELEASE(buffer);
}


DX11VertexBuffer::DX11VertexBuffer(uint stride, uint cbSize, void* pData)
	: stride(stride)
{
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = cbSize;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = pData;

	LOG_HR << gDXDevice->CreateBuffer(&bd, &sd, &buffer);
}

void DX11VertexBuffer::BindPipeline(uint slot)
{
	uint offset = 0;
	gDXContext->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
}

