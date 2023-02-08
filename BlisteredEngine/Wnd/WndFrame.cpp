#include "PCH.h"
#include "WndFrame.h"
#include <Windows.h>
#include <Graphics\DX11Context.h>
#include <Graphics\DX11ContextExt.h>
#include <Graphics\DX11_1Context.h>
#include <Core\Timer.h>
#include <Scene\Scene.h>

WndFrame::WndFrame(Wnd* parent, uint width, uint height)
{
	uint x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	uint y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	Create(x, y, width, height);
}

WndFrame::WndFrame(Wnd* parent, uint x, uint y, uint width, uint height)
	: Wnd(parent)
{
	Create(x, y, width, height);
}

void WndFrame::OnCreateEvent(uint cx, uint cy)
{
	mContext = std::make_shared<DX11Context>(cx, cy, *this, false);
}

void WndFrame::KeyPressEvent(uint keyCode)
{
	if (keyCode == VK_ESCAPE)
		PostQuitMessage(0);
}

void WndFrame::PreCreate(WNDCLASSEXT* wc)
{
	
}

int WndFrame::ExecProcedure(Scene* scene)
{
	if (scene) scene->InitScene();
	Timer timer;
	timer.Reset();
	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		LRESULT res = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (res) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			timer.Begin();
			
			mContext->ClearBuffer(0.1f, 0.1f, 0.12f, 1.0f);
			if (scene) {

				scene->UpdateFrame(timer.GetDeltaTime());
				scene->RenderFrame();
			}
			mContext->SwapChain();
		}
	}
	return (int)msg.lParam;
}
