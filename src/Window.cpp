#include "Window.h"
#include "Vertex.h"
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(const char* name, glm::ivec2 size) : recreateFramebuffer(false)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    m_nativeWindow = glfwCreateWindow(size.x, size.y, name, nullptr, nullptr);

    glfwSetWindowUserPointer((GLFWwindow*) m_nativeWindow, this);
    glfwSetFramebufferSizeCallback((GLFWwindow*) m_nativeWindow, [](GLFWwindow* window, int width, int height) {
        Window* w = (Window*) glfwGetWindowUserPointer(window);
        w->recreateFramebuffer = true;
        w->width = width;
        w->height = height;
        });
    glfwSetKeyCallback((GLFWwindow*) m_nativeWindow, [](GLFWwindow* window, int key, int code, int action, int mods) {
        Window* w = (Window*) glfwGetWindowUserPointer(window);
        if (w->keyCallback != nullptr)w->keyCallback(w, (Key) key, (State) action, vg::Flags<Mods>(mods));
        });
    glfwSetCursorPosCallback((GLFWwindow*) m_nativeWindow, [](GLFWwindow* window, double x, double y) {
        Window* w = (Window*) glfwGetWindowUserPointer(window);
        if (w->mouseMoveCallback != nullptr)w->mouseMoveCallback(w, x, y);
        });

    glfwGetFramebufferSize((GLFWwindow*) m_nativeWindow, &width, &height);

    vg::instance = vg::Instance({ "VK_KHR_surface", "VK_KHR_win32_surface" },
        [](vg::Debug::Severity severity, const char* message) {
            if (severity < vg::Debug::Severity::Warning) return;
            std::cout << message << '\n' << '\n';
        });

    vg::SurfaceHandle windowSurface = vg::Window::CreateWindowSurface(vg::instance, (GLFWwindow*) m_nativeWindow);
    vg::currentDevice = vg::Device({ vg::Queue::Type::Graphics, vg::Queue::Type::Present, vg::Queue::Type::Transfer }, { "VK_KHR_swapchain", "VK_EXT_line_rasterization" }, windowSurface,
        [](auto supportedQueues, auto supportedExtensions, auto type, vg::Limits limits) {
            return type == vg::Device::Type::Integrated;
        });
    surface = vg::Surface(windowSurface, vg::Format::BGRA8SRGB, vg::ColorSpace::SRGBNL);


    vg::Shader vertexShader(vg::ShaderStage::Vertex, "C:/Projekty/Vulkan/OverPaint/src/shaders/shaderVert.spv");
    vg::Shader fragmentShader(vg::ShaderStage::Fragment, "C:/Projekty/Vulkan/OverPaint/src/shaders/shaderFrag.spv");

    renderPass = vg::RenderPass(
        {
            vg::Attachment(surface.GetFormat(), vg::ImageLayout::PresentSrc)
        },
        {
            vg::Subpass(
                vg::GraphicsPipeline{
                    {&vertexShader, &fragmentShader},
                    vg::VertexLayout(1U,&Vertex::getBindingDescription(),Vertex::getAttributeDescriptions().size(),Vertex::getAttributeDescriptions().data()),
                    vg::InputAssembly(vg::Primitive::TriangleStrip, true),
                    vg::Tesselation(),
                    vg::ViewportState(vg::Viewport(width, height), vg::Scissor(width, height)),
                    vg::Rasterizer(true, false, vg::PolygonMode::Fill, vg::CullMode::None, vg::FrontFace::Clockwise, vg::DepthBias(), 2.0f),
                    vg::Multisampling(),
                    vg::DepthStencil(),
                    vg::ColorBlending(true, vg::LogicOp::Copy, { 0,0,0,0 }),
                    { vg::DynamicState::Viewport, vg::DynamicState::Scissor },
                    {}
                },
                {},
                { vg::AttachmentReference(0, vg::ImageLayout::ColorAttachmentOptimal) }
            )
        },
        {}
    );
    swapchain = vg::Swapchain(surface, 2, width, height);
    swapChainFramebuffers = swapchain.CreateFramebuffers(renderPass);
    commandBuffer = vg::CommandBuffer(vg::currentDevice.graphicsQueue);
    renderFinishedSemaphore = new vg::Semaphore();
    imageAvailableSemaphore = new vg::Semaphore();
    inFlightFence = new vg::Fence(true);
    imageIndex = -1;
}

