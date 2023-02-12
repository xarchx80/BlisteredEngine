#pragma once

#include <Scene\Scene.h>

struct vec2f
{
	float x, y;
};

struct vec3f
{
	float x, y, z;
};

struct Vertex {
	vec3f pos;
	vec2f st;
	vec3f col;
};

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
	ID3D11InputLayout* ilo;
	//ID3D11Buffer* vbo;
<<<<<<< HEAD

=======
	uint size;
>>>>>>> main
	std::unique_ptr<DX11Shader> shader;
	std::unique_ptr<DX11Buffer> vbo;
};