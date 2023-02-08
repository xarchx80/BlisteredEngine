#include "PCH.h"
#include "Wnd.h"
#include <Windows.h>

struct WNDCLASSEXT : WNDCLASSEX
{
	DWORD dwStyle;
	HWND  parentHwnd;
	uint x;
	uint y;
	uint cx;
	uint cy;
};

Wnd::Wnd(Wnd* parent) : mParent(parent), mHwnd(nullptr)
{
	
}

void Wnd::Create(uint x, uint y, uint cx, uint cy)
{
	WNDCLASSEXT wc{};
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance     = GetModuleHandle(NULL);
	wc.style         = CS_VREDRAW | CS_HREDRAW;
	wc.lpszClassName = "Wnd++";
	wc.lpfnWndProc   = Wnd::WndProc;
	wc.dwStyle       = WS_VISIBLE;
	wc.parentHwnd	 = mParent ? mParent->mHwnd : nullptr;
	PreCreate(&wc);
	if (!GetClassInfoEx(wc.hInstance, wc.lpszClassName, &wc)) {
		if (!RegisterClassEx(&wc)) {
			LOG_ERROR("failed to register %s", wc.lpszClassName);
		}
	}
	if (wc.parentHwnd) {
		wc.dwStyle &= ~WS_POPUP;
		wc.dwStyle |= WS_CHILD;
	}
	else {
		wc.dwStyle &= ~WS_CHILD;
		wc.dwStyle |= WS_POPUP;
	}

	wc.x = x;
	wc.y = y;
	wc.cx = cx;
	wc.cy = cy;
	
	mHwnd = CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName,
		wc.dwStyle, wc.x, wc.y, wc.cx, wc.cy, wc.parentHwnd, 0,
		wc.hInstance, this);
	if (!mHwnd) {
		LOG_ERROR("failed to create %s window", wc.lpszClassName);
	}

}

void Wnd::PreCreate(WNDCLASSEXT* wc)
{
}

void Wnd::OnCreateEvent(uint cx, uint cy)
{
}

void Wnd::OnSizeEvent(uint cx, uint cy)
{
}

void Wnd::KeyPressEvent(uint keyCode)
{
}

LRESULT Wnd::LocalWndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE: {
		RECT rc{};
		GetClientRect(mHwnd, &rc);
		OnCreateEvent(rc.right - rc.left, rc.bottom - rc.top);
	}
	case WM_SIZE: {
		OnSizeEvent(LOWORD(lp), HIWORD(lp));
	}
	case WM_KEYDOWN: {
		KeyPressEvent((uint)wp);
	}
	default:
		break;
	}
	return DefWindowProc(mHwnd, msg, wp, lp);
}

LRESULT WINAPI Wnd::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Wnd* wnd = (Wnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (msg == WM_NCCREATE) {
		wnd = reinterpret_cast<Wnd*>(((LPCREATESTRUCT)lp)->lpCreateParams);
		wnd->mHwnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
	}

	if (wnd) return wnd->LocalWndProc(msg, wp, lp);
	return DefWindowProc(hwnd, msg, wp, lp);
}
