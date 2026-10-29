#include <VG/VG.h>
#include <ranges>
#define WAYLAND
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include "Stylus.h"
#include "Strokes.h"
#include "Window.h"
#include "Renderer.h"
#include "UndoHistory.h"
#include "Input.h"
#include <iostream>

void VulkanInit()
{
    uint32_t glfwExtensionCount = 2;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    vg::instance =
        new vg::Instance({ glfwExtensions, glfwExtensionCount }, [](vg::MessageSeverity severity, const char* message) {
        if (severity < vg::MessageSeverity::Error) return;
        std::cout << message << '\n' << '\n';
            });
};

int main()
{
    Window window("OverPaint");
    VulkanInit();
    Input::window = &window;

    Renderer renderer(&window);

    Input::SetKeyCallback([](Key key, bool pressed, vg::Flags<Modifier> mods) {
        if (!pressed) return;
        if (key == Key::R && mods.IsSet(Modifier::Ctrl)) UndoHistory::Redo();
        else if (key == Key::Z && mods.IsSet(Modifier::Ctrl)) UndoHistory::Undo();
        });

    Stylus stylus(glfwGetWin32Window(
        static_cast<GLFWwindow*>(window.GetHandle())));
    Stroke* stroke = new Stroke(10, 11);
    while (window.IsOpen())
    {
        glfwPollEvents();
        if (glfwGetKey((GLFWwindow*) window.GetHandle(), GLFW_KEY_ESCAPE)) window.Close();
        if (glfwGetKey((GLFWwindow*) window.GetHandle(), GLFW_KEY_ESCAPE)) window.Close();
        stylus.GetData();

        if (stylus.IsTouching())
        {
            int windowWidth;
            int windowHeight;
            glfwGetWindowSize(
                static_cast<GLFWwindow*>(window.GetHandle()),
                &windowWidth, &windowHeight);

            glm::ivec2 framebufferSize = window.GetSize();
            if (windowWidth > 0 && windowHeight > 0)
            {
                glm::vec2 scale(
                    static_cast<float>(framebufferSize.x) / windowWidth,
                    static_cast<float>(framebufferSize.y) / windowHeight);
                float pressure = stylus.GetPressure();
                float thickness = pressure > 0.0f ? pressure * 2.0f : 1.0f;
                stroke->AddPoint(
                    stylus.GetPoint() * scale,
                    stylus.GetDirection() * scale,
                    thickness);
            }
        }
        if (stylus.EndedTouching())
        {
            UndoHistory::AddAction(new AddStroke(stroke));
            stroke = new Stroke(10, 11);
        }

        glm::ivec2 framebufferSize = window.GetSize();
        glm::mat4 viewProjection = glm::ortho(
            0.0, (double) framebufferSize.x,
            0.0, (double) framebufferSize.y,
            -1.0, 1.0);

        renderer.StartFrame(viewProjection);
        renderer.Draw(*stroke);
        for (auto& s : Strokes::GetStrokes()) renderer.Draw(*s);
        renderer.EndFrame();
        renderer.Present();
    }

    return 0;
}
