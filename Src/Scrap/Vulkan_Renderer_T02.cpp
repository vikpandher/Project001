#include "Vulkan_Renderer_T02.h"

#include "Engine/Platform/Vulkan_Error.h"
#include "Engine/MeshData.h"
#include "Engine/Window.h"

#include <array>
#include <fstream>
#include <set>

#define NUMBER_OF_TEXTURE_UNITS 16
#define NUMBER_OF_POINT_LIGHTS 8
#define NUMBER_OF_SPOT_LIGHTS 4



namespace Project001
{
    struct VertexShaderUniformBufferObject
    {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 projection;
    };

    struct FragmentShaderUniformBufferObject
    {
        alignas(16) glm::vec3 viewPosition;
        alignas(16) DirectionalLight directionalLight;
        alignas(16) PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
        alignas(16) SpotLight spotLights[NUMBER_OF_SPOT_LIGHTS];
    };

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
        bool multisampleAntiAliasing)
        : windowPtr_(windowPtr)
        , frameBufferWidth_(width)
        , frameBufferHeight_(height)
        , multisampleAntiAliasing_(multisampleAntiAliasing)
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
        , formatProperties_({})
        , depthFormat_(VK_FORMAT_UNDEFINED)
        , surfacePresentMode_(VK_PRESENT_MODE_FIFO_KHR)
        , physicalDeviceProperties_({})
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
        , vertexShaderUniformBuffer_(VK_NULL_HANDLE)
        , vertexShaderUniformBufferMemory_(VK_NULL_HANDLE)
        , fragmentShaderUniformBuffer_(VK_NULL_HANDLE)
        , fragmentShaderUniformBufferMemory_(VK_NULL_HANDLE)
        , defaultTexture_({})
        , descriptorSetLayout_(VK_NULL_HANDLE)
        , descriptorPool_(VK_NULL_HANDLE)
        , descriptorSet_(VK_NULL_HANDLE)
        , vertexBatchShaderModule_(VK_NULL_HANDLE)
        , fragmentBatchShaderModule_(VK_NULL_HANDLE)
        , pipelineLayout_(VK_NULL_HANDLE)
        , renderPass_(VK_NULL_HANDLE)
        , graphicsPipeline_(VK_NULL_HANDLE)
        , commandPool_(VK_NULL_HANDLE)
        , commandBuffer_(VK_NULL_HANDLE)
        , mainFence_(VK_NULL_HANDLE)
        , imageAvailableSemaphore_(VK_NULL_HANDLE)
        , renderFinishedSemaphore_(VK_NULL_HANDLE)
        , currentSwapchainFramebufferIndex_(0)
    {
        CreateVulkanInstance();

        CreateSurface();

        PickPhysicalDevice();

        CreateLogicalDevice();

        CreateCommandPool();

        CreateCommandBuffers();

        CreateSyncObjects();

        CreateSwapChain();

        CreateDataBuffers();

        CreateDefaultTexture();

        CreateDescriptorSetLayouts();

        CreateDescriptorPool();

        CreateDescriptorSets();

        CreatePipelineLayouts();

        CreateBatchShaderModules();

        CreateRenderPass();

        CreateGraphicsPipeline();

        DeleteBatchShaderModules();

        textureUnitStalenessValues_.resize(NUMBER_OF_TEXTURE_UNITS, 1);

        AcquireNextImage();
    }

    Vulkan_Renderer_T02::~Vulkan_Renderer_T02()
    {
        // this calls vkDeviceWaitIdle
        DeleteAllTextures();

        DeleteGraphicsPipeline();

        DeleteRenderPass();

        DeletePipelineLayouts();

        DeleteDescriptorPool();

        DeleteDescriptorSetLayouts();

        DeleteDefaultTexture();

        DeleteDataBuffers();

        DeleteSwapChain();

        DeleteSyncObjects();

        DeleteCommandPool();

        DeleteLogicalDevice();

        DeleteSurface();

        DeleteVulkanInstance();
    }

    bool Vulkan_Renderer_T02::CreateTexture(
        unsigned int& textureId,
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel,
        bool multisampleAntiAliasing,
        bool mipMaps)
    {
        unsigned int textureUnit = (unsigned int)textureMap_.size();
        if (textureUnit >= NUMBER_OF_TEXTURE_UNITS)
        {
            textureUnit = NUMBER_OF_TEXTURE_UNITS - 1;
        }

        if (recycledTextureIds_.empty())
        {
            textureId = (unsigned int)textureMap_.size();
        }
        else
        {
            textureId = recycledTextureIds_.front();
            recycledTextureIds_.pop_front();
        }

        CreateTexture(data, width, height, bytesPerPixel, multisampleAntiAliasing, mipMaps, textureMap_[textureId]);
        textureIdToUnitBiMap_.Add(textureId, textureUnit);

        return true;
    }

    bool Vulkan_Renderer_T02::DeleteTexture(unsigned int textureId)
    {
        vkDeviceWaitIdle(logicalDevice_);

        if (textureMap_.find(textureId) != textureMap_.end())
        {
            DeleteTexture(textureMap_[textureId]);
            textureIdToUnitBiMap_.Remove_Using_X(textureId);
            recycledTextureIds_.push_back(textureId);
            return true;
        }

        return false;
    }

    void Vulkan_Renderer_T02::DeleteAllTextures()
    {
        vkDeviceWaitIdle(logicalDevice_);

        for (std::map<unsigned int, Vulkan_Texture>::iterator iter = textureMap_.begin();
            iter != textureMap_.end(); ++iter)
        {
            DeleteTexture(iter->second);
        }
        textureMap_.clear();
        textureIdToUnitBiMap_.Clear();
        recycledTextureIds_.clear();
    }

    void Vulkan_Renderer_T02::AddPointLight(
        const glm::vec3& position,
        float constant,
        float linear,
        float quadratic,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        if (pointLights_.size() < NUMBER_OF_POINT_LIGHTS)
        {
            pointLights_.emplace_back(
                position,
                constant,
                linear,
                quadratic,
                ambient,
                diffuse,
                specular);
        }
    }

    void Vulkan_Renderer_T02::AddSpotLight(
        const glm::vec3& position,
        const glm::vec3& direction,
        float cutoff,
        float outerCutoff,
        float constant,
        float linear,
        float quadratic,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        if (spotLights_.size() < NUMBER_OF_SPOT_LIGHTS)
        {
            spotLights_.emplace_back(
                position,
                direction,
                cutoff,
                outerCutoff,
                constant,
                linear,
                quadratic,
                ambient,
                diffuse,
                specular);
        }
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
        if (textureId != (unsigned int)-1)
        {
            int getTextureUnitResult = GetTextureUnit(textureId, textureUnit);
            if (getTextureUnitResult == 1)
            {
                getTextureFailed = true;
            }
            else if (getTextureUnitResult == 2)
            {
                _LOG_ERROR("TextureId %u not found!", textureId);
                return false;
            }
        }

        float specularUnit = -1.0f;
        if (specularId != (unsigned int)-1)
        {
            int getTextureUnitResult = GetTextureUnit(specularId, specularUnit);
            if (getTextureUnitResult == 1)
            {
                getTextureFailed = true;
            }
            else if (getTextureUnitResult == 2)
            {
                _LOG_ERROR("SpecularId %u not found!", specularId);
                return false;
            }
        }

        if (getTextureFailed)
        {
            Render();
            GetTextureUnit(textureId, textureUnit);
            GetTextureUnit(specularId, specularUnit);
        }

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
            *(indexStagingBufferDataPtr_ + indexCount_++) = (uint32_t)(vertexBufferOffset + meshIndicies[j]);
        }

        return true;
    }

    void Vulkan_Renderer_T02::Render()
    {
        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        ApplyTextureBindings();
        IncreaseTectureUnitStaleness();

        VertexShaderUniformBufferObject vsubo = { viewMatrix_, projectionMatrix_ };
        void* data1;
        vkMapMemory(logicalDevice_, vertexShaderUniformBufferMemory_, 0, sizeof(vsubo), 0, &data1);
        memcpy(data1, &vsubo, sizeof(vsubo));
        vkUnmapMemory(logicalDevice_, vertexShaderUniformBufferMemory_);

        FragmentShaderUniformBufferObject fsubo;
        fsubo.viewPosition = viewPosition_;
        fsubo.directionalLight = directionalLight_;
        for (size_t i = 0; i < NUMBER_OF_POINT_LIGHTS && i < pointLights_.size(); ++i)
        {
            fsubo.pointLights[i] = pointLights_[i];
        }
        for (size_t i = 0; i < NUMBER_OF_SPOT_LIGHTS && i < spotLights_.size(); ++i)
        {
            fsubo.spotLights[i] = spotLights_[i];
        }

        void* data2;
        vkMapMemory(logicalDevice_, fragmentShaderUniformBufferMemory_, 0, sizeof(fsubo), 0, &data2);
        memcpy(data2, &fsubo, sizeof(fsubo));
        vkUnmapMemory(logicalDevice_, fragmentShaderUniformBufferMemory_);

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

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));

        // ---------------------------------------------------------------------

        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));

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

        vkCmdDrawIndexed(commandBuffer_, (uint32_t)indexCount_, 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer_);

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));

        vertexCount_ = 0;
        indexCount_ = 0;
    }

    void Vulkan_Renderer_T02::FinishRendering()
    {

    }

    void Vulkan_Renderer_T02::SwapBuffers()
    {
        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));

        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = swapchainImages_[currentSwapchainFramebufferIndex_];
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(
            commandBuffer_,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier
        );

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &renderFinishedSemaphore_;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphore_;

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

        AcquireNextImage();
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
                if (surfaceFormat.format == s_desiredSurfaceFormat &&
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
                vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &formatProperties_);

                if (imageTiling == VK_IMAGE_TILING_LINEAR &&
                    (formatProperties_.linearTilingFeatures & formatFeatureFlags) == formatFeatureFlags)
                {
                    depthFormat_ = format;
                    foundDepthFormat = true;
                    break;
                }
                else if (imageTiling == VK_IMAGE_TILING_OPTIMAL &&
                    (formatProperties_.optimalTilingFeatures & formatFeatureFlags) == formatFeatureFlags)
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

            vkGetPhysicalDeviceProperties(physicalDevice_, &physicalDeviceProperties_);

            VkSampleCountFlags counts = physicalDeviceProperties_.limits.framebufferColorSampleCounts & physicalDeviceProperties_.limits.framebufferDepthSampleCounts;
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
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        _VK_CHECK(vkCreateFence(logicalDevice_, &fenceInfo, nullptr, &mainFence_));

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        _VK_CHECK(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &imageAvailableSemaphore_));
        _VK_CHECK(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &renderFinishedSemaphore_));
    }

    void Vulkan_Renderer_T02::DeleteSyncObjects()
    {
        vkDestroyFence(logicalDevice_, mainFence_, nullptr);
        vkDestroySemaphore(logicalDevice_, imageAvailableSemaphore_, nullptr);
        vkDestroySemaphore(logicalDevice_, renderFinishedSemaphore_, nullptr);
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

        CreateBuffer(sizeof(VertexShaderUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexShaderUniformBuffer_, vertexShaderUniformBufferMemory_);

        CreateBuffer(sizeof(FragmentShaderUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, fragmentShaderUniformBuffer_, fragmentShaderUniformBufferMemory_);
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

        vkDestroyBuffer(logicalDevice_, vertexShaderUniformBuffer_, nullptr);
        vkFreeMemory(logicalDevice_, vertexShaderUniformBufferMemory_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateDefaultTexture()
    {
        unsigned char data[4] = { 255, 255, 255, 255 };
        CreateTexture(data, 1, 1, 4, false, false, defaultTexture_);
    }

    void Vulkan_Renderer_T02::DeleteDefaultTexture()
    {
        DeleteTexture(defaultTexture_);
    }

    void Vulkan_Renderer_T02::CreateDescriptorSetLayouts()
    {
        VkDescriptorSetLayoutBinding vsuboLayoutBinding = {};
        vsuboLayoutBinding.binding = 0;
        vsuboLayoutBinding.descriptorCount = 1;
        vsuboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        vsuboLayoutBinding.pImmutableSamplers = nullptr;
        vsuboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding fsuboLayoutBinding = {};
        fsuboLayoutBinding.binding = 1;
        fsuboLayoutBinding.descriptorCount = 1;
        fsuboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        fsuboLayoutBinding.pImmutableSamplers = nullptr;
        fsuboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 2;
        samplerLayoutBinding.descriptorCount = NUMBER_OF_TEXTURE_UNITS;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 3> bindings = { vsuboLayoutBinding, fsuboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = (uint32_t)bindings.size();
        layoutInfo.pBindings = bindings.data();

        _VK_CHECK(vkCreateDescriptorSetLayout(logicalDevice_, &layoutInfo, nullptr, &descriptorSetLayout_));
    }

    void Vulkan_Renderer_T02::DeleteDescriptorSetLayouts()
    {
        vkDestroyDescriptorSetLayout(logicalDevice_, descriptorSetLayout_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateDescriptorPool()
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes = {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 2;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = NUMBER_OF_TEXTURE_UNITS;

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

        std::array<VkWriteDescriptorSet, 2 + NUMBER_OF_TEXTURE_UNITS> descriptorWrites = {};

        VkDescriptorBufferInfo bufferInfo1 = {};
        bufferInfo1.buffer = vertexShaderUniformBuffer_;
        bufferInfo1.offset = 0;
        bufferInfo1.range = sizeof(VertexShaderUniformBufferObject);

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSet_;
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo1;

        VkDescriptorBufferInfo bufferInfo2 = {};
        bufferInfo2.buffer = fragmentShaderUniformBuffer_;
        bufferInfo2.offset = 0;
        bufferInfo2.range = sizeof(FragmentShaderUniformBufferObject);

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSet_;
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pBufferInfo = &bufferInfo2;

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = defaultTexture_.textureImageView_;
        imageInfo.sampler = defaultTexture_.textureSampler_;

        for (size_t i = 0; i < NUMBER_OF_TEXTURE_UNITS; i++)
        {
            descriptorWrites[2 + i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[2 + i].dstSet = descriptorSet_;
            descriptorWrites[2 + i].dstBinding = 2;
            descriptorWrites[2 + i].dstArrayElement = (uint32_t)i;
            descriptorWrites[2 + i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[2 + i].descriptorCount = 1;
            descriptorWrites[2 + i].pImageInfo = &imageInfo;
        }

        vkUpdateDescriptorSets(logicalDevice_, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

    void Vulkan_Renderer_T02::CreatePipelineLayouts()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;

        _VK_CHECK(vkCreatePipelineLayout(logicalDevice_, &pipelineLayoutInfo, nullptr, &pipelineLayout_));
    }

    void Vulkan_Renderer_T02::DeletePipelineLayouts()
    {
        vkDestroyPipelineLayout(logicalDevice_, pipelineLayout_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateBatchShaderModules()
    {
        std::vector<char> vertShaderCode = ReadFile("../Shaders/batchShader_vert.spv");
        std::vector<char> fragShaderCode = ReadFile("../Shaders/batchShader_frag.spv");

        vertexBatchShaderModule_ = CreateShaderModule(vertShaderCode);
        fragmentBatchShaderModule_ = CreateShaderModule(fragShaderCode);
    }

    void Vulkan_Renderer_T02::DeleteBatchShaderModules()
    {
        vkDestroyShaderModule(logicalDevice_, fragmentBatchShaderModule_, nullptr);
        vkDestroyShaderModule(logicalDevice_, vertexBatchShaderModule_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateRenderPass()
    {
        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = surfaceFormat_.format;
        colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
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
        for (size_t i = 0; i < swapchainFramebuffers_.size(); ++i)
        {
            const VkFramebuffer& framebuffer = swapchainFramebuffers_[i];
            vkDestroyFramebuffer(logicalDevice_, framebuffer, nullptr);
        }
        swapchainFramebuffers_.clear();

        vkDestroyRenderPass(logicalDevice_, renderPass_, nullptr);
    }

    void Vulkan_Renderer_T02::CreateGraphicsPipeline()
    {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertexBatchShaderModule_;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragmentBatchShaderModule_;
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
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

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
    }

    void Vulkan_Renderer_T02::DeleteGraphicsPipeline()
    {
        vkDestroyPipeline(logicalDevice_, graphicsPipeline_, nullptr);
    }






    void Vulkan_Renderer_T02::AcquireNextImage()
    {
        _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &mainFence_, VK_TRUE, UINT64_MAX));
        _VK_CHECK(vkResetFences(logicalDevice_, 1, &mainFence_));

        VkResult result = vkAcquireNextImageKHR(logicalDevice_, swapchain_, UINT64_MAX, imageAvailableSemaphore_, VK_NULL_HANDLE, &currentSwapchainFramebufferIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            // RecreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            _LOG_ERROR("Vulkan Error: Failed to acquire swap chain image!");
        }

        _VK_CHECK(vkResetCommandBuffer(commandBuffer_, 0));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer_, &beginInfo));

        VkMemoryBarrier memoryBarrier = {};

        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = swapchainImages_[currentSwapchainFramebufferIndex_];
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(
            commandBuffer_,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier
        );

        _VK_CHECK(vkEndCommandBuffer(commandBuffer_));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_;

        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &imageAvailableSemaphore_;
        submitInfo.pWaitDstStageMask = waitStages;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, mainFence_));
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

    void Vulkan_Renderer_T02::CreateTexture(
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel,
        bool multisampleAntiAliasing,
        bool mipMaps,
        Vulkan_Texture& texture)
    {
        // Creating Texture Image
        // ---------------------------------------------------------------------

        VkDeviceSize imageSize = width * height * bytesPerPixel;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(
            imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );

        if (data)
        {
            void* destinationData;
            _VK_CHECK(vkMapMemory(logicalDevice_, stagingBufferMemory, 0, imageSize, 0, &destinationData));
            memcpy(destinationData, data, (size_t)imageSize);
            vkUnmapMemory(logicalDevice_, stagingBufferMemory);
        }

        uint32_t mipLevels = 1;
        if (mipMaps)
        {
            uint32_t mipLevels = (uint32_t)std::floor(std::log2(std::max(width, height))) + 1;
        }

        CreateImage(
            width,
            height,
            mipLevels,
            VK_SAMPLE_COUNT_1_BIT,
            s_textureFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            texture.textureImage_,
            texture.textureImageMemory_
        );

        TransitionImageLayout(
            texture.textureImage_,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            mipLevels
        );

        CopyBufferToImage(
            stagingBuffer,
            texture.textureImage_,
            (uint32_t)width,
            (uint32_t)height
        );

        vkDestroyBuffer(logicalDevice_, stagingBuffer, nullptr);
        vkFreeMemory(logicalDevice_, stagingBufferMemory, nullptr);

        // transitions image layot to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        // after attempting to generating mipmaps
        GenerateMipmaps(
            texture.textureImage_,
            width,
            height,
            mipLevels
        );

        // Creating Texture Image View
        // ---------------------------------------------------------------------

        texture.textureImageView_ = CreateImageView(
            texture.textureImage_,
            s_textureFormat,
            VK_IMAGE_ASPECT_COLOR_BIT,
            mipLevels
        );

        // Creating Texture Sampler
        // ---------------------------------------------------------------------

        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

        if (multisampleAntiAliasing)
        {
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = physicalDeviceProperties_.limits.maxSamplerAnisotropy;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
        else
        {
            samplerInfo.magFilter = VK_FILTER_NEAREST;
            samplerInfo.minFilter = VK_FILTER_NEAREST;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        }

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.minLod = 0.0f; // (float)mipLevels_ / 2.0f;
        samplerInfo.maxLod = (float)mipLevels;
        samplerInfo.mipLodBias = 0.0f;

        _VK_CHECK(vkCreateSampler(logicalDevice_, &samplerInfo, nullptr, &texture.textureSampler_));
    }

    void Vulkan_Renderer_T02::DeleteTexture(Vulkan_Texture& texture)
    {
        vkDestroySampler(logicalDevice_, texture.textureSampler_, nullptr);
        vkDestroyImageView(logicalDevice_, texture.textureImageView_, nullptr);
        vkDestroyImage(logicalDevice_, texture.textureImage_, nullptr);
        vkFreeMemory(logicalDevice_, texture.textureImageMemory_, nullptr);
    }

    int Vulkan_Renderer_T02::GetTextureUnit(unsigned int textureId, float& textureUnit)
    {
        if (textureIdToUnitBiMap_.Find_X(textureId))
        {
            unsigned int textureUnit_uint = textureIdToUnitBiMap_.Get_Using_X(textureId);
            textureUnitStalenessValues_[textureUnit_uint] = 0;
            textureUnit = (float)textureUnit_uint;
        }
        else
        {
            unsigned int newTextureUnit = 1;
            if (GetStalestTextureUnit(newTextureUnit))
            {
                if (BindTexture(textureId, newTextureUnit))
                {
                    textureUnit = (float)newTextureUnit;
                    textureUnitStalenessValues_[newTextureUnit] = 0;
                }
                else
                {
                    // texture doesn't exist
                    return 2;
                }
            }
            else
            {
                // no more room for this texture
                return 1;
            }
        }

        return 0;
    }

    bool Vulkan_Renderer_T02::GetStalestTextureUnit(unsigned int& textureUnit) const
    {
        unsigned int stalestValue = 0;

        for (unsigned int i = 0; i < NUMBER_OF_TEXTURE_UNITS; ++i)
        {
            if (textureUnitStalenessValues_[i] > stalestValue)
            {
                textureUnit = i;
                stalestValue = textureUnitStalenessValues_[i];
            }
        }

        return stalestValue > 0;
    }

    void Vulkan_Renderer_T02::IncreaseTectureUnitStaleness()
    {
        for (size_t i = 0; i < NUMBER_OF_TEXTURE_UNITS; ++i)
        {
            textureUnitStalenessValues_[i]++;
        }
    }

    bool Vulkan_Renderer_T02::BindTexture(
        unsigned int textureId,
        unsigned int textureUnit)
    {
        if (textureUnit < NUMBER_OF_TEXTURE_UNITS &&
            textureMap_.find(textureId) != textureMap_.end())
        {
            textureIdToUnitBiMap_.Add(textureId, textureUnit);
            return true;
        }
        else
        {
            return false;
        }
    }

    void Vulkan_Renderer_T02::ApplyTextureBindings()
    {
        std::array<VkDescriptorImageInfo, NUMBER_OF_TEXTURE_UNITS> imageInfos = {};
        std::array<VkWriteDescriptorSet, NUMBER_OF_TEXTURE_UNITS> descriptorWrites = {};

        for (unsigned int i = 0; i < NUMBER_OF_TEXTURE_UNITS; i++)
        {
            Vulkan_Texture* currentTexturePtr;
            if (textureIdToUnitBiMap_.Find_Y(i))
            {
                const unsigned int& textureId = textureIdToUnitBiMap_.Get_Using_Y(i);
                currentTexturePtr = &textureMap_[textureId];
            }
            else
            {
                currentTexturePtr = &defaultTexture_;
            }

            imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[i].imageView = currentTexturePtr->textureImageView_;
            imageInfos[i].sampler = currentTexturePtr->textureSampler_;

            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = descriptorSet_;
            descriptorWrites[i].dstBinding = 2;
            descriptorWrites[i].dstArrayElement = (uint32_t)i;
            descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].pImageInfo = &imageInfos[i];
        }

        vkUpdateDescriptorSets(logicalDevice_, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

    void Vulkan_Renderer_T02::TransitionImageLayout(
        VkImage image,
        VkImageLayout oldLayout,
        VkImageLayout newLayout,
        uint32_t mipLevels)
    {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.oldLayout = oldLayout;
        imageMemoryBarrier.newLayout = newLayout;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

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
            1, &imageMemoryBarrier
        );

        EndSingleTimeCommands(commandBuffer);
    }

    void Vulkan_Renderer_T02::CopyBufferToImage(
        VkBuffer buffer,
        VkImage image,
        uint32_t width,
        uint32_t height)
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

    void Vulkan_Renderer_T02::GenerateMipmaps(
        VkImage image,
        int32_t texWidth,
        int32_t texHeight,
        uint32_t mipLevels)
    {
        if (!(formatProperties_.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            _LOG_ERROR("Vulkan Error: Texture image format does not support linear blitting!");
        }

        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;
        imageMemoryBarrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;

        for (uint32_t i = 1; i < mipLevels; i++)
        {
            imageMemoryBarrier.subresourceRange.baseMipLevel = i - 1;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier);

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

            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        imageMemoryBarrier.subresourceRange.baseMipLevel = mipLevels - 1;
        imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier);

        EndSingleTimeCommands(commandBuffer);
    }

    VkCommandBuffer Vulkan_Renderer_T02::BeginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool_;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        _VK_CHECK(vkAllocateCommandBuffers(logicalDevice_, &allocInfo, &commandBuffer));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        return commandBuffer;
    }

    void Vulkan_Renderer_T02::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        _VK_CHECK(vkEndCommandBuffer(commandBuffer));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE));
        _VK_CHECK(vkQueueWaitIdle(graphicsQueue_));

        vkFreeCommandBuffers(logicalDevice_, commandPool_, 1, &commandBuffer);
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

    VkRenderPass Vulkan_Renderer_T02::CreateRenderPass(
        bool depthTesting,
        bool msaa)
    {
        VkAttachmentDescription colorAttachmentDescription = {};
        colorAttachmentDescription.format = s_textureFormat;
        colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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

        VkRenderPass renderPass;
        _VK_CHECK(vkCreateRenderPass(logicalDevice_, &renderPassCreateInfo, nullptr, &renderPass));
        return renderPass;
    }

    VkPipeline Vulkan_Renderer_T02::CreateGraphicsPipeline(
        VkRenderPass& renderPass,
        bool depthTesting,
        bool msaa)
    {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertexBatchShaderModule_;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragmentBatchShaderModule_;
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
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

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
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkPipeline graphicsPipeline;
        _VK_CHECK(vkCreateGraphicsPipelines(logicalDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));
        return graphicsPipeline;
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

    const unsigned int Vulkan_Renderer_T02::s_indexBufferCapacity_ = 4096; // 4194304; // 8192;
    const unsigned int Vulkan_Renderer_T02::s_vertexBufferCapacity_ = 4096; // 4194304; // 6144;

    // const VkFormat Vulkan_Renderer_T02::s_desiredSurfaceFormat = VK_FORMAT_B8G8R8A8_SRGB;
    // const VkFormat Vulkan_Renderer_T02::s_textureFormat = VK_FORMAT_R8G8B8A8_SRGB;

    const VkFormat Vulkan_Renderer_T02::s_desiredSurfaceFormat = VK_FORMAT_B8G8R8A8_UNORM;
    const VkFormat Vulkan_Renderer_T02::s_textureFormat = VK_FORMAT_R8G8B8A8_UNORM;
}