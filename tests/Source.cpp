#include <VG/VG.h>
#define WAYLAND
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Stylus.h"
#include "Strokes.h"
#include "Window.h"
#include "Renderer.h"
#include "UndoHistory.h"
#include "Input.h"
#include <iostream>

#include <vulkan/vulkan.h>
void VulkanInit() {
    uint32_t glfwExtensionCount = 2;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    vg::instance =
        new vg::Instance({glfwExtensions, glfwExtensionCount}, [](vg::MessageSeverity severity, const char *message) {
            if (severity < vg::MessageSeverity::Error) return;
            std::cout << message << '\n' << '\n';
        });
};

int main() {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    Window window("OverPaint");
    VulkanInit();
    Input::window = &window;
    Stylus::window = window.GetHandle();

    Renderer renderer(&window);

    Input::SetKeyCallback([](Key key, bool pressed, vg::Flags<Modifier> mods) {
        if (!pressed) return;
        if (key == Key::R && mods.IsSet(Modifier::Ctrl)) UndoHistory::Redo();
        else if (key == Key::Z && mods.IsSet(Modifier::Ctrl)) UndoHistory::Undo();
    });

    Stylus stylus;
    Stroke *stroke = new Stroke(10, 11);
    while (window.IsOpen()) {
        glfwPollEvents();

        stylus.GetData();

        if (stylus.IsTouching()) { stroke->AddPoint(stylus.GetPoint(), stylus.GetDirection(), stylus.GetPressure()); }
        if (stylus.EndedTouching()) {
            UndoHistory::AddAction(new AddStroke(stroke));
            stroke = new Stroke(10, 11);
        }

        renderer.StartFrame();
        renderer.Draw(*stroke);
        for (auto &s : Strokes::GetStrokes()) renderer.Draw(*s);
        renderer.EndFrame();
        renderer.Present();
    }

    return 0;
}
