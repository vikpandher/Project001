#include "Vulkan_Renderer.h"

#include "Engine/Logger.h"
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
            _LOG_ERROR("Vulkan ERROR: %s", pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            _LOG_MESSAGE("Vulkan Warning: %s", pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    // public ------------------------------------------------------------------

    Vulkan_Renderer::Vulkan_Renderer(Window* windowPtr, unsigned int width, unsigned int height)
        : windowPtr_(windowPtr)
        , vkInstance_(VK_NULL_HANDLE)
        , vkDebugMessenger_(VK_NULL_HANDLE)
        , vkSurface_(VK_NULL_HANDLE)
        , vkPhysicalDevice_(VK_NULL_HANDLE)
        , vkDevice_(VK_NULL_HANDLE)
        , vkGraphicsQueue_(VK_NULL_HANDLE)
        , vkPresentQueue_(VK_NULL_HANDLE)
        , vkSwapChain_(VK_NULL_HANDLE)
        , vkRenderPass_(VK_NULL_HANDLE)
        , vkPipelineLayout_(VK_NULL_HANDLE)
        , vkGraphicsPipeline_(VK_NULL_HANDLE)
        , vkCommandPool_(VK_NULL_HANDLE)
        , currentFrame_(0)
        , framebufferResized_(false)
    {
        CreateVulkanInstance();
        SetupDebugMessanger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateGraphicsPipeline();
        CreateFramebuffers();
        CreateCommandPool();
        CreateCommandBuffers();
        CreateSyncObjects();
    }

    Vulkan_Renderer::~Vulkan_Renderer()
    {
        vkDeviceWaitIdle(vkDevice_);

        for (size_t i = 0; i < s_maxFramesInFlight_; ++i)
        {
            vkDestroySemaphore(vkDevice_, vkRenderFinishedSemaphores_[i], nullptr);
            vkDestroySemaphore(vkDevice_, vkImageAvailableSemaphores_[i], nullptr);
            vkDestroyFence(vkDevice_, vkInFlightFences_[i], nullptr);
        }

        vkDestroyCommandPool(vkDevice_, vkCommandPool_, nullptr);

        for (size_t i = 0; i < vkSwapChainFramebuffers_.size(); ++i)
        {
            const VkFramebuffer& framebuffer = vkSwapChainFramebuffers_[i];
            vkDestroyFramebuffer(vkDevice_, framebuffer, nullptr);
        }

        vkDestroyPipeline(vkDevice_, vkGraphicsPipeline_, nullptr);
        vkDestroyPipelineLayout(vkDevice_, vkPipelineLayout_, nullptr);
        vkDestroyRenderPass(vkDevice_, vkRenderPass_, nullptr);

        for (size_t i = 0; i < vkSwapChainImageViews_.size(); ++i)
        {
            const VkImageView& imageView = vkSwapChainImageViews_[i];
            vkDestroyImageView(vkDevice_, imageView, nullptr);
        }

        vkDestroySwapchainKHR(vkDevice_, vkSwapChain_, nullptr);
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

    void Vulkan_Renderer::Render()
    {
        DrawFrame();
    }

    // protected ---------------------------------------------------------------

    void Vulkan_Renderer::CreateVulkanInstance()
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

    void Vulkan_Renderer::SetupDebugMessanger()
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

    void Vulkan_Renderer::CreateSurface()
    {
        windowPtr_->CreateWindowSurface(vkInstance_, vkSurface_);
    }

    void Vulkan_Renderer::PickPhysicalDevice()
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
                break;
            }
        }

        if (vkPhysicalDevice_ == VK_NULL_HANDLE)
        {
            _LOG_ERROR("Vulkan Error: Failed to find a suitable GPU!");
        }
    }

    void Vulkan_Renderer::CreateLogicalDevice()
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

    void Vulkan_Renderer::CreateSwapChain()
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

    void Vulkan_Renderer::CreateImageViews()
    {
        vkSwapChainImageViews_.resize(vkSwapChainImages_.size());

        for (size_t i = 0; i < vkSwapChainImages_.size(); i++)
        {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = vkSwapChainImages_[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = vkSwapChainImageFormat_;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(vkDevice_, &createInfo, nullptr, &vkSwapChainImageViews_[i]) != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to create image views!");
            }
        }
    }

    void Vulkan_Renderer::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = vkSwapChainImageFormat_;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(vkDevice_, &renderPassInfo, nullptr, &vkRenderPass_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create render pass!");
        }
    }

    void Vulkan_Renderer::CreateGraphicsPipeline()
    {
        std::vector<char> vertShaderCode = ReadFile("../Shaders/09_shader_base_vert.spv");
        std::vector<char> fragShaderCode = ReadFile("../Shaders/09_shader_base_frag.spv");

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
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;

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
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

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
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

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

    void Vulkan_Renderer::CreateFramebuffers()
    {
        vkSwapChainFramebuffers_.resize(vkSwapChainImageViews_.size());

        for (size_t i = 0; i < vkSwapChainImageViews_.size(); ++i)
        {
            VkImageView attachments[] = { vkSwapChainImageViews_[i] };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vkRenderPass_;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = vkSwapChainExtent_.width;
            framebufferInfo.height = vkSwapChainExtent_.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(vkDevice_, &framebufferInfo, nullptr, &vkSwapChainFramebuffers_[i]) != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to create framebuffer!");
            }
        }
    }

    void Vulkan_Renderer::CreateCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vkPhysicalDevice_);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamilyIndex;

        if (vkCreateCommandPool(vkDevice_, &poolInfo, nullptr, &vkCommandPool_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create command pool!");
        }
    }

    void Vulkan_Renderer::CreateCommandBuffers()
    {
        vkCommandBuffers_.resize(s_maxFramesInFlight_);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vkCommandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)vkCommandBuffers_.size();

        if (vkAllocateCommandBuffers(vkDevice_, &allocInfo, vkCommandBuffers_.data()) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate command buffers!");
        }
    }

    void Vulkan_Renderer::CreateSyncObjects()
    {
        vkImageAvailableSemaphores_.resize(s_maxFramesInFlight_);
        vkRenderFinishedSemaphores_.resize(s_maxFramesInFlight_);
        vkInFlightFences_.resize(s_maxFramesInFlight_);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
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

    void Vulkan_Renderer::RecreateSwapChain()
    {
        int width = 0, height = 0;
        windowPtr_->GetFramebufferSize(width, height);
        while (width == 0 || height == 0) {
            windowPtr_->SleepAndWaitForEvents();
        }

        vkDeviceWaitIdle(vkDevice_);

        CleanupSwapChain();

        CreateSwapChain();
        CreateImageViews();
        CreateFramebuffers();
    }

    void Vulkan_Renderer::CleanupSwapChain()
    {
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

    void Vulkan_Renderer::DrawFrame()
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

        vkResetFences(vkDevice_, 1, &vkInFlightFences_[currentFrame_]);

        vkResetCommandBuffer(vkCommandBuffers_[currentFrame_], 0); // VkCommandBufferResetFlagBits);
        RecordCommandBuffer(vkCommandBuffers_[currentFrame_], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { vkImageAvailableSemaphores_[currentFrame_]};
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkCommandBuffers_[currentFrame_];

        VkSemaphore signalSemaphores[] = { vkRenderFinishedSemaphores_[currentFrame_]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vkGraphicsQueue_, 1, &submitInfo, vkInFlightFences_[currentFrame_]) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
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

    bool Vulkan_Renderer::CheckValidationLayerSupport()
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

    std::vector<const char*> Vulkan_Renderer::GetRequiredExtensions()
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

    void Vulkan_Renderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = DebugCallback;
    }

    bool Vulkan_Renderer::IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.surfaceFormats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    QueueFamilyIndices Vulkan_Renderer::FindQueueFamilies(VkPhysicalDevice device)
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

    SwapChainSupportDetails Vulkan_Renderer::QuerySwapChainSupport(VkPhysicalDevice device)
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

    VkSurfaceFormatKHR Vulkan_Renderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

    VkPresentModeKHR Vulkan_Renderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
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

    VkExtent2D Vulkan_Renderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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

    bool Vulkan_Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
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

    std::vector<char> Vulkan_Renderer::ReadFile(const std::string& filePath)
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

    VkShaderModule Vulkan_Renderer::CreateShaderModule(const std::vector<char>& code)
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

    void Vulkan_Renderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkRenderPass_;
        renderPassInfo.framebuffer = vkSwapChainFramebuffers_[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vkSwapChainExtent_;

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)vkSwapChainExtent_.width;
        viewport.height = (float)vkSwapChainExtent_.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = vkSwapChainExtent_;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to record command buffer!");
        }
    }

#ifndef NDEBUG
    const bool Vulkan_Renderer::s_enableValidationLayers_ = true;
    const std::vector<const char*> Vulkan_Renderer::s_validationLayers_ = {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    const bool Vulkan_Renderer::s_enableValidationLayers_ = false;
    const std::vector<const char*> s_validationLayers_ = {};
#endif

    const std::vector<const char*> Vulkan_Renderer::s_deviceExtensions_ = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const int Vulkan_Renderer::s_maxFramesInFlight_ = 2;
}