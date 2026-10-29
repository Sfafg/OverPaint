#include "Stylus.h"
#include "GLFW/glfw3.h"
#include <iostream>

void *Stylus::window = nullptr;
void GetStylusData(glm::vec2 *point, float *pressure) {
    double xpos, ypos;
    glfwGetCursorPos((GLFWwindow *)Stylus::window, &xpos, &ypos);
    point->x = xpos;
    point->y = ypos;
    *pressure = (glfwGetMouseButton((GLFWwindow *)Stylus::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) * 1000;
}

Stylus::Stylus() {}

void Stylus::GetData() {
    lastPoint = point;
    lastPressure = pressure;
    GetStylusData(&point, &pressure);
    direction = (lastPoint - point) * 0.9f + direction * 0.1f;
    bool isHovering = lastPoint != point || lastPressure != pressure;

    if (isHovering) notHoveringTimeout = 50;
    else if (notHoveringTimeout) --notHoveringTimeout;
}

glm::vec2 Stylus::GetPoint() const { return point * 2.f - 1.f; }
glm::vec2 Stylus::GetDirection() const { return direction; }
float Stylus::GetPressure() const { return pressure * 0.02f; }

bool Stylus::IsTouching() const { return pressure > 0; }
bool Stylus::IsHovering() const { return notHoveringTimeout != 0; }
bool Stylus::BeganTouching() const { return IsTouching() && lastPressure == 0; }
bool Stylus::EndedTouching() const { return !IsTouching() && lastPressure > 0; }
