#pragma once
#include <glm/glm.hpp>

class Stylus {
    glm::vec2 point;
    glm::vec2 lastPoint;
    glm::vec2 direction;
    float pressure;
    float lastPressure;
    int notHoveringTimeout = 0;

  public:
    static void *window;

    Stylus();

    void GetData();

    glm::vec2 GetPoint() const;
    glm::vec2 GetDirection() const;
    float GetPressure() const;

    bool IsHovering() const;
    bool IsTouching() const;
    bool BeganTouching() const;
    bool EndedTouching() const;
};
