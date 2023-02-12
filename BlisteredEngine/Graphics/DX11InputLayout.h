#pragma once

struct DX11InputLayout
{
	DX11InputLayout();
	~DX11InputLayout();

	void Create(D3D11_INPUT_ELEMENT_DESC* desc, uint nDesc);

	ID3D11InputLayout* layout;
};