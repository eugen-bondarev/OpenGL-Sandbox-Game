#include "Core/Window.h"
#include "Core/Engine.h"

int main() {
	FORGIO_PROFILER_BEGIN();

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
