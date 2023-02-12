#include "Vulkan_Renderer_T02.h"

#include "Engine/Platform/Vulkan_Error.h"
#include "Engine/MeshData.h"
#include "Engine/Window.h"

#include <array>
#include <fstream>
#include <set>
#include <vector>



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

    Vulkan_Renderer_T02::Vulkan_Renderer_T02(
        Window* windowPtr,
        unsigned int width,
        unsigned int height,
        bool multisampleAntaiAliasing)
        : windowPtr_(windowPtr)
        , frameBufferWidth_(width)
        , frameBufferHeight_(height)
        , multisampleAntaiAliasing_(multisampleAntaiAliasing)
        , depthTesting_(true)
        , viewportX_(0)
        , viewportY_(0)
        , viewportWidth_(width)
        , viewportHeight_(height)
        , borderColor_(0.1f, 0.1f, 0.1f, 1.0f)
        , clearColor_(0.0f, 0.0f, 0.0f, 1.0f)
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
        , swapchain_(VK_NULL_HANDLE)
        , swapchainImageCount_(0)
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
        , descriptorSetLayout_(VK_NULL_HANDLE)
        , descriptorPool_(VK_NULL_HANDLE)
        , descriptorSet_(VK_NULL_HANDLE)
        , renderPass_(VK_NULL_HANDLE)
        , pipelineLayout_(VK_NULL_HANDLE)
        , graphicsPipeline_(VK_NULL_HANDLE)
        , commandPool_(VK_NULL_HANDLE)
        , commandBuffer_(VK_NULL_HANDLE)
        , mainFence_(VK_NULL_HANDLE)
        , currentSwapchainFramebufferIndex_(0)
    {
        CreateVulkanInstance();

        CreateSurface();

        PickPhysicalDevice();

        CreateLogicalDevice();

        CreateSwapChain();

        CreateDataBuffers();

        CreateDescriptorSetLayout();

        CreateDescriptorPool();

        CreateDescriptorSets();

        CreateRenderPass();

        CreateGraphicsPipeline();

        CreateCommandPool();

        CreateCommandBuffers();

        CreateSyncObjects();

        VkResult result = vkAcquireNextImageKHR(logicalDevice_, swapchain_, UINT64_MAX, VK_NULL_HANDLE, mainFence_, &currentSwapchainFramebufferIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            // RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            _LOG_ERROR("Vulkan Error: Failed to acquire swap chain image!");
        }

        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo2 = {};
        beginInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo2));

        VkImageMemoryBarrier memoryBarrier = {};
        memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        memoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memoryBarrier.image = swapchainImages_[currentSwapchainFramebufferIndex_];
        memoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        memoryBarrier.subresourceRange.baseMipLevel = 0;
        memoryBarrier.subresourceRange.levelCount = 1;
        memoryBarrier.subresourceRange.baseArrayLayer = 0;
        memoryBarrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(
            commandBuffer_,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &memoryBarrier
        );

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));
    }

    Vulkan_Renderer_T02::~Vulkan_Renderer_T02()
    {
        vkDeviceWaitIdle(logicalDevice_);

        DeleteSyncObjects();

        DeleteCommandPool();

        DeleteGraphicsPipeline();

        DeleteRenderPass();

        DeleteDescriptorPool();

        DeleteDescriptorSetLayout();

        DeleteDataBuffers();

        DeleteSwapChain();

        DeleteLogicalDevice();

        DeleteSurface();

        DeleteVulkanInstance();
    }

    void Vulkan_Renderer_T02::BeginRendering()
    {

    }

    void Vulkan_Renderer_T02::Clear()
    {
        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass_;
        renderPassInfo.framebuffer = swapchainFramebuffers_[currentSwapchainFramebufferIndex_];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = surfaceExtent_;

        vkCmdBeginRenderPass(commandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkClearAttachment colorClearAttachment = {};
        colorClearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorClearAttachment.clearValue.color = { clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a };
        colorClearAttachment.colorAttachment = 0;

        VkClearRect clearRect = {};
        clearRect.layerCount = 1;
        clearRect.rect.offset = { 0, 0 };
        clearRect.rect.extent = { surfaceExtent_.width, surfaceExtent_.height };

        vkCmdClearAttachments(commandBuffer_, 1, &colorClearAttachment, 1, &clearRect);

        vkCmdEndRenderPass(commandBuffer_);

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));
    }

    bool Vulkan_Renderer_T02::AddMesh(
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

    void Vulkan_Renderer_T02::Render()
    {
        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));

        if (vertexCount_ > 0)
        {
            VkBufferCopy vertexCopyRegion = {};
            vertexCopyRegion.size = sizeof(VertexData) * vertexCount_;
            vkCmdCopyBuffer(commandBuffer_, vertexStagingBuffer_, vertexBuffer_, 1, &vertexCopyRegion);
        }

        if (indexCount_ > 0)
        {
            VkBufferCopy indexCopyRegion = {};
            indexCopyRegion.size = sizeof(uint32_t) * indexCount_;
            vkCmdCopyBuffer(commandBuffer_, indexStagingBuffer_, indexBuffer_, 1, &indexCopyRegion);
        }

        UniformBufferObject ubo = { viewMatrix_, projectionMatrix_ };
        void* data;
        vkMapMemory(logicalDevice_, uniformBufferMemory_, 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(logicalDevice_, uniformBufferMemory_);

        VkMemoryBarrier memoryBarrier = {};
        memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        memoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        memoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

        vkCmdPipelineBarrier(
            commandBuffer_,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            1, &memoryBarrier,
            0, nullptr,
            0, nullptr
        );

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass_;
        renderPassInfo.framebuffer = swapchainFramebuffers_[currentSwapchainFramebufferIndex_];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = surfaceExtent_;

        vkCmdBeginRenderPass(commandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);

        // viewport.y is non zero and viewport.height is negative to flip y axis
        // (making it behave like it does in OpenGL)
        // If VK_API_VERSION_1_1 < 1.1: requires VK_KHR_MAINTENANCE_1_EXTENSION_NAME
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = (float)viewportHeight_;
        viewport.width = (float)viewportWidth_;
        viewport.height = -(float)viewportHeight_;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer_, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = surfaceExtent_;
        vkCmdSetScissor(commandBuffer_, 0, 1, &scissor);

        VkBuffer vertexBuffers[] = { vertexBuffer_ };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer_, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer_, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_, 0, 1, &descriptorSet_, 0, nullptr);

        vkCmdDrawIndexed(commandBuffer_, indexCount_, 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer_);

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        vertexCount_ = 0;
        indexCount_ = 0;

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));
    }

    void Vulkan_Renderer_T02::FinishRendering()
    {

    }

    void Vulkan_Renderer_T02::SwapBuffers()
    {
        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo1 = {};
        beginInfo1.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo1));

        VkImageMemoryBarrier memoryBarrier1 = {};
        memoryBarrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        memoryBarrier1.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        memoryBarrier1.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        memoryBarrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memoryBarrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memoryBarrier1.image = swapchainImages_[currentSwapchainFramebufferIndex_];
        memoryBarrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        memoryBarrier1.subresourceRange.baseMipLevel = 0;
        memoryBarrier1.subresourceRange.levelCount = 1;
        memoryBarrier1.subresourceRange.baseArrayLayer = 0;
        memoryBarrier1.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(
            commandBuffer_,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &memoryBarrier1
        );

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo1 = {};
        submitInfo1.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo1.commandBufferCount = 1;
        submitInfo1.pCommandBuffers = &commandBuffer_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo1, mainFence_));

        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        VkSwapchainKHR swapChains[] = { swapchain_ };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &currentSwapchainFramebufferIndex_;

        VkResult result = vkQueuePresentKHR(presentQueue_, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized_)
        {
            framebufferResized_ = false;
            // RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            _LOG_ERROR("Vulkan Error: Failed to present swap chain image!");
        }

        result = vkAcquireNextImageKHR(logicalDevice_, swapchain_, UINT64_MAX, VK_NULL_HANDLE, mainFence_, &currentSwapchainFramebufferIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            // RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            _LOG_ERROR("Vulkan Error: Failed to acquire swap chain image!");
        }

        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo2 = {};
        beginInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo2));

        VkImageMemoryBarrier memoryBarrier2 = {};
        memoryBarrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        memoryBarrier2.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        memoryBarrier2.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        memoryBarrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memoryBarrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        memoryBarrier2.image = swapchainImages_[currentSwapchainFramebufferIndex_];
        memoryBarrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        memoryBarrier2.subresourceRange.baseMipLevel = 0;
        memoryBarrier2.subresourceRange.levelCount = 1;
        memoryBarrier2.subresourceRange.baseArrayLayer = 0;
        memoryBarrier2.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(
            commandBuffer_,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &memoryBarrier2
        );

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo2 = {};
        submitInfo2.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo2.commandBufferCount = 1;
        submitInfo2.pCommandBuffers = &commandBuffer_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo2, mainFence_));
    }

    // protected ---------------------------------------------------------------

    void Vulkan_Renderer_T02::CreateVulkanInstance()
    {
        if (s_enableValidationLayers_)
        {
            bool validationLayerMissing = false;

            uint32_t layerCount;
            _VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

            std::vector<VkLayerProperties> availableLayerProperties(layerCount);
            _VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayerProperties.data()));

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

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
        debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;

        if (s_enableValidationLayers_)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            instanceCreateInfo.enabledLayerCount = (uint32_t)s_validationLayers_.size();
            instanceCreateInfo.ppEnabledLayerNames = s_validationLayers_.data();
            instanceCreateInfo.pNext = (void*)&debugUtilsMessengerCreateInfo;
        }
        else
        {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        instanceCreateInfo.enabledExtensionCount = (uint32_t)extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        _VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance_));

        if (s_enableValidationLayers_)
        {
            PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerFunction =
                (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanInstance_, "vkCreateDebugUtilsMessengerEXT");

            if (createDebugUtilsMessengerFunction != nullptr)
            {
                _VK_CHECK(createDebugUtilsMessengerFunction(vulkanInstance_, &debugUtilsMessengerCreateInfo, nullptr, &debugMessenger_));
            }
            else
            {
                _LOG_ERROR("Vulkan Error: Failed to find CreateDebugUtilsMessengerEXT funciton!");
            }
        }
    }

    void Vulkan_Renderer_T02::DeleteVulkanInstance()
    {
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

    void Vulkan_Renderer_T02::CreateSurface()
    {
        windowPtr_->CreateWindowSurface(vulkanInstance_, surface_);
    }

    void Vulkan_Renderer_T02::DeleteSurface()
    {
        vkDestroySurfaceKHR(vulkanInstance_, surface_, nullptr);
    }

    void Vulkan_Renderer_T02::PickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        _VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance_, &deviceCount, nullptr));

        if (deviceCount == 0)
        {
            _LOG_ERROR("Vulkan Error: Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        _VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance_, &deviceCount, physicalDevices.data()));

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
                _VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, (uint32_t)i, surface_, &presentSupport));
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
            _VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr));

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            _VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data()));

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
            _VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, nullptr));

            if (formatCount == 0)
            {
                continue;
            }

            surfaceFormats.resize(formatCount);
            _VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, surfaceFormats.data()));

            uint32_t presentModeCount;
            _VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, nullptr));

            if (presentModeCount == 0)
            {
                continue;
            }

            presentModes.resize(presentModeCount);
            _VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, presentModes.data()));

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

            _VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface_, &surfaceCapabilities_));

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

    void Vulkan_Renderer_T02::CreateLogicalDevice()
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

        _VK_CHECK(vkCreateDevice(physicalDevice_, &deviceCreateInfo, nullptr, &logicalDevice_));

        vkGetDeviceQueue(logicalDevice_, graphicsQueueFamilyIndex_, 0, &graphicsQueue_);
        vkGetDeviceQueue(logicalDevice_, presentQueueFamilyIndex_, 0, &presentQueue_);
    }

    void Vulkan_Renderer_T02::DeleteLogicalDevice()
    {
        vkDestroyDevice(logicalDevice_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateSwapChain()
    {
        swapchainImageCount_ = surfaceCapabilities_.minImageCount + 1;
        if (surfaceCapabilities_.maxImageCount > 0 &&
            swapchainImageCount_ > surfaceCapabilities_.maxImageCount)
        {
            swapchainImageCount_ = surfaceCapabilities_.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.surface = surface_;
        swapchainCreateInfo.minImageCount = swapchainImageCount_;
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

        _VK_CHECK(vkCreateSwapchainKHR(logicalDevice_, &swapchainCreateInfo, nullptr, &swapchain_));

        _VK_CHECK(vkGetSwapchainImagesKHR(logicalDevice_, swapchain_, &swapchainImageCount_, nullptr));
        swapchainImages_.resize(swapchainImageCount_);
        _VK_CHECK(vkGetSwapchainImagesKHR(logicalDevice_, swapchain_, &swapchainImageCount_, swapchainImages_.data()));

        swapchainImageViews_.resize(swapchainImageCount_);
        for (size_t i = 0; i < swapchainImages_.size(); i++)
        {
            swapchainImageViews_[i] = CreateImageView(swapchainImages_[i], surfaceFormat_.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    void Vulkan_Renderer_T02::DeleteSwapChain()
    {
        for (size_t i = 0; i < swapchainImageViews_.size(); ++i)
        {
            const VkImageView& imageView = swapchainImageViews_[i];
            vkDestroyImageView(logicalDevice_, imageView, nullptr);
        }
        swapchainImageViews_.clear();

        vkDestroySwapchainKHR(logicalDevice_, swapchain_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateDataBuffers()
    {
        VkDeviceSize vertexBufferSize = sizeof(VertexData) * s_vertexBufferCapacity_;
        VkDeviceSize indexBufferSize = sizeof(uint32_t) * s_indexBufferCapacity_;

        CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexStagingBuffer_, vertexStagingBufferMemory_);
        _VK_CHECK(vkMapMemory(logicalDevice_, vertexStagingBufferMemory_, 0, vertexBufferSize, 0, (void**)&vertexStagingBufferDataPtr_));

        CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexStagingBuffer_, indexStagingBufferMemory_);
        _VK_CHECK(vkMapMemory(logicalDevice_, indexStagingBufferMemory_, 0, indexBufferSize, 0, (void**)&indexStagingBufferDataPtr_));

        CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer_, vertexBufferMemory_);

        CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer_, indexBufferMemory_);

        CreateBuffer(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer_, uniformBufferMemory_);
    }

    void Vulkan_Renderer_T02::DeleteDataBuffers()
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

    void Vulkan_Renderer_T02::CreateDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        std::array<VkDescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = (uint32_t)bindings.size();
        layoutInfo.pBindings = bindings.data();

        _VK_CHECK(vkCreateDescriptorSetLayout(logicalDevice_, &layoutInfo, nullptr, &descriptorSetLayout_));
    }

    void Vulkan_Renderer_T02::DeleteDescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(logicalDevice_, descriptorSetLayout_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateDescriptorPool()
    {
        std::array<VkDescriptorPoolSize, 1> poolSizes = {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 1;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 1;

        _VK_CHECK(vkCreateDescriptorPool(logicalDevice_, &poolInfo, nullptr, &descriptorPool_));
    }

    void Vulkan_Renderer_T02::DeleteDescriptorPool()
    {
        vkDestroyDescriptorPool(logicalDevice_, descriptorPool_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateDescriptorSets()
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool_;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout_;

        _VK_CHECK(vkAllocateDescriptorSets(logicalDevice_, &allocInfo, &descriptorSet_));

        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffer_;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSet_;
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(logicalDevice_, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

    void Vulkan_Renderer_T02::CreateRenderPass()
    {
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = surfaceFormat_.format;
        colorAttachmentDescription.samples = samples;
        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentReference = {};
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentReference;

        VkSubpassDependency subpssDependency = {};
        subpssDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpssDependency.dstSubpass = 0;
        subpssDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpssDependency.srcAccessMask = 0;
        subpssDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpssDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpssDependency;

        _VK_CHECK(vkCreateRenderPass(logicalDevice_, &renderPassCreateInfo, nullptr, &renderPass_));

        swapchainFramebuffers_.resize(swapchainImageCount_);
        for (size_t i = 0; i < swapchainImageViews_.size(); ++i)
        {
            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = renderPass_;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &swapchainImageViews_[i];
            framebufferCreateInfo.width = surfaceExtent_.width;
            framebufferCreateInfo.height = surfaceExtent_.height;
            framebufferCreateInfo.layers = 1;

            _VK_CHECK(vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo, nullptr, &swapchainFramebuffers_[i]));
        }
    }

    void Vulkan_Renderer_T02::DeleteRenderPass()
    {
        vkDestroyRenderPass(logicalDevice_, renderPass_, nullptr);

        for (size_t i = 0; i < swapchainImageViews_.size(); ++i)
        {
            const VkImageView& imageView = swapchainImageViews_[i];
            vkDestroyImageView(logicalDevice_, imageView, nullptr);
        }
        swapchainImageViews_.clear();
    }

    void Vulkan_Renderer_T02::CreateGraphicsPipeline()
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
        dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;

        _VK_CHECK(vkCreatePipelineLayout(logicalDevice_, &pipelineLayoutInfo, nullptr, &pipelineLayout_));

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
        pipelineInfo.layout = pipelineLayout_;
        pipelineInfo.renderPass = renderPass_;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        _VK_CHECK(vkCreateGraphicsPipelines(logicalDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_));

        vkDestroyShaderModule(logicalDevice_, fragShaderModule, nullptr);
        vkDestroyShaderModule(logicalDevice_, vertShaderModule, nullptr);
    }

    void Vulkan_Renderer_T02::DeleteGraphicsPipeline()
    {
        vkDestroyPipeline(logicalDevice_, graphicsPipeline_, nullptr);
        vkDestroyPipelineLayout(logicalDevice_, pipelineLayout_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateCommandPool()
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        commandPoolCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex_;

        _VK_CHECK(vkCreateCommandPool(logicalDevice_, &commandPoolCreateInfo, nullptr, &commandPool_));
    }

    void Vulkan_Renderer_T02::DeleteCommandPool()
    {
        vkDestroyCommandPool(logicalDevice_, commandPool_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateCommandBuffers()
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        _VK_CHECK(vkAllocateCommandBuffers(logicalDevice_, &allocInfo, &commandBuffer_));
    }

    void Vulkan_Renderer_T02::CreateSyncObjects()
    {
        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

        _VK_CHECK(vkCreateFence(logicalDevice_, &fenceInfo, nullptr, &mainFence_));
    }

    void Vulkan_Renderer_T02::DeleteSyncObjects()
    {
        vkDestroyFence(logicalDevice_, mainFence_, nullptr);
    }






    VkImageView Vulkan_Renderer_T02::CreateImageView(
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
        _VK_CHECK(vkCreateImageView(logicalDevice_, &imageViewCreateInfo, nullptr, &imageView));

        return imageView;
    }

    void Vulkan_Renderer_T02::CreateImage(
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

        _VK_CHECK(vkCreateImage(logicalDevice_, &imageCreateInfo, nullptr, &image));

        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(logicalDevice_, image, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags);

        _VK_CHECK(vkAllocateMemory(logicalDevice_, &memoryAllocateInfo, nullptr, &imageMemory));
        _VK_CHECK(vkBindImageMemory(logicalDevice_, image, imageMemory, 0));
    }

    void Vulkan_Renderer_T02::CreateBuffer(
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

        _VK_CHECK(vkCreateBuffer(logicalDevice_, &bufferInfo, nullptr, &buffer));

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(logicalDevice_, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        _VK_CHECK(vkAllocateMemory(logicalDevice_, &allocInfo, nullptr, &bufferMemory));
        _VK_CHECK(vkBindBufferMemory(logicalDevice_, buffer, bufferMemory, 0));
    }

    uint32_t Vulkan_Renderer_T02::FindMemoryType(
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

    std::vector<char> Vulkan_Renderer_T02::ReadFile(const char* const filePath)
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

    VkShaderModule Vulkan_Renderer_T02::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        _VK_CHECK(vkCreateShaderModule(logicalDevice_, &createInfo, nullptr, &shaderModule));

        return shaderModule;
    }

#ifndef NDEBUG
    const bool Vulkan_Renderer_T02::s_enableValidationLayers_ = true;
    const std::vector<const char*> Vulkan_Renderer_T02::s_validationLayers_ =
    {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    const bool VulkanTutorial_Renderer::s_enableValidationLayers_ = false;
    const std::vector<const char*> VulkanTutorial_Renderer::s_validationLayers_ = {};
#endif

    const std::vector<const char*> Vulkan_Renderer_T02::s_deviceExtensions_ =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const bool Vulkan_Renderer_T02::s_requireSamplerAnisotropy_ = true;

    const unsigned int Vulkan_Renderer_T02::s_indexBufferCapacity_ = 4194304; // 8192;
    const unsigned int Vulkan_Renderer_T02::s_vertexBufferCapacity_ = 4194304; // 6144;
}