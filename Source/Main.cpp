#include "Core/Window.h"

#include "Assets/TextAsset.h"

int main() {
    TextAsset asset("Assets/text.txt");
    LOG_OUT(asset.GetContent());

    Window window;

    while (!window.ShouldClose()) {
        window.PollEvents();

        window.SwapBuffers();
    }

    return 0;
}