Window::~Window()
{
    delete renderFinishedSemaphore;
    delete imageAvailableSemaphore;
    delete inFlightFence;
    glfwTerminate();
}

void Window::SetKeyCallback(std::function<void(Window*, Key, State, vg::Flags<Mods>)> keyCallback)
{
    this->keyCallback = keyCallback;
}
void Window::SetMouseMoveCallback(std::function<void(Window*, float, float)> mouseMoveCallback)
{
    this->mouseMoveCallback = mouseMoveCallback;
}

void Window::Draw(const vg::Buffer& buffer, int vertexByteOffset, int indexByteOffset, int indexType, int indexCount)
{
    if (imageIndex == -1)
    {
        hasDrawnInFrame = true;

        // Swapchain resize.
        if (recreateFramebuffer)
        {
            std::swap(oldSwapchain, swapchain);
            swapchain = vg::Swapchain(surface, 2, width, height, vg::Usage::ColorAttachment, vg::PresentMode::Fifo, vg::CompositeAlpha::Opaque, oldSwapchain);
            swapChainFramebuffers = swapchain.CreateFramebuffers(renderPass);
            recreateFramebuffer = false;
        }

        imageIndex = swapchain.GetNextImageIndex(*imageAvailableSemaphore);

        commandBuffer.Begin();
        commandBuffer.AppendNoBegin(
            vg::cmd::SetViewport(vg::Viewport(swapchain.GetWidth(), swapchain.GetHeight())),
            vg::cmd::SetScissor(vg::Scissor(swapchain.GetWidth(), swapchain.GetHeight())),
            vg::cmd::BeginRenderpass(renderPass, swapChainFramebuffers[imageIndex], 0, 0, swapchain.GetWidth(), swapchain.GetHeight()),
            vg::cmd::BindPipeline(renderPass.m_graphicsPipelines[0]),
            vg::cmd::BindVertexBuffer(buffer, vertexByteOffset),
            vg::cmd::BindIndexBuffer(buffer, indexByteOffset, indexType),
            vg::cmd::DrawIndexed(indexCount)
        );
    }
    else
    {
        commandBuffer.AppendNoBegin(
            vg::cmd::BindVertexBuffer(buffer, vertexByteOffset),
            vg::cmd::BindIndexBuffer(buffer, indexByteOffset, indexType),
            vg::cmd::DrawIndexed(indexCount)
        );
    }

}
void Window::Present()
{
    glfwPollEvents();
    if (glfwGetKey((GLFWwindow*) m_nativeWindow, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose((GLFWwindow*) m_nativeWindow, true);

    if (!hasDrawnInFrame || width == 0 || height == 0)
    {
        commandBuffer.Clear();
        return;
    }

    commandBuffer.AppendNoBegin(
        vg::cmd::EndRenderpass()
    );
    commandBuffer.End();

    commandBuffer.Submit({ {{ {vg::PipelineStage::ColorAttachmentOutput, *imageAvailableSemaphore} },{ commandBuffer },{ *renderFinishedSemaphore }} }, *inFlightFence);
    vg::currentDevice.presentQueue.Present({ *renderFinishedSemaphore }, { swapchain }, { (unsigned int) imageIndex });

    imageIndex = -1;
    if (!((vg::SwapchainHandle) oldSwapchain == nullptr))
    {
        vg::Swapchain sp;
        std::swap(sp, oldSwapchain);
    }
}

void Window::Close()
{
    glfwSetWindowShouldClose((GLFWwindow*) m_nativeWindow, GLFW_TRUE);
}
bool Window::ShouldClose() const
{
    return glfwWindowShouldClose((GLFWwindow*) m_nativeWindow);
}
void Window::AwaitAll()
{
    if (hasDrawnInFrame)
    {
        vg::Fence::AwaitAll({ *inFlightFence }, true);
        hasDrawnInFrame = false;
    }
}
