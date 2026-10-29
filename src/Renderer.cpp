#include "Renderer.h"
#include "CmdBuffer.h"
#include "Enums.h"
#include "GraphicsPipeline.h"
#include "Structs.h"
#include "VG/../NamedResources.h"

using namespace vg;
using namespace vg::cmd;

VertexBinding &Vertex::GetBindingDescription() {
    static VertexBinding bindingDescription(0, sizeof(Vertex));

    return bindingDescription;
}

std::vector<VertexAttribute> &Vertex::GetAttributeDescriptions() {
    static std::vector<VertexAttribute> attributeDescriptions = {
        VertexAttribute(0, 0, Format::RG32SFLOAT, offsetof(Vertex, position)),
        VertexAttribute(1, 0, Format::RGB32SFLOAT, offsetof(Vertex, color)),
    };

    return attributeDescriptions;
}

bool recreateFramebuffer = false;

int Renderer::objectCounter = 0;
Renderer::Renderer(Window *window) : window(window) {
    objectCounter++;

    window->SetFramebufferResizeCallaback([](Window *window, glm::ivec2 size) { recreateFramebuffer = true; });
    int w = window->GetSize().x;
    int h = window->GetSize().y;

    SurfaceHandle windowSurface = window->CreateSurface();
    DeviceFeatures deviceFeatures(
        {Feature::WideLines, Feature::LogicOp, Feature::SamplerAnisotropy, Feature::SampleRateShading}
    );
    queue = Queue({QueueType::General}, 1.0f);
    renderDevice = Device(
        {&queue}, {"VK_KHR_swapchain"}, deviceFeatures, windowSurface,
        [](auto id, auto supportedQueues, auto supportedExtensions, auto type, DeviceLimits limits,
           DeviceFeatures features) { return (type == DeviceType::Integrated); }
    );
    vg::currentDevice = &renderDevice;

    surface = Surface(windowSurface, {Format::BGRA8SRGB, ColorSpace::SRGBNL});
    swapchain = Swapchain(surface, 2, w, h, Usage::ColorAttachment, PresentMode::Fifo, CompositeAlpha::PreMultiplied);

    depthImage = Image(
        {swapchain.GetWidth(), swapchain.GetHeight()},
        {Format::D32SFLOAT, Format::D32SFLOATS8UINT, Format::x8D24UNORMPACK}, {FormatFeature::DepthStencilAttachment},
        {ImageUsage::DepthStencilAttachment}
    );
    vg::Debug::SetName(depthImage, "depthImage");
    Allocate(Span<Image *const>{&depthImage}, {MemoryProperty::DeviceLocal});
    depthImageView = ImageView(depthImage, {ImageAspect::Depth});

    renderPass = RenderPass(
        {Attachment(surface.GetFormat(), ImageLayout::PresentSrc),
         Attachment(depthImage.GetFormat(), ImageLayout::DepthStencilAttachmentOptimal)},
        Vector<PipelineLayout>{PipelineLayout{{}, {PushConstantRange(ShaderStage::Vertex, 0, sizeof(glm::mat4))}}},
        {Subpass(
            GraphicsPipeline(
                0,
                {new Shader(ShaderStage::Vertex, "resources/shaders/shader.vert.spv"),
                 new Shader(ShaderStage::Fragment, "resources/shaders/shader.frag.spv")},
                VertexLayout({Vertex::GetBindingDescription()}, Vertex::GetAttributeDescriptions()),
                InputAssembly(Primitive::TriangleStrip, true), Tesselation(),
                ViewportState(Viewport(w, h), Scissor(w, h)),
                Rasterizer(false, false, PolygonMode::Fill, CullMode::None), Multisampling(),
                DepthStencil(true, true, CompareOp::Less),
                ColorBlending(
                    true, LogicOp::Copy, {0, 0, 0, 0},
                    {ColorBlend(
                        BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha, BlendOp::Add, BlendFactor::One,
                        BlendFactor::Zero, BlendOp::Add, ColorComponent::RGBA
                    )}
                ),
                {DynamicState::Viewport, DynamicState::Scissor}
            ),
            {}, {AttachmentReference(0, ImageLayout::ColorAttachmentOptimal)}, {},
            AttachmentReference(1, ImageLayout::DepthStencilAttachmentOptimal)
        )},
        {SubpassDependency(
            -1, 0, PipelineStage::ColorAttachmentOutput, PipelineStage::ColorAttachmentOutput, 0,
            Access::ColorAttachmentWrite, {}
        )}
    );

    swapChainFramebuffers.resize(swapchain.GetImageCount());
    for (int i = 0; i < swapchain.GetImageCount(); i++)
        swapChainFramebuffers[i] = Framebuffer(
            renderPass, {swapchain.GetImageViews()[i], depthImageView}, swapchain.GetWidth(), swapchain.GetHeight()
        );

    commandBuffer = std::vector<CmdBuffer>(swapchain.GetImageCount());
    renderFinishedSemaphore = std::vector<Semaphore>(swapchain.GetImageCount()),
    imageAvailableSemaphore = std::vector<Semaphore>(swapchain.GetImageCount());
    inFlightFence = std::vector<Fence>(swapchain.GetImageCount());
    for (int i = 0; i < swapchain.GetImageCount(); i++) {
        commandBuffer[i] = CmdBuffer(queue);
        renderFinishedSemaphore[i] = Semaphore();
        imageAvailableSemaphore[i] = Semaphore();
        inFlightFence[i] = Fence(true);
    }
}

