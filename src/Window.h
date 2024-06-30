#pragma once
#include "VG/vg.h"
#include "Keys.h"
#include "BufferDraw.h"
#include <glm/glm.hpp>
#include <functional>

class Window
{
public:
    Window(const char* name, glm::ivec2 size);
    ~Window();

    void SetKeyCallback(std::function<void(Window*, Key, State, vg::Flags<Mods>)> keyCallback);
    void SetMouseMoveCallback(std::function<void(Window*, float, float)> mouseMoveCallback);

    void Draw(const vg::Buffer& buffer, int vertexByteOffset, int indexByteOffset, int indexType, int indexCount);
    void Draw(BufferDraw draw) { Draw(*draw.buffer, draw.vertexByteOffset, draw.indexByteOffset, draw.indexType, draw.indexCount); }
    void Present();

    void Close();
    bool ShouldClose() const;
    void AwaitAll();

protected:
    std::function<void(Window*, Key, State, vg::Flags<Mods>)> keyCallback;
    std::function<void(Window*, float, float)> mouseMoveCallback;
    bool recreateFramebuffer;
    bool hasDrawnInFrame = true;
    void* m_nativeWindow;
    int width;
    int height;
    int imageIndex;
    vg::Surface surface;
    vg::Swapchain swapchain;
    vg::Swapchain oldSwapchain;
    vg::RenderPass renderPass;
    std::vector<vg::Framebuffer> swapChainFramebuffers;
    vg::CommandBuffer commandBuffer;
    vg::Semaphore* renderFinishedSemaphore;
    vg::Semaphore* imageAvailableSemaphore;
    vg::Fence* inFlightFence;
};