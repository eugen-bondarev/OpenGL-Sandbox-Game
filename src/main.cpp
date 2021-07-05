#include "core/window.h"
#include "core/engine.h"

#include <boxer/boxer.h>

std::string DefineRoot(int amountOfArguments, char *arguments[])
{
	std::string root = std::string(arguments[0]);
	int slash0 = root.find_last_of('\\');
	int slash1 = root.find_last_of('/');
	int slashIndex = (slash0 != std::string::npos ? slash0 : slash1);
	root = root.substr(0, slashIndex) + "/";

	return root;
}

int main(int amountOfArguments, char *arguments[])
{
	NF_CONSOLE_HIDE();
	NF_PROFILER_BEGIN("Forgio");

	NF_ROOT = DefineRoot(amountOfArguments, arguments);

	Engine engine;

	while (engine.IsRunning())
	{
		NF_PROFILER_NAMED_SCOPE("New frame");

		engine.BeginFrame();
		engine.Run();
		engine.EndFrame();
	}

	NF_PROFILER_END();

	return 0;
}
