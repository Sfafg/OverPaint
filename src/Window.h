#pragma once
#include "VG/VG.h"
#include <glm/glm.hpp>

class Window {
    static int objectCounter;
    void *pimpl;
    void (*frameBufferResizeCallback)(Window *window, glm::ivec2 size);

  public:
    Window();
    Window(const char *name);
    ~Window();

    void Close();
    bool IsOpen() const;
    glm::ivec2 GetSize() const;

    void SetFramebufferResizeCallaback(void (*callback)(Window *window, glm::ivec2 size));

    vg::SurfaceHandle CreateSurface();

    void *GetHandle();
};
