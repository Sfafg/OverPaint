#ifndef WAYLAND
#define GLFW_INCLUDE_NONE
#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>

extern "C" {
typedef struct VkInstance_T *VkInstance;
typedef struct VkSurfaceKHR_T *VkSurfaceKHR;
int glfwCreateWindowSurface(VkInstance instance, GLFWwindow *window, const void *allocator, VkSurfaceKHR *surface);
}

int Window::objectCounter = 0;
Window::Window() : pimpl(nullptr) {}
Window::Window(const char *name) {
    if (objectCounter == 0) glfwInit();
    objectCounter++;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    pimpl = glfwCreateWindow(mode->width, mode->height, name, nullptr, nullptr);
    glfwSetWindowUserPointer((GLFWwindow *)pimpl, this);
}
Window::~Window() {
    Close();

    if (pimpl == nullptr) return;

    objectCounter--;
    if (objectCounter == 0) glfwTerminate();
}

void Window::Close() { return glfwSetWindowShouldClose((GLFWwindow *)pimpl, 1); }
bool Window::IsOpen() const { return !glfwWindowShouldClose((GLFWwindow *)pimpl); }

glm::ivec2 Window::GetSize() const {
    glm::ivec2 s;
    glfwGetFramebufferSize((GLFWwindow *)pimpl, &s.x, &s.y);
    return s;
}

void Window::SetFramebufferResizeCallaback(void (*callback)(Window *window, glm::ivec2 size)) {
    frameBufferResizeCallback = callback;
    glfwSetFramebufferSizeCallback((GLFWwindow *)pimpl, [](GLFWwindow *w, int x, int y) {
        Window *window = (Window *)glfwGetWindowUserPointer(w);
        window->frameBufferResizeCallback(window, {x, y});
    });
}

vg::SurfaceHandle Window::CreateSurface() {
    vg::SurfaceHandle surface;
    glfwCreateWindowSurface(*(VkInstance *)vg::instance, (GLFWwindow *)pimpl, nullptr, (VkSurfaceKHR *)&surface);
    return surface;
}

void *Window::GetHandle() { return pimpl; }
#endif
