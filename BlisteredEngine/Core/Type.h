#pragma once

//Forward Declare
//Win32

#define CALLBACK		__stdcall
#define WINAPI			__stdcall
#define APIENTRY		WINAPI

typedef unsigned int		UINT;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef void				*LPVOID;
typedef const char*			LPCSTR;
typedef char*				LPSTR;

#if defined(_WIN64)
typedef __int64				INT_PTR;
typedef unsigned __int64	UINT_PTR;
typedef __int64				LONG_PTR;
typedef unsigned __int64	ULONG_PTR;
#else
typedef int					INT_PTR;
typedef unsigned int		UINT_PTR;
typedef long				LONG_PTR;
typedef unsigned long		ULONG_PTR;
#endif

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;
typedef long				HRESULT;

typedef void *HANDLE;
struct	HWND__;	typedef HWND__	*HWND;
struct	HINSTANCE__; typedef HINSTANCE__ *HINSTANCE;
struct	HDC__;   typedef HDC__	 *HDC;
struct	HGLRC__; typedef HGLRC__ *HGLRC;
struct	HICON__; typedef HICON__ *HICON;


struct WNDCLASSEXT;


typedef unsigned int	uint;
typedef unsigned char	byte;

//engine declare
class Context;
class DX11Context;
class DX11ContextExt;


//directx11
struct ID3D11Device;
struct ID3D11Device1;
struct ID3D11DeviceContext;
struct ID3D11DeviceContext1;

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;


//user declare
class Scene;
