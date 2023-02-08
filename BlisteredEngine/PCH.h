#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#define SAFE_DELETE(x)	if(x)	{ delete x; x = nullptr;}
#define SAFE_RELEASE(x) if(x)	{ x->Release(); x = nullptr;}

#undef __PRETTY_FUNCTION__
#define __PF__ __FUNCSIG__					//func name


#include <Core\Type.h>
#include <Core\Log.h>

#include <memory>


//#ifdef _DEBUG
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#else
//#define new new
//#endif

