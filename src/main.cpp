#include "core/engine.h"

int main(int amountOfArguments, char *arguments[])
{
	mw::DefRoot(amountOfArguments, arguments);

	NF_CONSOLE_HIDE();
	NF_PROFILER_BEGIN("Forgio");

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
