#include "Engine/Platform/Vulkan_Renderer.h"

#include "Engine/Logger.h"
#include "Engine/Window.h"

#include <array>
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

    Vulkan_Renderer::Vulkan_Renderer(
        Window* windowPtr,
        unsigned int width,
        unsigned int height,
        bool multisampleAntaiAliasing)
        : windowPtr_(windowPtr)
        , vulkanInstance_(VK_NULL_HANDLE)
        , debugMessenger_(VK_NULL_HANDLE)
        , surface_(VK_NULL_HANDLE)
        , physicalDevice_(VK_NULL_HANDLE)
        , graphicsQueueFamilyIndex_((uint32_t)-1)
        , presentQueueFamilyIndex_((uint32_t)-1)
        , swapChainSurfaceCapabilities_({})
        , swapChainSurfaceFormat_({})
        , swapChainPresentMode_()
        , swapChainExtent_({})
        , swapChainMsaaSamples_(VK_SAMPLE_COUNT_1_BIT)
        , logicalDevice_(VK_NULL_HANDLE)
        , graphicsQueue_(VK_NULL_HANDLE)
        , presentQueue_(VK_NULL_HANDLE)
        , commandPool_(VK_NULL_HANDLE)
        , renderPass_(VK_NULL_HANDLE)
        , descriptorSetLayout_(VK_NULL_HANDLE)
        , swapChain_(VK_NULL_HANDLE)
        , colorImage_(VK_NULL_HANDLE)
        , colorImageMemory_(VK_NULL_HANDLE)
        , colorImageView_(VK_NULL_HANDLE)
        , depthImage_(VK_NULL_HANDLE)
        , depthImageMemory_(VK_NULL_HANDLE)
        , depthImageView_(VK_NULL_HANDLE)
    {
        CreateVulkanInstance();
        SetupDebugMessanger();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();

        CreateCommandPool();

        CreateRenderPass();

        CreateDescriptorSetLayout();

        CreateSwapChain();
        CreateImageViews();
        CreateColorResources();
        CreateDepthResources();
        CreateFramebuffers();


    }

    Vulkan_Renderer:: ~Vulkan_Renderer()
    {
        // CleanupSwapChain

        vkDestroyDescriptorSetLayout(logicalDevice_, descriptorSetLayout_, nullptr);

        vkDestroyRenderPass(logicalDevice_, renderPass_, nullptr);

        vkDestroyCommandPool(logicalDevice_, commandPool_, nullptr);

        vkDestroyDevice(logicalDevice_, nullptr);

        vkDestroySurfaceKHR(vulkanInstance_, surface_, nullptr);

        if (s_enableValidationLayers_)
        {
            PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerFunction =
                (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanInstance_, "vkDestroyDebugUtilsMessengerEXT");
            if (destroyDebugUtilsMessengerFunction != nullptr)
            {
                destroyDebugUtilsMessengerFunction(vulkanInstance_, debugMessenger_, nullptr);
            }
        }

        vkDestroyInstance(vulkanInstance_, nullptr);
    }

    // protected ---------------------------------------------------------------

    void Vulkan_Renderer::CreateVulkanInstance()
    {
        if (s_enableValidationLayers_)
        {
            bool validationLayerMissing = false;

            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayerProperties(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayerProperties.data());

            for (size_t i = 0; i < s_validationLayers_.size(); ++i)
            {
                const char* const& layerName = s_validationLayers_[i];

                bool layerFound = false;

                for (size_t j = 0; j < availableLayerProperties.size(); ++j)
                {
                    const VkLayerProperties& layerProperties = availableLayerProperties[j];
                    if (std::strcmp(layerName, layerProperties.layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound)
                {
                    validationLayerMissing = true;
                    break;
                }
            }

            if (validationLayerMissing)
            {
                _LOG_ERROR("Vulkan Error: Validation layers requested, but not available!");
            }
        }

        const char* windowTitle;
        windowPtr_->GetWindowTitle(windowTitle);

        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = windowTitle;
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pEngineName = "Project001";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = windowPtr_->GetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (s_enableValidationLayers_)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        instanceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT vkDebugUtilsMessengerCreateInfo;
        if (s_enableValidationLayers_)
        {
            instanceCreateInfo.enabledLayerCount = (uint32_t)s_validationLayers_.size();
            instanceCreateInfo.ppEnabledLayerNames = s_validationLayers_.data();

            PopulateDebugMessengerCreateInfo(vkDebugUtilsMessengerCreateInfo);
            instanceCreateInfo.pNext = (void*)&vkDebugUtilsMessengerCreateInfo;
        }
        else
        {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create instance!");
        }
    }

    void Vulkan_Renderer::SetupDebugMessanger()
    {
        if (s_enableValidationLayers_)
        {
            PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerFunction =
                (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanInstance_, "vkCreateDebugUtilsMessengerEXT");
            VkResult result;

            if (createDebugUtilsMessengerFunction != nullptr)
            {
                VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo;
                PopulateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo);
                result = createDebugUtilsMessengerFunction(vulkanInstance_, &debugUtilsMessengerCreateInfo, nullptr, &debugMessenger_);
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
        windowPtr_->CreateWindowSurface(vulkanInstance_, surface_);
    }

    void Vulkan_Renderer::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vulkanInstance_, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            _LOG_ERROR("Vulkan Error: Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkanInstance_, &deviceCount, physicalDevices.data());

        for (size_t i = 0; i < physicalDevices.size(); ++i)
        {
            const VkPhysicalDevice& physicalDevice = physicalDevices[i];

            // Find Required Queue Family Indicies
            // -----------------------------------------------------------------

            bool valid_graphicsQueueFamilyIndex = false;
            uint32_t graphicsQueueFamilyIndex = (uint32_t)-1;

            bool valid_presentQueueFamilyIndex = false;
            uint32_t presentQueueFamilyIndex = (uint32_t)-1;

            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

            for (size_t j = 0; j < queueFamilies.size(); ++j)
            {
                const VkQueueFamilyProperties& queueFamily = queueFamilies[j];

                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    graphicsQueueFamilyIndex = (uint32_t)i;
                    valid_graphicsQueueFamilyIndex = true;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, (uint32_t)i, surface_, &presentSupport);
                if (presentSupport)
                {
                    presentQueueFamilyIndex = (uint32_t)i;
                    valid_presentQueueFamilyIndex = true;
                }

                if (valid_graphicsQueueFamilyIndex && valid_presentQueueFamilyIndex)
                {
                    break;
                }
            }

            if (!valid_graphicsQueueFamilyIndex || !valid_presentQueueFamilyIndex)
            {
                continue;
            }

            // Check Device Extension Support
            // -----------------------------------------------------------------

            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

            std::set<std::string> unfoundExtensions(s_deviceExtensions_.begin(), s_deviceExtensions_.end());

            for (size_t i = 0; i < availableExtensions.size(); ++i)
            {
                const VkExtensionProperties& extension = availableExtensions[i];
                unfoundExtensions.erase(extension.extensionName);
            }

            if (!unfoundExtensions.empty())
            {
                continue;
            }

            // Query Swapchain Support
            // -----------------------------------------------------------------

            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            std::vector<VkPresentModeKHR> presentModes;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface_, &surfaceCapabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, nullptr);

            if (formatCount == 0)
            {
                continue;
            }

            surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, surfaceFormats.data());

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, nullptr);

            if (presentModeCount == 0)
            {
                continue;
            }

            presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, presentModes.data());

            // Query Additional Features
            // -----------------------------------------------------------------

            VkPhysicalDeviceFeatures supportedFeatures;
            vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

            if (!supportedFeatures.samplerAnisotropy)
            {
                continue;
            }

            // This Device is Suitable
            // -----------------------------------------------------------------

            physicalDevice_ = physicalDevice;

            graphicsQueueFamilyIndex_ = graphicsQueueFamilyIndex;
            presentQueueFamilyIndex_ = presentQueueFamilyIndex;

            swapChainSurfaceCapabilities_ = surfaceCapabilities;

            // Select a Swap Chain Surface Format
            // -----------------------------------------------------------------

            bool foundDesiredSurfaceFormat = false;
            for (size_t i = 0; i < surfaceFormats.size(); ++i)
            {
                const VkSurfaceFormatKHR& surfaceFormat = surfaceFormats[i];
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    swapChainSurfaceFormat_ = surfaceFormat;
                    foundDesiredSurfaceFormat = true;
                    break;
                }
            }

            if (!foundDesiredSurfaceFormat)
            {
                swapChainSurfaceFormat_ = surfaceFormats[0];
            }

            // Select a Swap Chain Present Mode
            // -----------------------------------------------------------------

            bool foundDesiredPresentMode = false;
            for (size_t i = 0; i < presentModes.size(); ++i)
            {
                const VkPresentModeKHR& presentMode = presentModes[i];
                if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    swapChainPresentMode_ = presentMode;
                    foundDesiredPresentMode = true;
                    break;
                }
            }

            if (!foundDesiredPresentMode)
            {
                swapChainPresentMode_ = VK_PRESENT_MODE_FIFO_KHR;
            }

            // Select a Swap Chain Extent
            // -----------------------------------------------------------------

            if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                swapChainExtent_ = surfaceCapabilities.currentExtent;
            }
            else
            {
                int width, height;
                windowPtr_->GetFramebufferSize(width, height);

                VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };

                if (actualExtent.width < surfaceCapabilities.minImageExtent.width)
                {
                    actualExtent.width = surfaceCapabilities.minImageExtent.width;
                }
                else if (actualExtent.width > surfaceCapabilities.maxImageExtent.width)
                {
                    actualExtent.width = surfaceCapabilities.maxImageExtent.width;
                }

                if (actualExtent.height < surfaceCapabilities.minImageExtent.height)
                {
                    actualExtent.height = surfaceCapabilities.minImageExtent.height;
                }
                else if (actualExtent.height > surfaceCapabilities.maxImageExtent.height)
                {
                    actualExtent.height = surfaceCapabilities.maxImageExtent.height;
                }

                swapChainExtent_ = actualExtent;
            }

            // Determin Swap Chain Images' MSAA Sample Count
            // -----------------------------------------------------------------

            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(physicalDevice_, &physicalDeviceProperties);

            VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
            if (counts & VK_SAMPLE_COUNT_64_BIT)
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_64_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_32_BIT)
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_32_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_16_BIT)
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_16_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_8_BIT)
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_8_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_4_BIT)
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_4_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_2_BIT)
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_2_BIT;
            }
            else
            {
                swapChainMsaaSamples_ = VK_SAMPLE_COUNT_1_BIT;
            }

            break;
        }

        if (physicalDevice_ == VK_NULL_HANDLE)
        {
            _LOG_ERROR("Vulkan Error: Failed to find a suitable GPU!");
        }
    }

    void Vulkan_Renderer::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { graphicsQueueFamilyIndex_, presentQueueFamilyIndex_ };

        float queuePriority = 1.0f;
        std::set<uint32_t>::iterator iter;
        for (iter = uniqueQueueFamilies.begin(); iter != uniqueQueueFamilies.end(); ++iter)
        {
            const uint32_t& queueFamily = *iter;

            VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
            deviceQueueCreateInfo.queueCount = 1;
            deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

            deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
        }

        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
        physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        deviceCreateInfo.queueCreateInfoCount = (uint32_t)deviceQueueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();

        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        deviceCreateInfo.enabledExtensionCount = (uint32_t)s_deviceExtensions_.size();
        deviceCreateInfo.ppEnabledExtensionNames = s_deviceExtensions_.data();

        if (s_enableValidationLayers_)
        {
            deviceCreateInfo.enabledLayerCount = (uint32_t)s_validationLayers_.size();
            deviceCreateInfo.ppEnabledLayerNames = s_validationLayers_.data();
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice_, &deviceCreateInfo, nullptr, &logicalDevice_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create logical device!");
        }

        vkGetDeviceQueue(logicalDevice_, graphicsQueueFamilyIndex_, 0, &graphicsQueue_);
        vkGetDeviceQueue(logicalDevice_, presentQueueFamilyIndex_, 0, &presentQueue_);
    }

    void Vulkan_Renderer::CreateCommandPool()
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex_;

        if (vkCreateCommandPool(logicalDevice_, &commandPoolCreateInfo, nullptr, &commandPool_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create graphics command pool!");
        }
    }

    void Vulkan_Renderer::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = swapChainSurfaceFormat_.format;
        colorAttachmentDescription.samples = swapChainMsaaSamples_;
        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachmentDescription = {};
        depthAttachmentDescription.format = FindDepthFormat();
        depthAttachmentDescription.samples = swapChainMsaaSamples_;
        depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription resolveAttachmentDescription = {};
        resolveAttachmentDescription.format = swapChainSurfaceFormat_.format;
        resolveAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        resolveAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        resolveAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        resolveAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        resolveAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        resolveAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        resolveAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRefeference = {};
        colorAttachmentRefeference.attachment = 0;
        colorAttachmentRefeference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRefeference = {};
        depthAttachmentRefeference.attachment = 1;
        depthAttachmentRefeference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference resolveAttachmentReference = {};
        resolveAttachmentReference.attachment = 2;
        resolveAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentRefeference;
        subpassDescription.pDepthStencilAttachment = &depthAttachmentRefeference;
        subpassDescription.pResolveAttachments = &resolveAttachmentReference;

        VkSubpassDependency subpssDependency = {};
        subpssDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpssDependency.dstSubpass = 0;
        subpssDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpssDependency.srcAccessMask = 0;
        subpssDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpssDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 3> attachments = { colorAttachmentDescription, depthAttachmentDescription, resolveAttachmentDescription };
        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = (uint32_t)attachments.size();
        renderPassCreateInfo.pAttachments = attachments.data();
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpssDependency;

        if (vkCreateRenderPass(logicalDevice_, &renderPassCreateInfo, nullptr, &renderPass_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create render pass!");
        }
    }

    void Vulkan_Renderer::CreateDescriptorSetLayout()
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
        VkDescriptorSetLayoutCreateInfo descripterSetLayoutCreateInfo = {};
        descripterSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descripterSetLayoutCreateInfo.bindingCount = (uint32_t)bindings.size();
        descripterSetLayoutCreateInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(logicalDevice_, &descripterSetLayoutCreateInfo, nullptr, &descriptorSetLayout_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create descriptor set layout!");
        }
    }

    void Vulkan_Renderer::CreateSwapChain()
    {
        uint32_t imageCount = swapChainSurfaceCapabilities_.minImageCount + 1;
        if (swapChainSurfaceCapabilities_.maxImageCount > 0 &&
            imageCount > swapChainSurfaceCapabilities_.maxImageCount)
        {
            imageCount = swapChainSurfaceCapabilities_.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = surface_;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = swapChainSurfaceFormat_.format;
        swapchainCreateInfo.imageColorSpace = swapChainSurfaceFormat_.colorSpace;
        swapchainCreateInfo.imageExtent = swapChainExtent_;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { graphicsQueueFamilyIndex_, presentQueueFamilyIndex_ };

        if (graphicsQueueFamilyIndex_ != presentQueueFamilyIndex_)
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        swapchainCreateInfo.preTransform = swapChainSurfaceCapabilities_.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = swapChainPresentMode_;
        swapchainCreateInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(logicalDevice_, &swapchainCreateInfo, nullptr, &swapChain_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(logicalDevice_, swapChain_, &imageCount, nullptr);
        swapChainImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice_, swapChain_, &imageCount, swapChainImages_.data());
    }

    void Vulkan_Renderer::CreateImageViews()
    {
        swapChainImageViews_.resize(swapChainImages_.size());

        for (size_t i = 0; i < swapChainImages_.size(); i++)
        {
            swapChainImageViews_[i] = CreateImageView(swapChainImages_[i], swapChainSurfaceFormat_.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    void Vulkan_Renderer::CreateColorResources()
    {
        VkFormat colorVkFormat = swapChainSurfaceFormat_.format;

        CreateImage(swapChainExtent_.width, swapChainExtent_.height, 1, swapChainMsaaSamples_, colorVkFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage_, colorImageMemory_);
        colorImageView_ = CreateImageView(colorImage_, colorVkFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void Vulkan_Renderer::CreateDepthResources()
    {
        VkFormat depthVkFormat = FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        CreateImage(swapChainExtent_.width, swapChainExtent_.height, 1, swapChainMsaaSamples_, depthVkFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage_, depthImageMemory_);
        depthImageView_ = CreateImageView(depthImage_, depthVkFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }

    void Vulkan_Renderer::CreateFramebuffers()
    {
        swapChainFramebuffers_.resize(swapChainImageViews_.size());

        for (size_t i = 0; i < swapChainImageViews_.size(); ++i)
        {
            std::array<VkImageView, 3> attachments = {
                colorImageView_,
                depthImageView_,
                swapChainImageViews_[i]
            };

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = renderPass_;
            framebufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
            framebufferCreateInfo.pAttachments = attachments.data();
            framebufferCreateInfo.width = swapChainExtent_.width;
            framebufferCreateInfo.height = swapChainExtent_.height;
            framebufferCreateInfo.layers = 1;

            if (vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo, nullptr, &swapChainFramebuffers_[i]) != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to create framebuffer!");
            }
        }
    }

    void Vulkan_Renderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugUtilsMessengerCreateInfo)
    {
        debugUtilsMessengerCreateInfo = {};
        debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;
    }

    VkFormat Vulkan_Renderer::FindDepthFormat()
    {
        return FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkFormat Vulkan_Renderer::FindSupportedFormat(
        const std::vector<VkFormat>& formatCandidates,
        VkImageTiling imageTiling,
        VkFormatFeatureFlags formatFeatureFlags)
    {
        for (size_t i = 0; i < formatCandidates.size(); ++i)
        {
            VkFormat format = formatCandidates[i];
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &formatProperties);

            if (imageTiling == VK_IMAGE_TILING_LINEAR &&
                (formatProperties.linearTilingFeatures & formatFeatureFlags) == formatFeatureFlags)
            {
                return format;
            }
            else if (imageTiling == VK_IMAGE_TILING_OPTIMAL &&
                (formatProperties.optimalTilingFeatures & formatFeatureFlags) == formatFeatureFlags)
            {
                return format;
            }
        }

        _LOG_ERROR("Vulkan Error: Failed to find supported format!");
        return VK_FORMAT_UNDEFINED;
    }

    VkImageView Vulkan_Renderer::CreateImageView(
        VkImage image,
        VkFormat format,
        VkImageAspectFlags imageAspectFlags,
        uint32_t mipLevels)
    {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = image;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = format;
        imageViewCreateInfo.subresourceRange.aspectMask = imageAspectFlags;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = mipLevels;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(logicalDevice_, &imageViewCreateInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create texture image view!");
        }

        return imageView;
    }

    void Vulkan_Renderer::CreateImage(
        uint32_t width,
        uint32_t height,
        uint32_t mipLevels,
        VkSampleCountFlagBits sampleCountFlagBits,
        VkFormat format,
        VkImageTiling imageTiling,
        VkImageUsageFlags imageUsageFlags,
        VkMemoryPropertyFlags memoryPropertyFlags,
        VkImage& image,
        VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent.width = width;
        imageCreateInfo.extent.height = height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = mipLevels;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = format;
        imageCreateInfo.tiling = imageTiling;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage = imageUsageFlags;
        imageCreateInfo.samples = sampleCountFlagBits;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(logicalDevice_, &imageCreateInfo, nullptr, &image) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create image!");
        }

        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(logicalDevice_, image, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags);

        if (vkAllocateMemory(logicalDevice_, &memoryAllocateInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate image memory!");
        }

        vkBindImageMemory(logicalDevice_, image, imageMemory, 0);
    }

    uint32_t Vulkan_Renderer::FindMemoryType(
        uint32_t typeFilter,
        VkMemoryPropertyFlags memoryPropertyFlags)
    {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &physicalDeviceMemoryProperties);

        for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) &&
                (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
            {
                return i;
            }
        }

        _LOG_ERROR("Vulkan Error: Failed to find suitable memory type!");

        return (uint32_t)-1;
    }

#ifndef NDEBUG
    const bool Vulkan_Renderer::s_enableValidationLayers_ = true;
    const std::vector<const char*> Vulkan_Renderer::s_validationLayers_ =
    {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    const bool VulkanTutorial_Renderer::s_enableValidationLayers_ = false;
    const std::vector<const char*> VulkanTutorial_Renderer::s_validationLayers_ = {};
#endif

    const std::vector<const char*> Vulkan_Renderer::s_deviceExtensions_ =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}