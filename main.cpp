#include <cstdlib>
#include <VrLib/Kernel.h>
#include "CellStrollApp.h"

int main(int argc, char* argv[])
{
	Kernel* kernel = Kernel::getInstance();		// Get the kernel
	CellStrollApp* app = new CellStrollApp();	// Instantiate an instance of the app

	// Load any config files specified on the command line
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--config") == 0)
		{
			i++;
			kernel->loadConfig(argv[i]);
		}
	}
	kernel->setApp(app);
	kernel->start();
	delete app;
	return 0;
}