#pragma once

#include <Scene\Scene.h>

class TriangleScene : public Scene
{
public:
	TriangleScene();
	~TriangleScene();

	void InitScene();
	void UpdateFrame(float delta);
	void RenderFrame();
private:
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vbo;
};