Renderer::~Renderer() {
    if (window == nullptr) return;

    objectCounter--;
    if (objectCounter == 0) vg::currentDevice->WaitUntilIdle();
}

void Renderer::StartFrame(const glm::mat4 &viewProjection) {
    currentDevice = &renderDevice;
    inFlightFence[currentFrame].Await(true);

    Swapchain oldSwapchain;
    if (recreateFramebuffer) {
        currentDevice->WaitUntilIdle();
        recreateFramebuffer = false;
        int w = window->GetSize().x;
        int h = window->GetSize().y;

        std::swap(oldSwapchain, swapchain);
        swapchain = Swapchain(
            surface, 2, w, h, oldSwapchain, Usage::ColorAttachment, PresentMode::Fifo, CompositeAlpha::PreMultiplied
        );
        depthImage = Image(
            {swapchain.GetWidth(), swapchain.GetHeight()}, depthImage.GetFormat(), {ImageUsage::DepthStencilAttachment},
            1, 1, ImageTiling::Optimal, ImageLayout::Undefined
        );
        Allocate(depthImage, {MemoryProperty::DeviceLocal});
        depthImageView = ImageView(depthImage, {ImageAspect::Depth});
        for (int i = 0; i < swapchain.GetImageCount(); i++)
            swapChainFramebuffers[i] = Framebuffer(
                renderPass, {swapchain.GetImageViews()[i], depthImageView}, swapchain.GetWidth(), swapchain.GetHeight()
            );
    }

    auto [imInd, result] = swapchain.GetNextImageIndex(imageAvailableSemaphore[currentFrame]);
    imageIndex = imInd;

    commandBuffer[currentFrame].Clear().Begin().Append(
        cmd::BeginRenderpass(
            renderPass, swapChainFramebuffers[imageIndex], {0, 0}, {swapchain.GetWidth(), swapchain.GetHeight()},
            {ClearColor{0, 0, 0, 0}, ClearDepthStencil{1.0f, 0U}}, SubpassContents::Inline
        ),
        cmd::BindPipeline(renderPass.GetPipelines()[0]),
        cmd::SetViewport(Viewport(swapchain.GetWidth(), swapchain.GetHeight())),
        cmd::SetScissor(Scissor(swapchain.GetWidth(), swapchain.GetHeight())),
        cmd::PushConstants(renderPass.GetPipelineLayouts()[0], ShaderStage::Vertex, 0, viewProjection)
        // cmd::BindDescriptorSets(
        //     renderPass.GetPipelineLayouts()[0], PipelineBindPoint::Graphics, 0, {descriptorSets[imageIndex]}
        // ),
    );
}
void Renderer::EndFrame() {

    commandBuffer[currentFrame]
        .Append(cmd::EndRenderpass())
        .End()
        .Submit(
            {{PipelineStage::ColorAttachmentOutput, imageAvailableSemaphore[currentFrame]}},
            {renderFinishedSemaphore[currentFrame]}, inFlightFence[currentFrame]
        );
}
void Renderer::Draw(const vg::Buffer &buffer, const vg::Buffer &indexBuffer, int indexCount) {
    commandBuffer[currentFrame].Append(
        cmd::BindVertexBuffers(buffer, 0), cmd::BindIndexBuffer(indexBuffer, 0, IndexType::Uint16),
        cmd::DrawIndexed(indexCount)
    );
}

void Renderer::Draw(const Stroke &stroke) {
    commandBuffer[currentFrame].Append(
        cmd::BindVertexBuffers(stroke.VertexBuffer(), 0),
        cmd::BindIndexBuffer(stroke.IndexBuffer(), 0, IndexType::Uint16), cmd::DrawIndexed(stroke.IndexCount())
    );
}

void Renderer::Present() {
    queue.Present({renderFinishedSemaphore[currentFrame]}, {swapchain}, {imageIndex});
    currentFrame = (currentFrame + 1) % swapchain.GetImageCount();
}

struct Material {
    Subpass subpass;
    Material(Subpass &&subpass) : subpass(std::move(subpass)) {}
};
