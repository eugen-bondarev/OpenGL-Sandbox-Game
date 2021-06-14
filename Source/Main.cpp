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
	FORGIO_PROFILER_BEGIN("Forgio");

	FORGIO_ROOT = DefineRoot(amountOfArguments, arguments);

	Engine engine;

	engine.InitResources();

	while (engine.IsRunning()) {
		FORGIO_PROFILER_NAMED_SCOPE("New frame");

		engine.BeginFrame();
		engine.Control();
		engine.Render();
		engine.EndFrame();
	}
	
	FORGIO_PROFILER_END();

	return 0;
}
