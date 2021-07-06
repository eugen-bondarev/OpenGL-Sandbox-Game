#include "core/engine.h"

int main(int amountOfArguments, char *arguments[])
{
	mw::DefRoot(amountOfArguments, arguments);

	MW_CONSOLE_HIDE();
	MW_PROFILER_BEGIN("Forgio");

	Engine engine;

	while (engine.IsRunning())
	{
		MW_PROFILER_NAMED_SCOPE("New frame");

		engine.BeginFrame();
		engine.Run();
		engine.EndFrame();
	}

	MW_PROFILER_END();

	return 0;
}
