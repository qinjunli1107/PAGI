#include <conio.h>
#include "System.h"

int main()
{
	System* engine = new System("3DS Loader and Renderer");
	if (!engine->Initialize())
	{
		printf("Press any key to shutdown\n");
		_getch();
		return -1;
	}
	engine->Run();
	engine->Shutdown();

	return 0;
}