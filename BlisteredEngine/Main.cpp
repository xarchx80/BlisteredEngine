#include <PCH.h>
#include <stdlib.h>
#include <Wnd\WndFrame.h>
#include <Scene\TriangleScene.h>
int main(int args, char* argv[])
{

	WndFrame frame(nullptr, 1024, 680);
	
	TriangleScene scene0;
	//system("pause");
	
	return frame.ExecProcedure(&scene0);
}