#include <PCH.h>
#include <Graphics\DX11InputLayout.h>
#include <Graphics\DX11_1Context.h>
#include <string>

DX11InputLayout::DX11InputLayout()
{
	
}

DX11InputLayout::~DX11InputLayout()
{
	SAFE_RELEASE(layout);
}

void DX11InputLayout::Create(D3D11_INPUT_ELEMENT_DESC* desc, uint nDesc)
{
	std::string temp = R"(
	struct{ )";

	for (int i = 0; i < nDesc; ++i) {
		if (i == 0) {
			
		}
	}
}
