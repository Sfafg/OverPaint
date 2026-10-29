#include <VG/VG.h>
#include <ranges>
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
        if (glfwGetKey((GLFWwindow *)window.GetHandle(), GLFW_KEY_ESCAPE)) window.Close();

        stylus.GetData();

        if (stylus.IsTouching()) { stroke->AddPoint(stylus.GetPoint(), stylus.GetDirection(), stylus.GetPressure()); }
        if (stylus.EndedTouching()) {
            UndoHistory::AddAction(new AddStroke(stroke));
            stroke = new Stroke(10, 11);
        }

        glm::mat4 viewProjection =
            glm::ortho(0.0, (double)window.GetSize().x * 2, 0.0, (double)window.GetSize().y * 2, -1.0, 1.0);

        renderer.StartFrame(viewProjection);
        renderer.Draw(*stroke);
        for (auto &s : Strokes::GetStrokes()) renderer.Draw(*s);
        renderer.EndFrame();
        renderer.Present();
    }

    return 0;
}
