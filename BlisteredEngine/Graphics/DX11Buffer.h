#pragma once


struct DX11Buffer
{
	DX11Buffer();
	~DX11Buffer();
	virtual void BindPipeline(uint slot){}

	ID3D11Buffer* buffer;
	uint id;
};


struct DX11VertexBuffer : DX11Buffer
{
	DX11VertexBuffer(uint stride, uint cbSize, void* pData);

	void BindPipeline(uint slot) override;
	uint stride;
};