#include "Engine/Platform/Vulkan_Renderer_E01.h"

#include "Engine/Logger.h"
#include "Engine/MeshData.h"
#include "Engine/Window.h"

#include <array>
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

    Vulkan_Renderer_E01::Vulkan_Renderer_E01(
        Window* windowPtr,
        unsigned int width,
        unsigned int height,
        bool multisampleAntaiAliasing)
        : windowPtr_(windowPtr)
        , frameBufferWidth_(width)
        , frameBufferHeight_(height)
        , multisampleAntaiAliasing_(multisampleAntaiAliasing)
        , depthTesting_(true)
        , clearColorValue_({0.0f, 0.0f, 0.0f, 1.0f})
        , viewportX_(0)
        , viewportY_(0)
        , viewportWidth_(width)
        , viewportHeight_(height)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
        , depthTestingChanged_(false)
        , msaaChanged_(false)
        , framebufferResized_(false)
        , vulkanInstance_(VK_NULL_HANDLE)
        , debugMessenger_(VK_NULL_HANDLE)
        , surface_(VK_NULL_HANDLE)
        , physicalDevice_(VK_NULL_HANDLE)
        , graphicsQueueFamilyIndex_((uint32_t)-1)
        , presentQueueFamilyIndex_((uint32_t)-1)
        , surfaceCapabilities_({})
        , surfaceExtent_({})
        , surfaceFormat_({})
        , depthFormat_(VK_FORMAT_UNDEFINED)
        , surfacePresentMode_(VK_PRESENT_MODE_FIFO_KHR)
        , msaaSampleCount_(VK_SAMPLE_COUNT_1_BIT)
        , logicalDevice_(VK_NULL_HANDLE)
        , graphicsQueue_(VK_NULL_HANDLE)
        , presentQueue_(VK_NULL_HANDLE)
        , renderPass_(VK_NULL_HANDLE)
        , swapChain_(VK_NULL_HANDLE)
        , swapChainImageCount_(0)
        , colorImage_(VK_NULL_HANDLE)
        , colorImageMemory_(VK_NULL_HANDLE)
        , colorImageView_(VK_NULL_HANDLE)
        , depthImage_(VK_NULL_HANDLE)
        , depthImageMemory_(VK_NULL_HANDLE)
        , depthImageView_(VK_NULL_HANDLE)
        , descriptorSetLayout_(VK_NULL_HANDLE)
        , pipelineLayout_(VK_NULL_HANDLE)
        , graphicsPipeline_(VK_NULL_HANDLE)
        , imageAvailableSemaphore_(VK_NULL_HANDLE)
        , renderFinishedSemaphore_(VK_NULL_HANDLE)
        , inFlightFence_(VK_NULL_HANDLE)
        , commandPool_(VK_NULL_HANDLE)
        , commandBuffer_(VK_NULL_HANDLE)
        , vertexStagingBuffer_(VK_NULL_HANDLE)
        , vertexStagingBufferMemory_(VK_NULL_HANDLE)
        , vertexStagingBufferDataPtr_(NULL)
        , vertexCount_(0)
        , indexStagingBuffer_(VK_NULL_HANDLE)
        , indexStagingBufferMemory_(VK_NULL_HANDLE)
        , indexStagingBufferDataPtr_(NULL)
        , indexCount_(0)
        , vertexBuffer_(VK_NULL_HANDLE)
        , vertexBufferMemory_(VK_NULL_HANDLE)
        , indexBuffer_(VK_NULL_HANDLE)
        , indexBufferMemory_(VK_NULL_HANDLE)
        , uniformBuffer_(VK_NULL_HANDLE)
        , uniformBufferMemory_(VK_NULL_HANDLE)
        , currentFrame_(0)
        , currentImageIndex_(0)
    {
        CreateVulkanInstance();

        CreateSurface();

        PickPhysicalDevice();

        CreateLogicalDevice();

        CreateRenderPass();

        CreateSwapChain();

        CreateDescriptorSetLayout();

        CreateGraphicsPipeline();

        CreateSyncObjects();

        CreateCommandPool();

        CreateCommandBuffers();

        CreateDataBuffers();
    }

    Vulkan_Renderer_E01::~Vulkan_Renderer_E01()
    {
        vkDeviceWaitIdle(logicalDevice_);

        CleanUpDataBuffers();

        vkDestroyCommandPool(logicalDevice_, commandPool_, nullptr);

        CleanUpSyncObjects();

        CleanUpGraphicsPipeline();

        vkDestroyDescriptorSetLayout(logicalDevice_, descriptorSetLayout_, nullptr);

        CleanUpRenderPass();

        CleanupSwapChain();

        vkDestroyDevice(logicalDevice_, nullptr);

        vkDestroySurfaceKHR(vulkanInstance_, surface_, nullptr);

        CleanUpVulkanInstance();
    }

    void Vulkan_Renderer_E01::BeginRendering()
    {
        vkWaitForFences(logicalDevice_, 1, &inFlightFence_, VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(logicalDevice_, swapChain_, UINT64_MAX, imageAvailableSemaphore_, VK_NULL_HANDLE, &currentImageIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            _LOG_ERROR("Vulkan Error: Failed to acquire swap chain image!");
        }

        vkResetFences(logicalDevice_, 1, &inFlightFence_);
    }

    void Vulkan_Renderer_E01::Clear()
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer_, &beginInfo) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass_;
        renderPassInfo.framebuffer = swapChainFramebuffers_[currentFrame_];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = surfaceExtent_;

        vkCmdBeginRenderPass(commandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        std::vector<VkClearAttachment> clearAttachments;
        clearAttachments.reserve(2);

        VkClearAttachment colorClearAttachment = {};
        colorClearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorClearAttachment.clearValue.color = clearColorValue_;
        colorClearAttachment.colorAttachment = 0;
        clearAttachments.push_back(colorClearAttachment);

        if (depthTesting_)
        {
            VkClearAttachment depthClearAttachment = {};
            depthClearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            depthClearAttachment.clearValue.depthStencil = { 1.0f, 0 };
            clearAttachments.push_back(depthClearAttachment);
        }

        VkClearRect clearRect = {};
        clearRect.layerCount = 1;
        clearRect.rect.offset = { 0, 0 };
        clearRect.rect.extent = { surfaceExtent_.width, surfaceExtent_.height };

        vkCmdClearAttachments(commandBuffer_, clearAttachments.size(), clearAttachments.data(), 1, &clearRect);

        vkCmdEndRenderPass(commandBuffer_);

        if (vkEndCommandBuffer(commandBuffer_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to record command buffer!");
        }

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphore_ };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore_ };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue_, 1, &submitInfo, inFlightFence_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to submit draw command buffer!");
        }
    }

    bool Vulkan_Renderer_E01::AddMesh(
        const MeshVertex* meshVerticies,
        unsigned int meshVertexCount,
        const unsigned int* meshIndicies,
        unsigned int meshIndexCount,
        unsigned int textureId,
        unsigned int specularId,
        const glm::vec3& position,
        const glm::quat& orientation,
        const glm::vec3& scale,
        const glm::vec4& color,
        float shininess,
        bool translucent,
        bool lit)
    {
        return true;

        if ((vertexCount_ + meshVertexCount) > s_vertexBufferCapacity_ ||
            (indexCount_ + meshIndexCount) > s_indexBufferCapacity_)
        {
            if (meshVertexCount > s_vertexBufferCapacity_ ||
                meshIndexCount > s_indexBufferCapacity_)
            {
                _LOG_ERROR("Mesh larger then buffer!");
                return false;
            }

            Render();
        }

        if ((vertexCount_ + meshVertexCount) > s_vertexBufferCapacity_ ||
            (indexCount_ + meshIndexCount) > s_indexBufferCapacity_)
        {
            return false;
        }

        bool getTextureFailed = false;

        float textureUnit = -1.0f;
        // if (textureId != (unsigned int)-1)
        // {
        //     int getTextureUnitResult = GetTextureUnit(textureId, textureUnit);
        //     if (getTextureUnitResult == 1)
        //     {
        //         getTextureFailed = true;
        //     }
        //     else if (getTextureUnitResult == 2)
        //     {
        //         _LOG_ERROR("TextureId %u not found!", textureId);
        //         return false;
        //     }
        // }

        float specularUnit = -1.0f;
        // if (specularId != (unsigned int)-1)
        // {
        //     int getTextureUnitResult = GetTextureUnit(specularId, specularUnit);
        //     if (getTextureUnitResult == 1)
        //     {
        //         getTextureFailed = true;
        //     }
        //     else if (getTextureUnitResult == 2)
        //     {
        //         _LOG_ERROR("SpecularId %u not found!", specularId);
        //         return false;
        //     }
        // }

        // if (getTextureFailed)
        // {
        //     Render();
        //     GetTextureUnit(textureId, textureUnit);
        //     GetTextureUnit(specularId, specularUnit);
        // }

        size_t vertexBufferOffset = vertexCount_;

        for (size_t j = 0; j < meshVertexCount; ++j)
        {
            const MeshVertex& currentMeshVertex = meshVerticies[j];

            VertexData newVertex;
            newVertex.position = currentMeshVertex.position;
            newVertex.textureCoordinate = currentMeshVertex.textureCoordinate;
            newVertex.normal = currentMeshVertex.normal;
            newVertex.color = color;
            newVertex.textureUnit = textureUnit;
            newVertex.specularUnit = specularUnit;
            newVertex.shininess = shininess;
            newVertex.scale = scale;
            newVertex.translation = position;
            newVertex.orientation.x = orientation.x;
            newVertex.orientation.y = orientation.y;
            newVertex.orientation.z = orientation.z;
            newVertex.orientation.w = orientation.w;
            newVertex.lit = lit;

            *(vertexStagingBufferDataPtr_ + vertexCount_++) = newVertex;
        }

        for (unsigned int j = 0; j < meshIndexCount; ++j)
        {
            *(indexStagingBufferDataPtr_ + indexCount_++) = vertexBufferOffset + meshIndicies[j];
        }

        return true;
    }

    void Vulkan_Renderer_E01::Render()
    {
        /*
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer_, &beginInfo) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to begin recording command buffer!");
        }

        // Update Vertex and Index Buffers
        // ---------------------------------------------------------------------

        VkBufferCopy vertexCopyRegion = {};
        vertexCopyRegion.size = vertexCount_;
        vkCmdCopyBuffer(commandBuffer_, vertexStagingBuffer_, vertexBuffer_, 1, &vertexCopyRegion);

        VkBufferCopy indexCopyRegion = {};
        indexCopyRegion.size = indexCount_;
        vkCmdCopyBuffer(commandBuffer_, indexStagingBuffer_, indexBuffer_, 1, &indexCopyRegion);

        // Update Uniform Buffer
        // ---------------------------------------------------------------------

        UniformBufferObject ubo = { viewMatrix_, projectionMatrix_ };
        void* data;
        vkMapMemory(logicalDevice_, uniformBufferMemory_, 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(logicalDevice_, uniformBufferMemory_);

        vkCmdPipelineBarrier(commandBuffer_, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, 0, 0, 0, 0, 0);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass_;
        renderPassInfo.framebuffer = swapChainFramebuffers_[currentFrame_];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = surfaceExtent_;

        */
    }

    void Vulkan_Renderer_E01::SwapBuffers()
    {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        VkSemaphore waitSemaphores[] = { renderFinishedSemaphore_ };
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = waitSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain_ };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &currentImageIndex_;

        VkResult result = vkQueuePresentKHR(presentQueue_, &presentInfo);

        if (depthTestingChanged_ || msaaChanged_)
        {
            depthTestingChanged_ = false;
            msaaChanged_ = false;
            framebufferResized_ = false;
            RecreateRenderPass();
            RecreateSwapChain();
            RecreateGraphicsPipeline();
        } else
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized_)
        {
            framebufferResized_ = false;
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to present swap chain image!");
        }

        currentFrame_ = (currentFrame_ + 1) % swapChainImageCount_;
    }

    // protected ---------------------------------------------------------------

    void Vulkan_Renderer_E01::CreateVulkanInstance()
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

        if (s_enableValidationLayers_)
        {
            VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
            debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;

            instanceCreateInfo.enabledLayerCount = (uint32_t)s_validationLayers_.size();
            instanceCreateInfo.ppEnabledLayerNames = s_validationLayers_.data();
            instanceCreateInfo.pNext = (void*)&debugUtilsMessengerCreateInfo;

            // Setup Debug Messanger
            // -----------------------------------------------------------------

            PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerFunction =
                (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanInstance_, "vkCreateDebugUtilsMessengerEXT");
            VkResult result;

            if (createDebugUtilsMessengerFunction != nullptr)
            {
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

    void Vulkan_Renderer_E01::CleanUpVulkanInstance()
    {
        if (s_enableValidationLayers_)
        {
            PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerFunction =
                (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanInstance_, "vkDestroyDebugUtilsMessengerEXT");
            if (destroyDebugUtilsMessengerFunction != nullptr)
            {
                destroyDebugUtilsMessengerFunction(vulkanInstance_, debugMessenger_, nullptr);
                debugMessenger_ == VK_NULL_HANDLE;
            }
        }

        vkDestroyInstance(vulkanInstance_, nullptr);
    }

    void Vulkan_Renderer_E01::CreateSurface()
    {
        windowPtr_->CreateWindowSurface(vulkanInstance_, surface_);
    }

    void Vulkan_Renderer_E01::PickPhysicalDevice()
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

            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            std::vector<VkPresentModeKHR> presentModes;

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

            if (s_requireSamplerAnisotropy_ && !supportedFeatures.samplerAnisotropy)
            {
                continue;
            }

            // This Device is Suitable
            // -----------------------------------------------------------------
            // -----------------------------------------------------------------

            physicalDevice_ = physicalDevice;

            graphicsQueueFamilyIndex_ = graphicsQueueFamilyIndex;
            presentQueueFamilyIndex_ = presentQueueFamilyIndex;

            // Get Surface Capabilities
            // -----------------------------------------------------------------

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface_, &surfaceCapabilities_);

            // Get the Swap Chain Surface Extent
            // -----------------------------------------------------------------

            if (surfaceCapabilities_.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                surfaceExtent_ = surfaceCapabilities_.currentExtent;
                viewportHeight_ = (unsigned int)surfaceExtent_.height;
                viewportWidth_ = (unsigned int)surfaceExtent_.width;
            }
            else
            {
                int width, height;
                windowPtr_->GetFramebufferSize(width, height);

                VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };

                if (actualExtent.width < surfaceCapabilities_.minImageExtent.width)
                {
                    actualExtent.width = surfaceCapabilities_.minImageExtent.width;
                }
                else if (actualExtent.width > surfaceCapabilities_.maxImageExtent.width)
                {
                    actualExtent.width = surfaceCapabilities_.maxImageExtent.width;
                }

                if (actualExtent.height < surfaceCapabilities_.minImageExtent.height)
                {
                    actualExtent.height = surfaceCapabilities_.minImageExtent.height;
                }
                else if (actualExtent.height > surfaceCapabilities_.maxImageExtent.height)
                {
                    actualExtent.height = surfaceCapabilities_.maxImageExtent.height;
                }

                surfaceExtent_ = actualExtent;
            }

            // Select a Swap Chain Surface Format
            // -----------------------------------------------------------------

            bool foundDesiredSurfaceFormat = false;
            for (size_t i = 0; i < surfaceFormats.size(); ++i)
            {
                const VkSurfaceFormatKHR& surfaceFormat = surfaceFormats[i];
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                    surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    surfaceFormat_ = surfaceFormat;
                    foundDesiredSurfaceFormat = true;
                    break;
                }
            }

            if (!foundDesiredSurfaceFormat)
            {
                _LOG_ERROR("Vulkan Error: Failed to find desired surface format!");
                surfaceFormat_ = surfaceFormats[0];
            }

            // Select a Depth Format
            // -----------------------------------------------------------------

            std::array<VkFormat, 3> formatCandidates = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
            VkImageTiling imageTiling = VK_IMAGE_TILING_OPTIMAL;
            VkFormatFeatureFlagBits formatFeatureFlags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

            bool foundDepthFormat = false;
            for (size_t i = 0; i < formatCandidates.size(); ++i)
            {
                VkFormat format = formatCandidates[i];
                VkFormatProperties formatProperties;
                vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &formatProperties);

                if (imageTiling == VK_IMAGE_TILING_LINEAR &&
                    (formatProperties.linearTilingFeatures & formatFeatureFlags) == formatFeatureFlags)
                {
                    depthFormat_ = format;
                    foundDepthFormat = true;
                    break;
                }
                else if (imageTiling == VK_IMAGE_TILING_OPTIMAL &&
                    (formatProperties.optimalTilingFeatures & formatFeatureFlags) == formatFeatureFlags)
                {
                    depthFormat_ = format;
                    foundDepthFormat = true;
                    break;
                }
            }

            if (!foundDepthFormat)
            {
                _LOG_ERROR("Vulkan Error: Failed to find desired surface depth format!");
                depthFormat_ = formatCandidates[0];
            }

            // Select a Swap Chain Present Mode
            // -----------------------------------------------------------------

            bool foundDesiredPresentMode = false;
            for (size_t i = 0; i < presentModes.size(); ++i)
            {
                const VkPresentModeKHR& presentMode = presentModes[i];
                if (presentMode == VK_PRESENT_MODE_FIFO_KHR)
                {
                    surfacePresentMode_ = presentMode;
                    foundDesiredPresentMode = true;
                    break;
                }
            }

            if (!foundDesiredPresentMode)
            {
                _LOG_ERROR("Vulkan Error: Failed to find desired surface present mode!");
                surfacePresentMode_ = presentModes[0];
            }

            // Determin Swap Chain Images' MSAA Sample Count
            // -----------------------------------------------------------------

            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(physicalDevice_, &physicalDeviceProperties);

            VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
            if (counts & VK_SAMPLE_COUNT_64_BIT)
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_64_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_32_BIT)
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_32_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_16_BIT)
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_16_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_8_BIT)
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_8_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_4_BIT)
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_4_BIT;
            }
            else if (counts & VK_SAMPLE_COUNT_2_BIT)
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_2_BIT;
            }
            else
            {
                msaaSampleCount_ = VK_SAMPLE_COUNT_1_BIT;
            }

            break;
        }

        if (physicalDevice_ == VK_NULL_HANDLE)
        {
            _LOG_ERROR("Vulkan Error: Failed to find a suitable GPU!");
        }
    }

    void Vulkan_Renderer_E01::CreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { graphicsQueueFamilyIndex_, presentQueueFamilyIndex_ };

        float queuePriority = 1.0f;
        std::set<uint32_t>::iterator iter;
        for (iter = uniqueQueueFamilies.begin(); iter != uniqueQueueFamilies.end(); ++iter)
        {
            const uint32_t& queueFamilyIndex = *iter;

            VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
            deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            deviceQueueCreateInfo.queueCount = 1;
            deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

            deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
        }

        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
        if (s_requireSamplerAnisotropy_)
        {
            physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
        }

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

    void Vulkan_Renderer_E01::CreateRenderPass()
    {
        VkSampleCountFlagBits samples = multisampleAntaiAliasing_ ? msaaSampleCount_ : VK_SAMPLE_COUNT_1_BIT;

        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = surfaceFormat_.format;
        colorAttachmentDescription.samples = samples;
        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        if (multisampleAntaiAliasing_)
        {
            colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        else
        {
            colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }

        VkAttachmentDescription depthAttachmentDescription = {};
        depthAttachmentDescription.format = depthFormat_;
        depthAttachmentDescription.samples = samples;
        depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription resolveAttachmentDescription = {};
        resolveAttachmentDescription.format = surfaceFormat_.format;
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
        if (depthTesting_)
        {
            resolveAttachmentReference.attachment = 2;
        }
        else
        {
            resolveAttachmentReference.attachment = 1;
        }
        resolveAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentRefeference;
        if (depthTesting_)
        {
            subpassDescription.pDepthStencilAttachment = &depthAttachmentRefeference;
        }
        if (multisampleAntaiAliasing_)
        {
            subpassDescription.pResolveAttachments = &resolveAttachmentReference;
        }

        VkSubpassDependency subpssDependency = {};
        subpssDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpssDependency.dstSubpass = 0;
        subpssDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpssDependency.srcAccessMask = 0;
        subpssDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpssDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        std::vector<VkAttachmentDescription> attachments;
        attachments.reserve(3);
        attachments.push_back(colorAttachmentDescription);
        if (depthTesting_)
        {
            subpssDependency.srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpssDependency.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpssDependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            attachments.push_back(depthAttachmentDescription);
        }
        if (multisampleAntaiAliasing_)
        {
            attachments.push_back(resolveAttachmentDescription);
        }

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

    void Vulkan_Renderer_E01::CleanUpRenderPass()
    {
        vkDestroyRenderPass(logicalDevice_, renderPass_, nullptr);
        renderPass_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer_E01::RecreateRenderPass()
    {
        VkResult result = vkDeviceWaitIdle(logicalDevice_);
        if (result != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to wait for a device to become idle!");
        }

        CleanUpRenderPass();
        CreateRenderPass();
    }

    void Vulkan_Renderer_E01::CreateSwapChain()
    {
        swapChainImageCount_ = surfaceCapabilities_.minImageCount + 1;
        if (surfaceCapabilities_.maxImageCount > 0 &&
            swapChainImageCount_ > surfaceCapabilities_.maxImageCount)
        {
            swapChainImageCount_ = surfaceCapabilities_.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = surface_;
        swapchainCreateInfo.minImageCount = swapChainImageCount_;
        swapchainCreateInfo.imageFormat = surfaceFormat_.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat_.colorSpace;
        swapchainCreateInfo.imageExtent = surfaceExtent_;
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

        swapchainCreateInfo.preTransform = surfaceCapabilities_.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = surfacePresentMode_;
        swapchainCreateInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(logicalDevice_, &swapchainCreateInfo, nullptr, &swapChain_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(logicalDevice_, swapChain_, &swapChainImageCount_, nullptr);
        swapChainImages_.resize(swapChainImageCount_);
        vkGetSwapchainImagesKHR(logicalDevice_, swapChain_, &swapChainImageCount_, swapChainImages_.data());

        swapChainImageViews_.resize(swapChainImageCount_);
        for (size_t i = 0; i < swapChainImages_.size(); i++)
        {
            swapChainImageViews_[i] = CreateImageView(swapChainImages_[i], surfaceFormat_.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }

        VkSampleCountFlagBits samples = multisampleAntaiAliasing_ ? msaaSampleCount_ : VK_SAMPLE_COUNT_1_BIT;

        if (multisampleAntaiAliasing_)
        {
            CreateImage(surfaceExtent_.width, surfaceExtent_.height, 1, samples, surfaceFormat_.format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage_, colorImageMemory_);
            colorImageView_ = CreateImageView(colorImage_, surfaceFormat_.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }

        if (depthTesting_)
        {
            CreateImage(surfaceExtent_.width, surfaceExtent_.height, 1, samples, depthFormat_, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage_, depthImageMemory_);
            depthImageView_ = CreateImageView(depthImage_, depthFormat_, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        }

        swapChainFramebuffers_.resize(swapChainImageCount_);
        for (size_t i = 0; i < swapChainImageViews_.size(); ++i)
        {
            std::vector<VkImageView> attachments;
            attachments.reserve(3);
            if (multisampleAntaiAliasing_)
            {
                attachments.push_back(colorImageView_);
                if (depthTesting_)
                {
                    attachments.push_back(depthImageView_);
                }
                attachments.push_back(swapChainImageViews_[i]);
            }
            else
            {
                attachments.push_back(swapChainImageViews_[i]);
                if (depthTesting_)
                {
                    attachments.push_back(depthImageView_);
                }
            }

            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = renderPass_;
            framebufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
            framebufferCreateInfo.pAttachments = attachments.data();
            framebufferCreateInfo.width = surfaceExtent_.width;
            framebufferCreateInfo.height = surfaceExtent_.height;
            framebufferCreateInfo.layers = 1;

            if (vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo, nullptr, &swapChainFramebuffers_[i]) != VK_SUCCESS)
            {
                _LOG_ERROR("Vulkan Error: Failed to create framebuffer!");
            }
        }
    }

    void Vulkan_Renderer_E01::CleanupSwapChain()
    {
        for (size_t i = 0; i < swapChainFramebuffers_.size(); ++i)
        {
            const VkFramebuffer& framebuffer = swapChainFramebuffers_[i];
            vkDestroyFramebuffer(logicalDevice_, framebuffer, nullptr);
        }
        swapChainFramebuffers_.clear();

        if (depthImageView_ != VK_NULL_HANDLE && depthImage_ != VK_NULL_HANDLE && depthImageMemory_ != VK_NULL_HANDLE)
        {
            vkDestroyImageView(logicalDevice_, depthImageView_, nullptr);
            vkDestroyImage(logicalDevice_, depthImage_, nullptr);
            vkFreeMemory(logicalDevice_, depthImageMemory_, nullptr);

            depthImageView_ = VK_NULL_HANDLE;
            depthImage_ = VK_NULL_HANDLE;
            depthImageMemory_ = VK_NULL_HANDLE;
        }

        if (colorImageView_ != VK_NULL_HANDLE && colorImage_ != VK_NULL_HANDLE && colorImageMemory_ != VK_NULL_HANDLE)
        {
            vkDestroyImageView(logicalDevice_, colorImageView_, nullptr);
            vkDestroyImage(logicalDevice_, colorImage_, nullptr);
            vkFreeMemory(logicalDevice_, colorImageMemory_, nullptr);

            colorImageView_ = VK_NULL_HANDLE;
            colorImage_ = VK_NULL_HANDLE;
            colorImageMemory_ = VK_NULL_HANDLE;
        }

        for (size_t i = 0; i < swapChainImageViews_.size(); ++i)
        {
            const VkImageView& imageView = swapChainImageViews_[i];
            vkDestroyImageView(logicalDevice_, imageView, nullptr);
        }
        swapChainImageViews_.clear();

        vkDestroySwapchainKHR(logicalDevice_, swapChain_, nullptr);
        swapChain_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer_E01::RecreateSwapChain()
    {
        int width = 0, height = 0;
        windowPtr_->GetFramebufferSize(width, height);
        frameBufferWidth_ = (unsigned int)width;
        frameBufferHeight_ = (unsigned int)height;
        // while (width == 0 || height == 0)
        // {
        //     windowPtr_->SleepAndWaitForEvents();
        // }

        VkResult result = vkDeviceWaitIdle(logicalDevice_);
        if (result != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to wait for a device to become idle!");
        }

        CleanupSwapChain();
        CreateSwapChain();
    }

    void Vulkan_Renderer_E01::CreateDescriptorSetLayout()
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

    void Vulkan_Renderer_E01::CreateGraphicsPipeline()
    {
        std::vector<char> vertShaderCode = ReadFile("../Shaders/batchShader_vert.spv");
        std::vector<char> fragShaderCode = ReadFile("../Shaders/batchShader_frag.spv");

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

        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexData);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::array<VkVertexInputAttributeDescription, 11> attributeDescriptions = {};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexData, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexData, textureCoordinate);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(VertexData, normal);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(VertexData, color);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(VertexData, textureUnit);

        attributeDescriptions[5].binding = 0;
        attributeDescriptions[5].location = 5;
        attributeDescriptions[5].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[5].offset = offsetof(VertexData, specularUnit);

        attributeDescriptions[6].binding = 0;
        attributeDescriptions[6].location = 6;
        attributeDescriptions[6].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[6].offset = offsetof(VertexData, shininess);

        attributeDescriptions[7].binding = 0;
        attributeDescriptions[7].location = 7;
        attributeDescriptions[7].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[7].offset = offsetof(VertexData, scale);

        attributeDescriptions[8].binding = 0;
        attributeDescriptions[8].location = 8;
        attributeDescriptions[8].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[8].offset = offsetof(VertexData, translation);

        attributeDescriptions[9].binding = 0;
        attributeDescriptions[9].location = 9;
        attributeDescriptions[9].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[9].offset = offsetof(VertexData, orientation);

        attributeDescriptions[10].binding = 0;
        attributeDescriptions[10].location = 10;
        attributeDescriptions[10].format = VK_FORMAT_R32_SFLOAT;
        attributeDescriptions[10].offset = offsetof(VertexData, lit);

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

        VkSampleCountFlagBits samples = multisampleAntaiAliasing_ ? msaaSampleCount_ : VK_SAMPLE_COUNT_1_BIT;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = samples;

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
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;

        if (vkCreatePipelineLayout(logicalDevice_, &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
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
        if (depthTesting_)
        {
            pipelineInfo.pDepthStencilState = &depthStencil;
        }
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout_;
        pipelineInfo.renderPass = renderPass_;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(logicalDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(logicalDevice_, fragShaderModule, nullptr);
        vkDestroyShaderModule(logicalDevice_, vertShaderModule, nullptr);
    }

    void Vulkan_Renderer_E01::CleanUpGraphicsPipeline()
    {
        vkDestroyPipeline(logicalDevice_, graphicsPipeline_, nullptr);
        graphicsPipeline_ = VK_NULL_HANDLE;

        vkDestroyPipelineLayout(logicalDevice_, pipelineLayout_, nullptr);
        pipelineLayout_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer_E01::RecreateGraphicsPipeline()
    {
        VkResult result = vkDeviceWaitIdle(logicalDevice_);
        if (result != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to wait for a device to become idle!");
        }

        CleanUpGraphicsPipeline();
        CreateGraphicsPipeline();
    }

    void Vulkan_Renderer_E01::CreateSyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if (vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &imageAvailableSemaphore_) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &renderFinishedSemaphore_) != VK_SUCCESS ||
            vkCreateFence(logicalDevice_, &fenceInfo, nullptr, &inFlightFence_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create synchronization objects for a frame!");
        }
    }

    void Vulkan_Renderer_E01::CleanUpSyncObjects()
    {
        vkDestroySemaphore(logicalDevice_, imageAvailableSemaphore_, nullptr);
        vkDestroySemaphore(logicalDevice_, renderFinishedSemaphore_, nullptr);
        vkDestroyFence(logicalDevice_, inFlightFence_, nullptr);
    }

    void Vulkan_Renderer_E01::CreateCommandPool()
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

    void Vulkan_Renderer_E01::CreateCommandBuffers()
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(logicalDevice_, &allocInfo, &commandBuffer_) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate command buffers!");
        }
    }

    void Vulkan_Renderer_E01::CreateDataBuffers()
    {
        VkDeviceSize vertexBufferSize = sizeof(VertexData) * s_vertexBufferCapacity_;
        VkDeviceSize indexBufferSize = sizeof(uint32_t) * s_indexBufferCapacity_;

        CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexStagingBuffer_, vertexStagingBufferMemory_);
        vkMapMemory(logicalDevice_, vertexStagingBufferMemory_, 0, vertexBufferSize, 0, (void**)&vertexStagingBufferDataPtr_);

        CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexStagingBuffer_, indexStagingBufferMemory_);
        vkMapMemory(logicalDevice_, indexStagingBufferMemory_, 0, indexBufferSize, 0, (void**)&indexStagingBufferDataPtr_);

        CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer_, vertexBufferMemory_);

        CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer_, indexBufferMemory_);

        CreateBuffer(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer_, uniformBufferMemory_);
    }

    void Vulkan_Renderer_E01::CleanUpDataBuffers()
    {
        vkUnmapMemory(logicalDevice_, vertexStagingBufferMemory_);
        vkDestroyBuffer(logicalDevice_, vertexStagingBuffer_, nullptr);
        vkFreeMemory(logicalDevice_, vertexStagingBufferMemory_, nullptr);

        vkUnmapMemory(logicalDevice_, indexStagingBufferMemory_);
        vkDestroyBuffer(logicalDevice_, indexStagingBuffer_, nullptr);
        vkFreeMemory(logicalDevice_, indexStagingBufferMemory_, nullptr);

        vkDestroyBuffer(logicalDevice_, vertexBuffer_, nullptr);
        vkFreeMemory(logicalDevice_, vertexBufferMemory_, nullptr);

        vkDestroyBuffer(logicalDevice_, indexBuffer_, nullptr);
        vkFreeMemory(logicalDevice_, indexBufferMemory_, nullptr);

        vkDestroyBuffer(logicalDevice_, uniformBuffer_, nullptr);
        vkFreeMemory(logicalDevice_, uniformBufferMemory_, nullptr);
    }

    VkImageView Vulkan_Renderer_E01::CreateImageView(
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
            _LOG_ERROR("Vulkan Error: Failed to create image view!");
        }

        return imageView;
    }

    void Vulkan_Renderer_E01::CreateImage(
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

    void Vulkan_Renderer_E01::CreateBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(logicalDevice_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(logicalDevice_, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(logicalDevice_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(logicalDevice_, buffer, bufferMemory, 0);
    }

    uint32_t Vulkan_Renderer_E01::FindMemoryType(
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

    std::vector<char> Vulkan_Renderer_E01::ReadFile(const char* filePath)
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

    VkShaderModule Vulkan_Renderer_E01::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(logicalDevice_, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to create shader module!");
        }

        return shaderModule;
    }

    // VVV --- UNUSED --- VVV

    VkCommandBuffer Vulkan_Renderer_E01::BeginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool_;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(logicalDevice_, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void Vulkan_Renderer_E01::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue_);

        vkFreeCommandBuffers(logicalDevice_, commandPool_, 1, &commandBuffer);
    }

#ifndef NDEBUG
    const bool Vulkan_Renderer_E01::s_enableValidationLayers_ = true;
    const std::vector<const char*> Vulkan_Renderer_E01::s_validationLayers_ =
    {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    const bool VulkanTutorial_Renderer::s_enableValidationLayers_ = false;
    const std::vector<const char*> VulkanTutorial_Renderer::s_validationLayers_ = {};
#endif

    const std::vector<const char*> Vulkan_Renderer_E01::s_deviceExtensions_ =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const bool Vulkan_Renderer_E01::s_requireSamplerAnisotropy_ = true;

    const unsigned int Vulkan_Renderer_E01::s_indexBufferCapacity_ = 4194304; // 8192;
    const unsigned int Vulkan_Renderer_E01::s_vertexBufferCapacity_ = 4194304; // 6144;
}