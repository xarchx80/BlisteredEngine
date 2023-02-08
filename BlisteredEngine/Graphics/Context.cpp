#include <PCH.h>
#include <Graphics\Context.h>

Context* Context::gContext = nullptr;

Context::Context(uint w, uint h, HWND hwnd, bool vsync)
	: mWidth(w), mHeight(h), mHwnd(hwnd), mVsync(vsync), mEnableMsaa(false)
{
	if (!gContext) {
		gContext = this;
	}
	else LOG_ERROR("invalid multiple context");
}