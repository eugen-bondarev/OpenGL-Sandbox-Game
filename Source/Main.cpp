#include "Core/Window.h"
#include "Core/Engine.h"

int main() {
    Engine engine;

    engine.InitResources();

    while (engine.IsRunning()) {
        engine.BeginFrame();
        engine.Control();
        engine.Render();
        engine.EndFrame();
    }

    return 0;
}
