#include "Core/Window.h"

int main() {
    Window window;

    while (!window.ShouldClose()) {
        window.PollEvents();

        window.SwapBuffers();
    }

    return 0;
}
