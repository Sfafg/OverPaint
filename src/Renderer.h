#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "VG/VG.h"
#include "Vertex.h"
#include "Strokes.h"

class Renderer {
    static int objectCounter;
    Window *window;
    vg::Device renderDevice;
    vg::Queue queue;
    vg::Surface surface;
    vg::Swapchain swapchain;
    vg::Image colorImage;
    vg::Image depthImage;
    vg::ImageView colorImageView;
    vg::ImageView depthImageView;
    vg::RenderPass renderPass;
    std::vector<vg::Framebuffer> swapChainFramebuffers;
    std::vector<vg::CmdBuffer> commandBuffer;
    std::vector<vg::Semaphore> renderFinishedSemaphore, imageAvailableSemaphore;
    std::vector<vg::Fence> inFlightFence;
    uint32_t currentFrame = 0;
    uint32_t imageIndex = 0;

  public:
    Renderer(Window *window);
    ~Renderer();

    void StartFrame();
    void EndFrame();
    void Draw(const vg::Buffer &buffer, const vg::Buffer &indexBuffer, int indexCount);
    void Draw(const Stroke &stroke);
    void Present();
};
