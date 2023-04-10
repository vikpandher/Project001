#include "Vulkan_Renderer_T01.h"

#include "Engine/MeshLoader.h"
#include "Engine/Logger.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"

#include <fstream>
#include <set>



namespace Project001
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            _LOG_ERROR("Vulkan Error: %s", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            _LOG_MESSAGE("Vulkan Warning: %s", pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    // public ------------------------------------------------------------------

    Vulkan_Renderer_T01::Vulkan_Renderer_T01(
        Window* windowPtr,
        unsigned int width,
        unsigned int height,
        bool multisampleAntiAliasing)
        : windowPtr_(windowPtr)
        , vkInstance_(VK_NULL_HANDLE)
        , vkDebugMessenger_(VK_NULL_HANDLE)
        , vkSurface_(VK_NULL_HANDLE)
        , vkPhysicalDevice_(VK_NULL_HANDLE)
        , vkMsaaSamples_(VK_SAMPLE_COUNT_1_BIT)
        , vkDevice_(VK_NULL_HANDLE)
        , vkGraphicsQueue_(VK_NULL_HANDLE)
        , vkPresentQueue_(VK_NULL_HANDLE)
        , vkSwapChain_(VK_NULL_HANDLE)
        , vkRenderPass_(VK_NULL_HANDLE)
        , vkDescriptorSetLayout_(VK_NULL_HANDLE)
        , vkPipelineLayout_(VK_NULL_HANDLE)
        , vkGraphicsPipeline_(VK_NULL_HANDLE)
        , vkCommandPool_(VK_NULL_HANDLE)
        , vkColorImage_(VK_NULL_HANDLE)
        , vkColorImageMemory_(VK_NULL_HANDLE)
        , vkColorImageView_(VK_NULL_HANDLE)
        , mipLevels_(0)
        , vkTextureImage_(VK_NULL_HANDLE)
        , vkTextureImageMemory_(VK_NULL_HANDLE)
        , vkTextureImageView_(VK_NULL_HANDLE)
        , vkTextureSampler_(VK_NULL_HANDLE)
        , vkVertexBuffer_(VK_NULL_HANDLE)
        , vkVertexBufferMemory_(VK_NULL_HANDLE)
        , vkIndexBuffer_(VK_NULL_HANDLE)
        , vkIndexBufferMemory_(VK_NULL_HANDLE)
        , vkDescriptorPool_(VK_NULL_HANDLE)
        , currentFrame_(0)
        , framebufferResized_(false)
    {
        CreateVulkanInstance(); // 1
        SetupDebugMessanger(); // 2
        CreateSurface(); // 3
        PickPhysicalDevice(); // 4
        CreateLogicalDevice(); // 5
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDescriptorSetLayout();
        CreateGraphicsPipeline();
        CreateCommandPool(); // 6
        CreateColorResources();
        CreateDepthResources();
        CreateFramebuffers();
        CreateTextureImage();
        CreateTextureImageView();
        CreateTextureSampler();
        LoadModel();
        CreateVertexBuffer();
        CreateIndexBuffer();
        CreateUniformBuffers();
        CreateDescriptorPool();
        CreateDescriptorSets();
        CreateCommandBuffers();
        CreateSyncObjects();
    }

    Vulkan_Renderer_T01::~Vulkan_Renderer_T01()
    {
        vkDeviceWaitIdle(vkDevice_);

        CleanupSwapChain();

        vkDestroyPipeline(vkDevice_, vkGraphicsPipeline_, nullptr);
        vkDestroyPipelineLayout(vkDevice_, vkPipelineLayout_, nullptr);
        vkDestroyRenderPass(vkDevice_, vkRenderPass_, nullptr);

        for (size_t i = 0; i < s_maxFramesInFlight_; i++)
        {
            vkDestroyBuffer(vkDevice_, vkUniformBuffers_[i], nullptr);
            vkFreeMemory(vkDevice_, vkUniformBuffersMemory_[i], nullptr);
        }

        vkDestroyDescriptorPool(vkDevice_, vkDescriptorPool_, nullptr);

        vkDestroySampler(vkDevice_, vkTextureSampler_, nullptr);
        vkDestroyImageView(vkDevice_, vkTextureImageView_, nullptr);

        vkDestroyImage(vkDevice_, vkTextureImage_, nullptr);
        vkFreeMemory(vkDevice_, vkTextureImageMemory_, nullptr);

        vkDestroyDescriptorSetLayout(vkDevice_, vkDescriptorSetLayout_, nullptr);

        vkDestroyBuffer(vkDevice_, vkIndexBuffer_, nullptr);
        vkFreeMemory(vkDevice_, vkIndexBufferMemory_, nullptr);

        vkDestroyBuffer(vkDevice_, vkVertexBuffer_, nullptr);
        vkFreeMemory(vkDevice_, vkVertexBufferMemory_, nullptr);

        for (size_t i = 0; i < s_maxFramesInFlight_; ++i)
        {
            vkDestroySemaphore(vkDevice_, vkRenderFinishedSemaphores_[i], nullptr);
            vkDestroySemaphore(vkDevice_, vkImageAvailableSemaphores_[i], nullptr);
            vkDestroyFence(vkDevice_, vkInFlightFences_[i], nullptr);
        }

        vkDestroyCommandPool(vkDevice_, vkCommandPool_, nullptr);

        vkDestroyDevice(vkDevice_, nullptr);

        if (s_enableValidationLayers_)
        {
            PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance_, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr)
            {
                func(vkInstance_, vkDebugMessenger_, nullptr);
            }
        }

        vkDestroySurfaceKHR(vkInstance_, vkSurface_, nullptr);
        vkDestroyInstance(vkInstance_, nullptr);
    }

    void Vulkan_Renderer_T01::Render()
    {
        DrawFrame();
    }

    // protected ---------------------------------------------------------------

    void Vulkan_Renderer_T01::CreateVulkanInstance()
    {
        if (s_enableValidationLayers_ && !CheckValidationLayerSupport())
        {
            _LOG_ERROR("Vulkan Error: Validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "No Application Name";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine Name";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        std::vector<const char*> extensions = GetRequiredExtensions();
        createInfo.enabledExtensionCount = (uint32_t)extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (s_enableValidationLayers_)
        {
            createInfo.enabledLayerCount = (uint32_t)s_validationLayers_.size();
            createInfo.ppEnabledLayerNames = s_validationLayers_.data();

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (void*)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &vkInstance_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create instance!");
        }
    }

    void Vulkan_Renderer_T01::SetupDebugMessanger()
    {
        if (s_enableValidationLayers_)
        {
            PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance_, "vkCreateDebugUtilsMessengerEXT");
            VkResult result;

            if (func != nullptr)
            {
                VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
                PopulateDebugMessengerCreateInfo(debugCreateInfo);
                result = func(vkInstance_, &debugCreateInfo, nullptr, &vkDebugMessenger_);
            }
            else
            {
                result = VK_ERROR_EXTENSION_NOT_PRESENT;
            }

            if (result != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to set up debug messenger!");
            }
        }
    }

    void Vulkan_Renderer_T01::CreateSurface()
    {
        windowPtr_->CreateWindowSurface(vkInstance_, vkSurface_);
    }

    void Vulkan_Renderer_T01::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            _LOG_ERROR("Vulkan Error: Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, devices.data());

        for (size_t i = 0; i < devices.size(); ++i)
        {
            const VkPhysicalDevice& device = devices[i];
            if (IsDeviceSuitable(device))
            {
                vkPhysicalDevice_ = device;
                vkMsaaSamples_ = GetMaxUsableSampleCount();
                break;
            }
        }

        if (vkPhysicalDevice_ == VK_NULL_HANDLE)
        {
            _LOG_ERROR("Vulkan Error: Failed to find a suitable GPU!");
        }
    }

    void Vulkan_Renderer_T01::CreateLogicalDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice_);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamilyIndex, indices.presentFamilyIndex };

        float queuePriority = 1.0f;
        std::set<uint32_t>::iterator iter;
        for (iter = uniqueQueueFamilies.begin(); iter != uniqueQueueFamilies.end(); ++iter)
        {
            const uint32_t& queueFamily = *iter;

            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = (uint32_t)s_deviceExtensions_.size();
        createInfo.ppEnabledExtensionNames = s_deviceExtensions_.data();

        if (s_enableValidationLayers_)
        {
            createInfo.enabledLayerCount = (uint32_t)s_validationLayers_.size();
            createInfo.ppEnabledLayerNames = s_validationLayers_.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(vkPhysicalDevice_, &createInfo, nullptr, &vkDevice_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create logical device!");
        }

        vkGetDeviceQueue(vkDevice_, indices.graphicsFamilyIndex, 0, &vkGraphicsQueue_);
        vkGetDeviceQueue(vkDevice_, indices.presentFamilyIndex, 0, &vkPresentQueue_);
    }

    void Vulkan_Renderer_T01::CreateSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(vkPhysicalDevice_);

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.surfaceFormats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.surfaceCapabilities);

        uint32_t imageCount = swapChainSupport.surfaceCapabilities.minImageCount + 1;
        if (swapChainSupport.surfaceCapabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.surfaceCapabilities.maxImageCount)
        {
            imageCount = swapChainSupport.surfaceCapabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vkSurface_;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice_);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamilyIndex, indices.presentFamilyIndex };

        if (indices.graphicsFamilyIndex != indices.presentFamilyIndex)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(vkDevice_, &createInfo, nullptr, &vkSwapChain_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(vkDevice_, vkSwapChain_, &imageCount, nullptr);
        vkSwapChainImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(vkDevice_, vkSwapChain_, &imageCount, vkSwapChainImages_.data());

        vkSwapChainImageFormat_ = surfaceFormat.format;
        vkSwapChainExtent_ = extent;
    }

    void Vulkan_Renderer_T01::CreateImageViews()
    {
        vkSwapChainImageViews_.resize(vkSwapChainImages_.size());

        for (size_t i = 0; i < vkSwapChainImages_.size(); i++)
        {
            vkSwapChainImageViews_[i] = CreateImageView(vkSwapChainImages_[i], vkSwapChainImageFormat_, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    void Vulkan_Renderer_T01::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = vkSwapChainImageFormat_;
        colorAttachment.samples = vkMsaaSamples_;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = FindDepthFormat();
        depthAttachment.samples = vkMsaaSamples_;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve = {};
        colorAttachmentResolve.format = vkSwapChainImageFormat_;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentResolveRef = {};
        colorAttachmentResolveRef.attachment = 2;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = (uint32_t)attachments.size();
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(vkDevice_, &renderPassInfo, nullptr, &vkRenderPass_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create render pass!");
        }
    }

    void Vulkan_Renderer_T01::CreateDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = (uint32_t)bindings.size();
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(vkDevice_, &layoutInfo, nullptr, &vkDescriptorSetLayout_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create descriptor set layout!");
        }
    }

    void Vulkan_Renderer_T01::CreateGraphicsPipeline()
    {
        std::vector<char> vertShaderCode = ReadFile("../Shaders/27_shader_depth_vert.spv");
        std::vector<char> fragShaderCode = ReadFile("../Shaders/27_shader_depth_frag.spv");

        VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDescription = Vertex::GetBindingDescription();
        auto attributeDescriptions = Vertex::GetAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // OpenGL default
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vkMsaaSamples_;

        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &vkDescriptorSetLayout_;

        if (vkCreatePipelineLayout(vkDevice_, &pipelineLayoutInfo, nullptr, &vkPipelineLayout_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = vkPipelineLayout_;
        pipelineInfo.renderPass = vkRenderPass_;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(vkDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(vkDevice_, fragShaderModule, nullptr);
        vkDestroyShaderModule(vkDevice_, vertShaderModule, nullptr);
    }

    void Vulkan_Renderer_T01::CreateCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vkPhysicalDevice_);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamilyIndex;

        if (vkCreateCommandPool(vkDevice_, &poolInfo, nullptr, &vkCommandPool_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create graphics command pool!");
        }
    }

    void Vulkan_Renderer_T01::CreateColorResources()
    {
        VkFormat colorFormat = vkSwapChainImageFormat_;

        CreateImage(vkSwapChainExtent_.width, vkSwapChainExtent_.height, 1, vkMsaaSamples_, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkColorImage_, vkColorImageMemory_);
        vkColorImageView_ = CreateImageView(vkColorImage_, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void Vulkan_Renderer_T01::CreateDepthResources()
    {
        VkFormat depthFormat = FindDepthFormat();

        CreateImage(vkSwapChainExtent_.width, vkSwapChainExtent_.height, 1, vkMsaaSamples_, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkDepthImage_, vkDepthImageMemory_);
        vkDepthImageView_ = CreateImageView(vkDepthImage_, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void Vulkan_Renderer_T01::CreateFramebuffers()
    {
        vkSwapChainFramebuffers_.resize(vkSwapChainImageViews_.size());

        for (size_t i = 0; i < vkSwapChainImageViews_.size(); ++i)
        {
            std::array<VkImageView, 3> attachments = {
                vkColorImageView_,
                vkDepthImageView_,
                vkSwapChainImageViews_[i]
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vkRenderPass_;
            framebufferInfo.attachmentCount = (uint32_t)attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = vkSwapChainExtent_.width;
            framebufferInfo.height = vkSwapChainExtent_.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(vkDevice_, &framebufferInfo, nullptr, &vkSwapChainFramebuffers_[i]) != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to create framebuffer!");
            }
        }
    }

    void Vulkan_Renderer_T01::CreateTextureImage()
    {
        TextureData textureData;
        if (!TextureLoader::LoadTexture(textureData, s_texturePath_, true))
        {
            _LOG_ERROR("Failed to load texture image!");
        }

        VkDeviceSize imageSize = textureData.width * textureData.height * 4;
        mipLevels_ = (uint32_t)std::floor(std::log2(std::max(textureData.width, textureData.height))) + 1;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* destinationData;
        vkMapMemory(vkDevice_, stagingBufferMemory, 0, imageSize, 0, &destinationData);
        memcpy(destinationData, textureData.data, (size_t)imageSize);
        vkUnmapMemory(vkDevice_, stagingBufferMemory);

        CreateImage(textureData.width, textureData.height, mipLevels_, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkTextureImage_, vkTextureImageMemory_);

        TransitionImageLayout(vkTextureImage_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels_);
        CopyBufferToImage(stagingBuffer, vkTextureImage_, (uint32_t)textureData.width, (uint32_t)textureData.height);
        // transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps

        vkDestroyBuffer(vkDevice_, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice_, stagingBufferMemory, nullptr);

        GenerateMipmaps(vkTextureImage_, VK_FORMAT_R8G8B8A8_SRGB, textureData.width, textureData.height, mipLevels_);
    }

    void Vulkan_Renderer_T01::CreateTextureImageView()
    {
        vkTextureImageView_ = CreateImageView(vkTextureImage_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels_);
    }

    void Vulkan_Renderer_T01::CreateTextureSampler()
    {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(vkPhysicalDevice_, &properties);

        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f; // (float)mipLevels_ / 2.0f;
        samplerInfo.maxLod = (float)mipLevels_;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(vkDevice_, &samplerInfo, nullptr, &vkTextureSampler_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create texture sampler!");
        }
    }

    void Vulkan_Renderer_T01::LoadModel()
    {
        MeshData meshData;
        if (!MeshLoader::LoadMeshOBJ(meshData, s_meshPath_))
        {
            _LOG_ERROR("Failed to load model!");
        }

        Project001::MeshLoader::RotateMeshX(meshData, -glm::half_pi<float>() - glm::quarter_pi<float>());
        Project001::MeshLoader::RotateMeshY(meshData, -glm::half_pi<float>() - glm::quarter_pi<float>());

        for (size_t i = 0; i < meshData.meshVertexArray.size(); ++i)
        {
            const MeshVertex& currentMeshVertex = meshData.meshVertexArray[i];
            Vertex newVertex =
            {
                currentMeshVertex.position,
                {1.0f, 1.0f, 1.0f},
                currentMeshVertex.textureCoordinate
            };
            vertices_.push_back(newVertex);
        }

        for (size_t i = 0; i < meshData.meshIndexArray.size(); ++i)
        {
            indices_.push_back((uint32_t)meshData.meshIndexArray[i]);
        }
    }

    void Vulkan_Renderer_T01::CreateVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(vertices_[0]) * vertices_.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice_, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices_.data(), (size_t)bufferSize);
        vkUnmapMemory(vkDevice_, stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkVertexBuffer_, vkVertexBufferMemory_);

        CopyBuffer(stagingBuffer, vkVertexBuffer_, bufferSize);

        vkDestroyBuffer(vkDevice_, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice_, stagingBufferMemory, nullptr);
    }

    void Vulkan_Renderer_T01::CreateIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(indices_[0]) * indices_.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice_, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices_.data(), (size_t)bufferSize);
        vkUnmapMemory(vkDevice_, stagingBufferMemory);

        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkIndexBuffer_, vkIndexBufferMemory_);

        CopyBuffer(stagingBuffer, vkIndexBuffer_, bufferSize);

        vkDestroyBuffer(vkDevice_, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice_, stagingBufferMemory, nullptr);
    }

    void Vulkan_Renderer_T01::CreateCommandBuffers()
    {
        vkCommandBuffers_.resize(s_maxFramesInFlight_);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vkCommandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)vkCommandBuffers_.size();

        if (vkAllocateCommandBuffers(vkDevice_, &allocInfo, vkCommandBuffers_.data()) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate command buffers!");
        }
    }

    void Vulkan_Renderer_T01::CreateUniformBuffers()
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        vkUniformBuffers_.resize(s_maxFramesInFlight_);
        vkUniformBuffersMemory_.resize(s_maxFramesInFlight_);

        for (size_t i = 0; i < s_maxFramesInFlight_; i++)
        {
            CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vkUniformBuffers_[i], vkUniformBuffersMemory_[i]);
        }
    }

    void Vulkan_Renderer_T01::CreateDescriptorPool()
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes = {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = s_maxFramesInFlight_;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = s_maxFramesInFlight_;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = s_maxFramesInFlight_;

        if (vkCreateDescriptorPool(vkDevice_, &poolInfo, nullptr, &vkDescriptorPool_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create descriptor pool!");
        }
    }

    void Vulkan_Renderer_T01::CreateDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(s_maxFramesInFlight_, vkDescriptorSetLayout_);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = vkDescriptorPool_;
        allocInfo.descriptorSetCount = s_maxFramesInFlight_;
        allocInfo.pSetLayouts = layouts.data();

        vkDescriptorSets_.resize(s_maxFramesInFlight_);
        if (vkAllocateDescriptorSets(vkDevice_, &allocInfo, vkDescriptorSets_.data()) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < s_maxFramesInFlight_; i++)
        {
            VkDescriptorBufferInfo bufferInfo = {};
            bufferInfo.buffer = vkUniformBuffers_[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo = {};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vkTextureImageView_;
            imageInfo.sampler = vkTextureSampler_;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = vkDescriptorSets_[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = vkDescriptorSets_[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(vkDevice_, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
    }

    void Vulkan_Renderer_T01::CreateSyncObjects()
    {
        vkImageAvailableSemaphores_.resize(s_maxFramesInFlight_);
        vkRenderFinishedSemaphores_.resize(s_maxFramesInFlight_);
        vkInFlightFences_.resize(s_maxFramesInFlight_);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (size_t i = 0; i < s_maxFramesInFlight_; i++)
        {
            if (vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkImageAvailableSemaphores_[i]) != VK_SUCCESS ||
                vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkRenderFinishedSemaphores_[i]) != VK_SUCCESS ||
                vkCreateFence(vkDevice_, &fenceInfo, nullptr, &vkInFlightFences_[i]) != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to create synchronization objects for a frame!");
            }
        }
    }

    void Vulkan_Renderer_T01::RecreateSwapChain()
    {
        int width = 0, height = 0;
        windowPtr_->GetFramebufferSize(width, height);
        while (width == 0 || height == 0)
        {
            windowPtr_->SleepAndWaitForEvents();
        }

        vkDeviceWaitIdle(vkDevice_);

        CleanupSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateColorResources();
        CreateDepthResources();
        CreateFramebuffers();
    }

    void Vulkan_Renderer_T01::CleanupSwapChain()
    {
        vkDestroyImageView(vkDevice_, vkDepthImageView_, nullptr);
        vkDestroyImage(vkDevice_, vkDepthImage_, nullptr);
        vkFreeMemory(vkDevice_, vkDepthImageMemory_, nullptr);

        vkDestroyImageView(vkDevice_, vkColorImageView_, nullptr);
        vkDestroyImage(vkDevice_, vkColorImage_, nullptr);
        vkFreeMemory(vkDevice_, vkColorImageMemory_, nullptr);

        for (size_t i = 0; i < vkSwapChainFramebuffers_.size(); ++i)
        {
            const VkFramebuffer& framebuffer = vkSwapChainFramebuffers_[i];
            vkDestroyFramebuffer(vkDevice_, framebuffer, nullptr);
        }

        for (size_t i = 0; i < vkSwapChainImageViews_.size(); ++i)
        {
            const VkImageView& imageView = vkSwapChainImageViews_[i];
            vkDestroyImageView(vkDevice_, imageView, nullptr);
        }

        vkDestroySwapchainKHR(vkDevice_, vkSwapChain_, nullptr);
    }

    bool Vulkan_Renderer_T01::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (size_t i = 0; i < s_validationLayers_.size(); ++i)
        {
            const char* const& layerName = s_validationLayers_[i];

            bool layerFound = false;

            for (size_t j = 0; j < availableLayers.size(); ++j)
            {
                const VkLayerProperties& layerProperties = availableLayers[j];
                if (std::strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> Vulkan_Renderer_T01::GetRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = windowPtr_->GetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (s_enableValidationLayers_)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void Vulkan_Renderer_T01::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
    }

    bool Vulkan_Renderer_T01::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.surfaceFormats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures = {};
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete()
            && extensionsSupported
            && swapChainAdequate
            && supportedFeatures.samplerAnisotropy;
    }

    VkSampleCountFlagBits Vulkan_Renderer_T01::GetMaxUsableSampleCount()
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(vkPhysicalDevice_, &physicalDeviceProperties);

        VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    QueueFamilyIndices Vulkan_Renderer_T01::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = {};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (size_t i = 0; i < queueFamilies.size(); ++i)
        {
            const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamilyIndex = (uint32_t)i;
                indices.valid_graphicsFamilyIndex = true;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, (uint32_t)i, vkSurface_, &presentSupport);
            if (presentSupport)
            {
                indices.presentFamilyIndex = (uint32_t)i;
                indices.valid_presentFamilyIndex = true;
            }

            if (indices.isComplete())
            {
                break;
            }
        }

        return indices;
    }

    SwapChainSupportDetails Vulkan_Renderer_T01::QuerySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vkSurface_, &details.surfaceCapabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface_, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSurface_, &formatCount, details.surfaceFormats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface_, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSurface_, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR Vulkan_Renderer_T01::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (size_t i = 0; i < availableFormats.size(); ++i)
        {
            const VkSurfaceFormatKHR& availableFormat = availableFormats[i];
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR Vulkan_Renderer_T01::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (size_t i = 0; i < availablePresentModes.size(); ++i)
        {
            const VkPresentModeKHR& availablePresentMode = availablePresentModes[i];
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Vulkan_Renderer_T01::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            windowPtr_->GetFramebufferSize(width, height);

            VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };

            if (actualExtent.width < capabilities.minImageExtent.width)
            {
                actualExtent.width = capabilities.minImageExtent.width;
            }
            else if (actualExtent.width > capabilities.maxImageExtent.width)
            {
                actualExtent.width = capabilities.maxImageExtent.width;
            }

            if (actualExtent.height < capabilities.minImageExtent.height)
            {
                actualExtent.height = capabilities.minImageExtent.height;
            }
            else if (actualExtent.height > capabilities.maxImageExtent.height)
            {
                actualExtent.height = capabilities.maxImageExtent.height;
            }

            return actualExtent;
        }
    }

    VkImageView Vulkan_Renderer_T01::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
    {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(vkDevice_, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create texture image view!");
        }

        return imageView;
    }

    VkFormat Vulkan_Renderer_T01::FindDepthFormat()
    {
        return FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkFormat Vulkan_Renderer_T01::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (size_t i = 0; i < candidates.size(); ++i)
        {
            VkFormat format = candidates[i];
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice_, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        _LOG_ERROR("Vulkan Error: Failed to find supported format!");
        return VK_FORMAT_UNDEFINED;
    }

    bool Vulkan_Renderer_T01::HasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    bool Vulkan_Renderer_T01::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(s_deviceExtensions_.begin(), s_deviceExtensions_.end());

        for (size_t i = 0; i < availableExtensions.size(); ++i)
        {
            const VkExtensionProperties& extension = availableExtensions[i];
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    std::vector<char> Vulkan_Renderer_T01::ReadFile(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            _LOG_ERROR("Vulkan Error: Failed to open file!");
            return std::vector<char>();
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VkShaderModule Vulkan_Renderer_T01::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create shader module!");
        }

        return shaderModule;
    }

    void Vulkan_Renderer_T01::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(vkDevice_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(vkDevice_, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(vkDevice_, buffer, bufferMemory, 0);
    }

    void Vulkan_Renderer_T01::CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = numSamples;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(vkDevice_, &imageInfo, nullptr, &image) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(vkDevice_, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate image memory!");
        }

        vkBindImageMemory(vkDevice_, image, imageMemory, 0);
    }

    void Vulkan_Renderer_T01::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            _LOG_ERROR("Vulkan Error: Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        EndSingleTimeCommands(commandBuffer);
    }

    void Vulkan_Renderer_T01::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        EndSingleTimeCommands(commandBuffer);
    }

    void Vulkan_Renderer_T01::GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
    {
        // Check if image format supports linear blitting
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice_, imageFormat, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            _LOG_ERROR("Vulkan Error: Texture image format does not support linear blitting!");
        }

        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;

        for (uint32_t i = 1; i < mipLevels; i++)
        {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit = {};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        EndSingleTimeCommands(commandBuffer);
    }

    void Vulkan_Renderer_T01::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy copyRegion = {};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndSingleTimeCommands(commandBuffer);
    }

    uint32_t Vulkan_Renderer_T01::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice_, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        _LOG_ERROR("Vulkan Error: Failed to find suitable memory type!");

        return (uint32_t)-1;
    }

    VkCommandBuffer Vulkan_Renderer_T01::BeginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vkCommandPool_;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vkDevice_, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void Vulkan_Renderer_T01::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vkGraphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vkGraphicsQueue_);

        vkFreeCommandBuffers(vkDevice_, vkCommandPool_, 1, &commandBuffer);
    }

    void Vulkan_Renderer_T01::DrawFrame()
    {
        vkWaitForFences(vkDevice_, 1, &vkInFlightFences_[currentFrame_], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(vkDevice_, vkSwapChain_, UINT64_MAX, vkImageAvailableSemaphores_[currentFrame_], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            _LOG_ERROR("Vulkan Error: Failed to acquire swap chain image!");
        }

        UpdateUniformBuffer(currentFrame_);

        vkResetFences(vkDevice_, 1, &vkInFlightFences_[currentFrame_]);

        vkResetCommandBuffer(vkCommandBuffers_[currentFrame_], 0); // VkCommandBufferResetFlagBits);
        RecordCommandBuffer(vkCommandBuffers_[currentFrame_], imageIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { vkImageAvailableSemaphores_[currentFrame_] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkCommandBuffers_[currentFrame_];

        VkSemaphore signalSemaphores[] = { vkRenderFinishedSemaphores_[currentFrame_] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vkGraphicsQueue_, 1, &submitInfo, vkInFlightFences_[currentFrame_]) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { vkSwapChain_ };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(vkPresentQueue_, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized_)
        {
            framebufferResized_ = false;
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to present swap chain image!");
        }

        currentFrame_ = (currentFrame_ + 1) % s_maxFramesInFlight_;
    }

    void Vulkan_Renderer_T01::UpdateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo = {};
        ubo.model = glm::identity<glm::mat4>();
        ubo.view = viewMatrix_;
        ubo.proj = projectionMatrix_;

        void* data;
        vkMapMemory(vkDevice_, vkUniformBuffersMemory_[currentImage], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(vkDevice_, vkUniformBuffersMemory_[currentImage]);
    }

    void Vulkan_Renderer_T01::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkRenderPass_;
        renderPassInfo.framebuffer = vkSwapChainFramebuffers_[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vkSwapChainExtent_;

        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);

        // viewport.y is non zero and viewport.height is negative to flip y axis
        // (making it behave like it does in OpenGL)
        // If VK_API_VERSION_1_1 < 1.1: requires VK_KHR_MAINTENANCE_1_EXTENSION_NAME
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = (float)vkSwapChainExtent_.height;
        viewport.width = (float)vkSwapChainExtent_.width;
        viewport.height = -(float)vkSwapChainExtent_.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = vkSwapChainExtent_;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        VkBuffer vertexBuffers[] = { vkVertexBuffer_ };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer_, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout_, 0, 1, &vkDescriptorSets_[currentFrame_], 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, (uint32_t)indices_.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to record command buffer!");
        }
    }

#ifndef NDEBUG
    const bool Vulkan_Renderer_T01::s_enableValidationLayers_ = true;
    const std::vector<const char*> Vulkan_Renderer_T01::s_validationLayers_ =
    {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    const bool Vulkan_Renderer_T01::s_enableValidationLayers_ = false;
    const std::vector<const char*> Vulkan_Renderer_T01::s_validationLayers_ = {};
#endif

    const std::vector<const char*> Vulkan_Renderer_T01::s_deviceExtensions_ =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
        , VK_KHR_MAINTENANCE_1_EXTENSION_NAME
    };

    const uint32_t Vulkan_Renderer_T01::s_maxFramesInFlight_ = 2;

    const std::string Vulkan_Renderer_T01::s_meshPath_ = "../Models/viking_room.obj";

    const std::string Vulkan_Renderer_T01::s_texturePath_ = "../Textures/viking_room.png";
}