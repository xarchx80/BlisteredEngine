#pragma once

#include <Wnd/Wnd.h>

class WndFrame : public Wnd
{
public:
	WndFrame(Wnd* parent, uint width, uint height);
	WndFrame(Wnd* parent,uint x, uint y, uint width, uint height);
	
	void OnCreateEvent(uint cx, uint cy) override;
	void KeyPressEvent(uint keyCode) override;
	void PreCreate(WNDCLASSEXT* wc) override;
	

	int ExecProcedure(Scene* scene);

	std::shared_ptr<Context> mContext;
};

