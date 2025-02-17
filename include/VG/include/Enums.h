#pragma once

namespace vg
{
    enum class PresentMode
    {
        Immediate = 0,
        Mailbox = 1,
        Fifo = 2,
        FifoRelaxed = 3,
        SharedDemandRefresh = 1000111000,
        SharedContinuousRefresh = 1000111001,
    };
    enum class CompositeAlpha
    {
        Opaque = 0x00000001,
        PreMultiplied = 0x00000002,
        PostMultiplied = 0x00000004,
        Inherit = 0x00000008
    };
    enum class Usage
    {
        TransferSrc = 0x00000001,
        TransferDst = 0x00000002,
        Sampled = 0x00000004,
        Storage = 0x00000008,
        ColorAttachment = 0x00000010,
        DepthStencilAttachment = 0x00000020,
        TransientAttachment = 0x00000040,
        InputAttachment = 0x00000080,
        VideoDecodeDst = 0x00000400,
        VideoDecodeSrc = 0x00000800,
        VideoDecodeDpb = 0x00001000,
        FragmentDensityMap = 0x00000200,
        FragmentShadingRateAttachment = 0x00000100,
        HostTransfer = 0x00400000,
        VideoEncodeDst = 0x00002000,
        VideoEncodeSrc = 0x00004000,
        VideoEncodeDpb = 0x00008000,
        AttachmentFeedbackLoop = 0x00080000,
        InvocationMask = 0x00040000,
        SampleWeight = 0x00100000,
        SampleBlockMatch = 0x00200000,
    };
    enum class Format
    {
        Undefined = 0,
        RG4UNORMPACKED = 1,
        RGBA4UNORMPACK = 2,
        BGRA4UNORMPACK = 3,
        R5G6B5UNORMPACK = 4,
        B5G6R5UNORMPACK = 5,
        RGB5A1UNORMPACK = 6,
        BGR5A1UNORMPACK = 7,
        A1RGB5UNORMPACK = 8,
        R8UNORM = 9,
        R8SNORM = 10,
        R8USCALED = 11,
        R8SSCALED = 12,
        R8UINT = 13,
        R8SINT = 14,
        R8SRGB = 15,
        RG8UNORM = 16,
        RG8SNORM = 17,
        RG8USCALED = 18,
        RG8SSCALED = 19,
        RG8UINT = 20,
        RG8SINT = 21,
        RG8SRGB = 22,
        RGB8UNORM = 23,
        RGB8SNORM = 24,
        RGB8USCALED = 25,
        RGB8SSCALED = 26,
        RGB8UINT = 27,
        RGB8SINT = 28,
        RGB8SRGB = 29,
        BGR8UNORM = 30,
        BGR8SNORM = 31,
        BGR8USCALED = 32,
        BGR8SSCALED = 33,
        BGR8UINT = 34,
        BGR8SINT = 35,
        BGR8SRGB = 36,
        RGBA8UNORM = 37,
        RGBA8SNORM = 38,
        RGBA8USCALED = 39,
        RGBA8SSCALED = 40,
        RGBA8UINT = 41,
        RGBA8SINT = 42,
        RGBA8SRGB = 43,
        BGRA8UNORM = 44,
        BGRA8SNORM = 45,
        BGRA8USCALED = 46,
        BGRA8SSCALED = 47,
        BGRA8UINT = 48,
        BGRA8SINT = 49,
        BGRA8SRGB = 50,
        ABGR8UNORMPACK = 51,
        ABGR8SNORMPACK = 52,
        ABGR8USCALEDPACK = 53,
        ABGR8SSCALEDPACK = 54,
        ABGR8UINTPACK = 55,
        ABGR8SINTPACK = 56,
        ABGR8SRGBPACK = 57,
        A2RGB10UNORMPACK = 58,
        A2RGB10SNORMPACK = 59,
        A2RGB10USCALEDPACK = 60,
        A2RGB10SSCALEDPACK = 61,
        A2RGB10UINTPACK = 62,
        A2RGB10SINTPACK = 63,
        A2BGR10UNORMPACK = 64,
        A2BGR10SNORMPACK = 65,
        A2BGR10USCALEDPACK = 66,
        A2BGR10SSCALEDPACK = 67,
        A2BGR10UINTPACK = 68,
        A2BGR10SINTPACK = 69,
        R16UNORM = 70,
        R16SNORM = 71,
        R16USCALED = 72,
        R16SSCALED = 73,
        R16UINT = 74,
        R16SINT = 75,
        R16SFLOAT = 76,
        RG16UNORM = 77,
        RG16SNORM = 78,
        RG16USCALED = 79,
        RG16SSCALED = 80,
        RG16UINT = 81,
        RG16SINT = 82,
        RG16SFLOAT = 83,
        RGB16UNORM = 84,
        RGB16SNORM = 85,
        RGB16USCALED = 86,
        RGB16SSCALED = 87,
        RGB16UINT = 88,
        RGB16SINT = 89,
        RGB16SFLOAT = 90,
        RGBA16UNORM = 91,
        RGBA16SNORM = 92,
        RGBA16USCALED = 93,
        RGBA16SSCALED = 94,
        RGBA16UINT = 95,
        RGBA16SINT = 96,
        RGBA16SFLOAT = 97,
        R32UINT = 98,
        R32SINT = 99,
        R32SFLOAT = 100,
        RG32UINT = 101,
        RG32SINT = 102,
        RG32SFLOAT = 103,
        RGB32UINT = 104,
        RGB32SINT = 105,
        RGB32SFLOAT = 106,
        RGBA32UINT = 107,
        RGBA32SINT = 108,
        RGBA32SFLOAT = 109,
        R64UINT = 110,
        R64SINT = 111,
        R64SFLOAT = 112,
        RG64UINT = 113,
        RG64SINT = 114,
        RG64SFLOAT = 115,
        RGB64UINT = 116,
        RGB64SINT = 117,
        RGB64SFLOAT = 118,
        RGBA64UINT = 119,
        RGBA64SINT = 120,
        RGBA64SFLOAT = 121,
        B10GR11UFLOATPACK = 122,
        E5BGR9UFLOATPACK = 123,
        D16UNORM = 124,
        x8D24UNORMPACK = 125,
        D32SFLOAT = 126,
        S8UINT = 127,
        D16UNORMS8UINT = 128,
        D24UNORMS8UINT = 129,
        D32SFLOATS8UINT = 130,
        BC1RGBUNORMBLOCK = 131,
        BC1RGBSRGBBLOCK = 132,
        BC1RGBAUNORMBLOCK = 133,
        BC1RGBASRGBBLOCK = 134,
        BC2UNORMBLOCK = 135,
        BC2SRGBBLOCK = 136,
        BC3UNORMBLOCK = 137,
        BC3SRGBBLOCK = 138,
        BC4UNORMBLOCK = 139,
        BC4SNORMBLOCK = 140,
        BC5UNORMBLOCK = 141,
        BC5SNORMBLOCK = 142,
        BC6HUFLOATBLOCK = 143,
        BC6HSFLOATBLOCK = 144,
        BC7UNORMBLOCK = 145,
        BC7SRGBBLOCK = 146,
        ETC2RGB8UNORMBLOCK = 147,
        ETC2RGB8SRGBBLOCK = 148,
        ETC2RGB8A1UNORMBLOCK = 149,
        ETC2RGB8A1SRGBBLOCK = 150,
        ETC2RGBA8UNORMBLOCK = 151,
        ETC2RGBA8SRGBBLOCK = 152,
        EACR11UNORMBLOCK = 153,
        EACR11SNORMBLOCK = 154,
        EACRG11UNORMBLOCK = 155,
        EACRG11SNORMBLOCK = 156,
        ASTC4x4UNORMBLOCK = 157,
        ASTC4x4SRGBBLOCK = 158,
        ASTC5x4UNORMBLOCK = 159,
        ASTC5x4SRGBBLOCK = 160,
        ASTC5x5UNORMBLOCK = 161,
        ASTC5x5SRGBBLOCK = 162,
        ASTC6x5UNORMBLOCK = 163,
        ASTC6x5SRGBBLOCK = 164,
        ASTC6x6UNORMBLOCK = 165,
        ASTC6x6SRGBBLOCK = 166,
        ASTC8x5UNORMBLOCK = 167,
        ASTC8x5SRGBBLOCK = 168,
        ASTC8x6UNORMBLOCK = 169,
        ASTC8x6SRGBBLOCK = 170,
        ASTC8x8UNORMBLOCK = 171,
        ASTC8x8SRGBBLOCK = 172,
        ASTC10x5UNORMBLOCK = 173,
        ASTC10x5SRGBBLOCK = 174,
        ASTC10x6UNORMBLOCK = 175,
        ASTC10x6SRGBBLOCK = 176,
        ASTC10x8UNORMBLOCK = 177,
        ASTC10x8SRGBBLOCK = 178,
        ASTC10x10UNORMBLOCK = 179,
        ASTC10x10SRGBBLOCK = 180,
        ASTC12x10UNORMBLOCK = 181,
        ASTC12x10SRGBBLOCK = 182,
        ASTC12x12UNORMBLOCK = 183,
        ASTC12x12SRGBBLOCK = 184,
        GBGR8422UNORM = 1000156000,
        BGRG8422UNORM = 1000156001,
        GBR83PLANE420UNORM = 1000156002,
        GBR82PLANE420UNORM = 1000156003,
        GBR83PLANE422UNORM = 1000156004,
        GBR82PLANE422UNORM = 1000156005,
        GBR83PLANE444UNORM = 1000156006,
        R10x6UNORMPACK16 = 1000156007,
        R10x6G10x6UNORM2PACK16 = 1000156008,
        R10x6G10x6B10x6A10x6UNORM4PACK16 = 1000156009,
        G10x6B10x6G10x6R10x6422UNORM4PACK16 = 1000156010,
        B10x6G10x6R10x6G10x6422UNORM4PACK16 = 1000156011,
        G10x6B10x6R10x63PLANE420UNORM3PACK16 = 1000156012,
        G10x6B10x6R10x62PLANE420UNORM3PACK16 = 1000156013,
        G10x6B10x6R10x63PLANE422UNORM3PACK16 = 1000156014,
        G10x6B10x6R10x62PLANE422UNORM3PACK16 = 1000156015,
        G10x6B10x6R10x63PLANE444UNORM3PACK16 = 1000156016,
        R12x4UNORMPACK16 = 1000156017,
        R12x4G12x4UNORM2PACK16 = 1000156018,
        R12x4G12x4B12x4A12x4UNORM4PACK16 = 1000156019,
        G12x4B12x4G12x4R12x4422UNORM4PACK16 = 1000156020,
        B12x4G12x4R12x4G12x4422UNORM4PACK16 = 1000156021,
        G12x4B12x4R12x43PLANE420UNORM3PACK16 = 1000156022,
        G12x4B12x4R12x42PLANE420UNORM3PACK16 = 1000156023,
        G12x4B12x4R12x43PLANE422UNORM3PACK16 = 1000156024,
        G12x4B12x4R12x42PLANE422UNORM3PACK16 = 1000156025,
        G12x4B12x4R12x43PLANE444UNORM3PACK16 = 1000156026,
        GBGR16422UNORM = 1000156027,
        BGRG16422UNORM = 1000156028,
        GBR163PLANE420UNORM = 1000156029,
        GBR162PLANE420UNORM = 1000156030,
        GBR163PLANE422UNORM = 1000156031,
        GBR162PLANE422UNORM = 1000156032,
        GBR163PLANE444UNORM = 1000156033,
        GBR82PLANE444UNORM = 1000330000,
        G10x6B10x6R10x62PLANE444UNORM3PACK16 = 1000330001,
        G12x4B12x4R12x42PLANE444UNORM3PACK16 = 1000330002,
        GBR162PLANE444UNORM = 1000330003,
        ARGB4UNORMPACK16 = 1000340000,
        ABGR4UNORMPACK16 = 1000340001,
        ASTC4x4SFLOATBLOCK = 1000066000,
        ASTC5x4SFLOATBLOCK = 1000066001,
        ASTC5x5SFLOATBLOCK = 1000066002,
        ASTC6x5SFLOATBLOCK = 1000066003,
        ASTC6x6SFLOATBLOCK = 1000066004,
        ASTC8x5SFLOATBLOCK = 1000066005,
        ASTC8x6SFLOATBLOCK = 1000066006,
        ASTC8x8SFLOATBLOCK = 1000066007,
        ASTC10x5SFLOATBLOCK = 1000066008,
        ASTC10x6SFLOATBLOCK = 1000066009,
        ASTC10x8SFLOATBLOCK = 1000066010,
        ASTC10x10SFLOATBLOCK = 1000066011,
        ASTC12x10SFLOATBLOCK = 1000066012,
        ASTC12x12SFLOATBLOCK = 1000066013,
        PVRTC12BPPUNORMBLOCKIMG = 1000054000,
        PVRTC14BPPUNORMBLOCKIMG = 1000054001,
        PVRTC22BPPUNORMBLOCKIMG = 1000054002,
        PVRTC24BPPUNORMBLOCKIMG = 1000054003,
        PVRTC12BPPSRGBBLOCKIMG = 1000054004,
        PVRTC14BPPSRGBBLOCKIMG = 1000054005,
        PVRTC22BPPSRGBBLOCKIMG = 1000054006,
        PVRTC24BPPSRGBBLOCKIMG = 1000054007,
        RG16S105 = 1000464000,
        A1BGR5UNORMPACK16 = 1000470000,
        A8UNORM = 1000470001,
    };
    enum class ColorSpace
    {
        SRGBNL = 0,
        DispP3NL = 1000104002,
        ExtSRGBL = 1000104003,
        DispP3L = 1000104004,
        DciP3NL = 1000104005,
        Bt709L = 1000104006,
        Bt709NL = 1000104007,
        Bt2020L = 1000104008,
        HDR10St2084 = 1000104009,
        Dolbyvision = 1000104010,
        HDR10HLG = 1000104011,
        AdobeRGBL = 1000104012,
        AdobeRGBNL = 1000104013,
        PassThrough = 1000104014,
        ExtSRGBNL = 1000213000
    };
    enum class ShaderStage
    {
        Vertex = 0x00000001,
        TessellationControl = 0x00000002,
        TessellationEvaluation = 0x00000004,
        Geometry = 0x00000008,
        Fragment = 0x00000010,
        Compute = 0x00000020,
        AllGraphics = 0x0000001F,
        All = 0x7FFFFFFF,
        Raygen = 0x00000100,
        AnyHit = 0x00000200,
        ClosestHit = 0x00000400,
        Miss = 0x00000800,
        Intersection = 0x00001000,
        Callable = 0x00002000,
        Task = 0x00000040,
        Mesh = 0x00000080,
        SubpassShading = 0x00004000,
        ClusterCulling = 0x00080000,
    };
    enum class DynamicState
    {
        Viewport = 0,
        Scissor = 1,
        LineWidth = 2,
        DepthBias = 3,
        BlendConstants = 4,
        DepthBounds = 5,
        StencilCompareMask = 6,
        StencilWriteMask = 7,
        StencilReference = 8,
        CullMode = 1000267000,
        FrontFace = 1000267001,
        PrimitiveTopology = 1000267002,
        ViewportWithCount = 1000267003,
        ScissorWithCount = 1000267004,
        VertexInputBindingStride = 1000267005,
        DepthTestEnable = 1000267006,
        DepthWriteEnable = 1000267007,
        DepthCompareOp = 1000267008,
        DepthBoundsTestEnable = 1000267009,
        StencilTestEnable = 1000267010,
        StencilOp = 1000267011,
        RasterizerDiscardEnable = 1000377001,
        DepthBiasEnable = 1000377002,
        PrimitiveRestartEnable = 1000377004,
        ViewportWScaling = 1000087000,
        DiscardRectangle = 1000099000,
        DiscardRectangleEnable = 1000099001,
        DiscardRectangleMode = 1000099002,
        SampleLocations = 1000143000,
        RayTracingPipelineStackSize = 1000347000,
        ViewportShadingRatePalette = 1000164004,
        ViewportCoarseSampleOrder = 1000164006,
        ExclusiveScissorEnable = 1000205000,
        ExclusiveScissor = 1000205001,
        FragmentShadingRate = 1000226000,
        LineStipple = 1000259000,
        VertexInput = 1000352000,
        PatchControlPoints = 1000377000,
        LogicOp = 1000377003,
        ColorWriteEnable = 1000381000,
        TessellationDomainOrigin = 1000455002,
        DepthClampEnable = 1000455003,
        PolygonMode = 1000455004,
        RasterizationSamples = 1000455005,
        SampleMask = 1000455006,
        AlphaToCoverageEnable = 1000455007,
        AlphaToOneEnable = 1000455008,
        LogicOpEnable = 1000455009,
        ColorBlendEnable = 1000455010,
        ColorBlendEquation = 1000455011,
        ColorWriteMask = 1000455012,
        RasterizationStream = 1000455013,
        ConservativeRasterizationMode = 1000455014,
        raPrimitiveOverestimationSize = 1000455015,
        DepthClipEnable = 1000455016,
        SampleLocationsEnable = 1000455017,
        ColorBlendAdvanced = 1000455018,
        ProvokingVertexMode = 1000455019,
        LineRasterizationMode = 1000455020,
        LineStippleEnable = 1000455021,
        DepthClipNegativeOneToOne = 1000455022,
        ViewportWScalingEnable = 1000455023,
        ViewportSwizzle = 1000455024,
        CoverageToColorEnable = 1000455025,
        CoverageToColorLocation = 1000455026,
        CoverageModulationMode = 1000455027,
        CoverageModulationTableEnable = 1000455028,
        CoverageModulationTable = 1000455029,
        ShadingRateImageEnable = 1000455030,
        RepresentativeFragmentTestEnable = 1000455031,
        CoverageReductionMode = 1000455032,
        AttachmentFeedbackLoopEnable = 1000524000,
    };
    enum class Primitive
    {
        Points = 0,
        Lines = 1,
        LineStrip = 2,
        Triangles = 3,
        TriangleStrip = 4,
        TriangleFan = 5,
        LineListWithAdjacency = 6,
        LineStripWithAdjacency = 7,
        TriangleListWithAdjacency = 8,
        TriangleStripWithAdjacency = 9,
        PatchList = 10,
    };
    enum class PolygonMode
    {
        Fill = 0,
        Line = 1,
        Point = 2,
        FillRectangle = 1000153000
    };
    enum class CullMode
    {
        None = 0,
        Front = 0x00000001,
        Back = 0x00000002,
        FrontAndBack = 0x00000003
    };
    enum class FrontFace
    {
        CounterClockwise = 0,
        Clockwise = 1
    };
    enum class ColorComponent
    {
        R = 0x00000001,
        G = 0x00000002,
        B = 0x00000004,
        A = 0x00000008,
        RGB = R | G | B,
        RGBA = RGB | A,
    };
    enum class BlendOp
    {
        Add = 0,
        Subtract = 1,
        ReverseSubtract = 2,
        Min = 3,
        Max = 4,
        Zero = 1000148000,
        Src = 1000148001,
        Dst = 1000148002,
        SrcOver = 1000148003,
        DstOver = 1000148004,
        SrcIn = 1000148005,
        DstIn = 1000148006,
        SrcOut = 1000148007,
        DstOut = 1000148008,
        SrcAtop = 1000148009,
        DstAtop = 1000148010,
        Xor = 1000148011,
        Multiply = 1000148012,
        Screen = 1000148013,
        Overlay = 1000148014,
        Darken = 1000148015,
        Lighten = 1000148016,
        Colordodge = 1000148017,
        Colorburn = 1000148018,
        Hardlight = 1000148019,
        Softlight = 1000148020,
        Difference = 1000148021,
        Exclusion = 1000148022,
        Invert = 1000148023,
        InvertRgb = 1000148024,
        Lineardodge = 1000148025,
        Linearburn = 1000148026,
        Vividlight = 1000148027,
        Linearlight = 1000148028,
        Pinlight = 1000148029,
        Hardmix = 1000148030,
        HslHue = 1000148031,
        HslSaturation = 1000148032,
        HslColor = 1000148033,
        HslLuminosity = 1000148034,
        Plus = 1000148035,
        PlusClamped = 1000148036,
        PlusClampedAlpha = 1000148037,
        PlusDarker = 1000148038,
        Minus = 1000148039,
        MinusClamped = 1000148040,
        Contrast = 1000148041,
        InvertOvg = 1000148042,
        Red = 1000148043,
        Green = 1000148044,
        Blue = 1000148045
    };
    enum class LogicOp
    {
        Clear = 0,
        And = 1,
        AndReverse = 2,
        Copy = 3,
        AndInverted = 4,
        NoOp = 5,
        Xor = 6,
        Or = 7,
        Nor = 8,
        Equivalent = 9,
        Invert = 10,
        OrReverse = 11,
        CopyInverted = 12,
        OrInverted = 13,
        Nand = 14,
        Set = 15,
    };
    enum class BlendFactor
    {
        Zero = 0,
        One = 1,
        SrcColor = 2,
        OneMinusSrcColor = 3,
        DstColor = 4,
        OneMinusDstColor = 5,
        SrcAlpha = 6,
        OneMinusSrcAlpha = 7,
        DstAlpha = 8,
        OneMinusDstAlpha = 9,
        ConstantColor = 10,
        OneMinusConstantColor = 11,
        ConstantAlpha = 12,
        OneMinusConstantAlpha = 13,
        SrcAlphaSaturate = 14,
        Src1Color = 15,
        OneMinusSrc1Color = 16,
        Src1Alpha = 17,
        OneMinusSrc1Alpha = 18,
    };
    enum class LoadOp
    {
        Load = 0,
        Clear = 1,
        DontCare = 2,
        None = 1000400000
    };
    enum class StoreOp
    {
        Store = 0,
        DontCare = 1,
        None = 1000301000,
    };
    enum class ImageLayout
    {
        Undefined = 0,
        General = 1,
        ColorAttachmentOptimal = 2,
        DepthStencilAttachmentOptimal = 3,
        DepthStencilReadOnlyOptimal = 4,
        ShaderReadOnlyOptimal = 5,
        TransferSrcOptimal = 6,
        TransferDstOptimal = 7,
        Preinitialized = 8,
        DepthReadOnlyStencilAttachmentOptimal = 1000117000,
        DepthAttachmentStencilReadOnlyOptimal = 1000117001,
        DepthAttachmentOptimal = 1000241000,
        DepthReadOnlyOptimal = 1000241001,
        StencilAttachmentOptimal = 1000241002,
        StencilReadOnlyOptimal = 1000241003,
        ReadOnlyOptimal = 1000314000,
        AttachmentOptimal = 1000314001,
        PresentSrc = 1000001002,
        VideoDecodeDst = 1000024000,
        VideoDecodeSrc = 1000024001,
        VideoDecodeDpb = 1000024002,
        SharedPresent = 1000111000,
        FragmentDensityMapOptimal = 1000218000,
        FragmentShadingRateAttachmentOptimal = 1000164003,
        VideoEncodeDst = 1000299000,
        VideoEncodeSrc = 1000299001,
        VideoEncodeDpb = 1000299002,
        AttachmentFeedbackLoopOptimal = 1000339000,
    };
    enum class PipelineStage
    {
        TopOfPipe = 1,
        DrawIndirect = 2,
        VertexInput = 4,
        VertexShader = 8,
        TessellationControlShader = 16,
        TessellationEvaluationShader = 32,
        GeometryShader = 64,
        FragmentShader = 128,
        EarlyFragmentTests = 256,
        LateFragmentTests = 512,
        ColorAttachmentOutput = 1024,
        ComputeShader = 2048,
        Transfer = 4096,
        BottomOfPipe = 8192,
        Host = 16384,
        AllGraphics = 32768,
        AllCommands = 65536,
        None = 0,
        TransformFeedback = 0x01000000,
        ConditionalRendering = 0x00040000,
        AccelerationStructureBuild = 0x02000000,
        RayTracingShader = 0x00200000,
        ShadingRateImage = 0x00800000,
        TaskShader = 0x00080000,
        MeshShader = 0x00100000,
    };

