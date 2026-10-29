#pragma once
#include <glm/glm.hpp>
#include <windows.h>
#include <mutex>

class Stylus
{
  HWND window = nullptr;
  glm::vec2 point;
  glm::vec2 lastPoint;
  glm::vec2 direction;
  float pressure;
  float lastPressure;
  int notHoveringTimeout = 0;

  bool hovering = false;
  bool touching = false;
  bool beganTouching = false;
  bool endedTouching = false;

  mutable std::mutex mutex;

  bool pendingBegin = false;
  bool pendingEnd = false;
  bool pendingHovering = false;
  bool pendingTouching = false;

  static LRESULT CALLBACK WindowProc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
    UINT_PTR subclassId, DWORD_PTR refData);

public:

  explicit Stylus(HWND window);
  ~Stylus();

  void GetData();

  glm::vec2 GetPoint() const;
  glm::vec2 GetDirection() const;
  float GetPressure() const;

  bool IsHovering() const;
  bool IsTouching() const;
  bool BeganTouching() const;
  bool EndedTouching() const;
};
