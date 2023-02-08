#pragma once

class Wnd
{
public:
	Wnd(Wnd* parent = nullptr);

	void Create(uint x, uint y, uint cx, uint cy);
	virtual void PreCreate(WNDCLASSEXT* wc);

	virtual void OnCreateEvent(uint cx, uint cy);
	virtual void OnSizeEvent(uint cx, uint cy);
	virtual void KeyPressEvent(uint keyCode);

	operator HWND() { return mHwnd; }

	virtual LRESULT LocalWndProc(UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
private:
	HWND mHwnd;
	Wnd* mParent;
};

