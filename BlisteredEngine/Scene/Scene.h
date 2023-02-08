#pragma once

class Scene {
public:
	Scene(){}
	virtual~Scene(){}
	virtual void InitScene() = 0;
	virtual void UpdateFrame(float delta) = 0;
	virtual void RenderFrame() = 0;
};