#include "Core/Window.h"
#include "Core/Engine.h"

std::string DefineRoot(int amountOfArguments, char* arguments[]) {
	std::string root = std::string(arguments[0]);
	int slash0 = root.find_last_of('\\');
	int slash1 = root.find_last_of('/');	
	int slashIndex = (slash0 != std::string::npos ? slash0 : slash1);
	root = root.substr(0, slashIndex) + "/";

	return root;
}

int main(int amountOfArguments, char* arguments[]) {
	NATURAFORGE_CONSOLE_HIDE();
	NATURAFORGE_PROFILER_BEGIN("Forgio");

	NATURAFORGE_ROOT = DefineRoot(amountOfArguments, arguments);

	Engine engine;

	while (engine.IsRunning()) {
		NATURAFORGE_PROFILER_NAMED_SCOPE("New frame");

		engine.BeginFrame();
		engine.Render();
		engine.EndFrame();
	}
	
	NATURAFORGE_PROFILER_END();

	return 0;
}
