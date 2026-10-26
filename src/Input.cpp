#include "Input.h"
#include <GLFW/glfw3.h>

Window *Input::window = nullptr;
void (*Input::keyCallback)(Key key, bool pressed, vg::Flags<Modifier> mods) = nullptr;

void Input::SetKeyCallback(void (*callback)(Key key, bool pressed, vg::Flags<Modifier> mods)) {
    Input::keyCallback = callback;
    GLFWwindow *w = (GLFWwindow *)window->GetHandle();
    glfwSetKeyCallback(w, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        keyCallback(Key(key), action != GLFW_RELEASE, mods);
    });
}