    enum class BufferUsage
    {
        TransferSrc = 0x00000001,
        TransferDst = 0x00000002,
        UniformTexelBuffer = 0x00000004,
        StorageTexelBuffer = 0x00000008,
        UniformBuffer = 0x00000010,
        StorageBuffer = 0x00000020,
        IndexBuffer = 0x00000040,
        VertexBuffer = 0x00000080,
        IndirectBuffer = 0x00000100,
        ShaderDeviceAddress = 0x00020000,
        VideoDecodeSrc = 0x00002000,
        VideoDecodeDst = 0x00004000,
        TransformFeedbackBuffer = 0x00000800,
        TransformFeedbackCounterBuffer = 0x00001000,
        ConditionalRendering = 0x00000200,
        AccelerationStructureBuildInputReadOnly = 0x00080000,
        AccelerationStructureStorage = 0x00100000,
        ShaderBindingTable = 0x00000400,
        SamplerDescriptorBuffer = 0x00200000,
        ResourceDescriptorBuffer = 0x00400000,
        PushDescriptorsDescriptorBuffer = 0x04000000,
        MicromapBuildInputReadOnly = 0x00800000,
        MicromapStorage = 0x01000000,
    };

    enum class BufferSharing
    {
        Exclusive = 0,
        Concurrent = 1,
    };

    enum class MemoryProperty
    {
        DeviceLocal = 0x00000001,
        HostVisible = 0x00000002,
        HostCoherent = 0x00000004,
        HostCached = 0x00000008,
        LazilyAllocated = 0x00000010,
        Protected = 0x00000020,
        DeviceCoherentAMD = 0x00000040,
        DeviceUncachedAMD = 0x00000080,
        RdmaCapableNV = 0x00000100
    };

    enum class DescriptorType
    {
        Sampler = 0,
        CombinedImageSampler = 1,
        SampledImage = 2,
        StorageImage = 3,
        UniformTexelBuffer = 4,
        StorageTexelBuffer = 5,
        UniformBuffer = 6,
        StorageBuffer = 7,
        UniformBufferDynamic = 8,
        StorageBufferDynamic = 9,
        InputAttachment = 10,
        InlineUniformBlock = 1000138000,
        AccelerationStructure = 1000150000,
        AccelerationStructureNv = 1000165000,
        SampleWeightImage = 1000440000,
        BlockMatchImage = 1000440001,
        Mutable = 1000351000,
    };

    enum class InputRate
    {
        Vertex = 0,
        Instance = 1
    };
}