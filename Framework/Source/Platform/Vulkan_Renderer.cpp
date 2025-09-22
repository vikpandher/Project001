// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-09-21

#include "Vulkan_Renderer.h"

#include "Platform/Vulkan_Error.h"

#include "Platform/Vulkan_Shaders/Generated/v_primary_frag_spv.h"
#include "Platform/Vulkan_Shaders/Generated/v_primary_vert_spv.h"
#include "Platform/Vulkan_Shaders/Generated/v_screen_frag_spv.h"
#include "Platform/Vulkan_Shaders/Generated/v_screen_vert_spv.h"

#include "Logger.h"
#include "Window.h"

#include <array>
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
            LOG_ERROR_F("Vulkan Error: " << pCallbackData->pMessage);
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            LOG_WARNING_F("Vulkan Warning: " << pCallbackData->pMessage);
        }
        return VK_FALSE;
    }

    // public ------------------------------------------------------------------

    Vulkan_Renderer::Vulkan_Renderer(const RendererInfo& rendererInfo)
        : windowPtr_(rendererInfo.windowPtr)
        , frameBufferWidth_(rendererInfo.frameBufferWidth)
        , frameBufferHeight_(rendererInfo.frameBufferHeight)
        , instanceBufferCapacity_(rendererInfo.instanceBufferCapacity)
        , batchedIndexBufferCapacity_(rendererInfo.batchedIndexBufferCapacity)
        , batchedVertexBufferCapacity_(rendererInfo.batchedVertexBufferCapacity)
        , multisampleAntiAliasing_(rendererInfo.multisampleAntiAliasing)
        , depthTesting_(rendererInfo.depthTesting)
        , viewportX_(0)
        , viewportY_(0)
        , viewportWidth_(rendererInfo.frameBufferWidth)
        , viewportHeight_(rendererInfo.frameBufferHeight)
        , cameraViewportX_(0.0f)
        , cameraViewportY_(0.0f)
        , cameraViewportWidth_(1.0f)
        , cameraViewportHeight_(1.0f)
        , borderColor_(0.1f, 0.1f, 0.1f, 1.0f)
        , clearColor_(0.0f, 0.0f, 0.0f, 1.0f)
        , viewMatrix_(1.0f)
        , viewPosition_(0.0f, 0.0f, 0.0f)
        , projectionMatrix_(1.0f)
        , framebufferResized_(false)
        , pendingFrameBufferWidth_(rendererInfo.frameBufferWidth)
        , pendingFrameBufferHeight_(rendererInfo.frameBufferHeight)
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
        , commandPool_(VK_NULL_HANDLE)
        , renderingCommandBuffer_(VK_NULL_HANDLE)
        , nextCommandBufferIndex_(0)
        , renderingFence_(VK_NULL_HANDLE)
        , justRenderedToBatch_(false)
        , batchedDataTransferFence_(VK_NULL_HANDLE)
        , justRenderedToTexture_(false)
        , instanceDataTransferFence_(VK_NULL_HANDLE)
        , imageAvailableSemaphore_(VK_NULL_HANDLE)
        , readyToPresentSemaphore_(VK_NULL_HANDLE)
        , currentSemaphoreIndex_(0)
        , swapchain_(VK_NULL_HANDLE)
        , swapchainImageCount_(0)
        , instanceStagingBuffer_(VK_NULL_HANDLE)
        , instanceStagingBufferMemory_(VK_NULL_HANDLE)
        , instanceStagingBufferDataPtr_(NULL)
        , instanceCount_(0)
        , instanceBuffer_(VK_NULL_HANDLE)
        , instanceBufferMemory_(VK_NULL_HANDLE)
        , batchedIndexStagingBuffer_(VK_NULL_HANDLE)
        , batchedIndexStagingBufferMemory_(VK_NULL_HANDLE)
        , batchedIndexStagingBufferDataPtr_(NULL)
        , batchedIndexCount_(0)
        , batchedIndexBuffer_(VK_NULL_HANDLE)
        , batchedIndexBufferMemory_(VK_NULL_HANDLE)
        , batchedVertexStagingBuffer_(VK_NULL_HANDLE)
        , batchedVertexStagingBufferMemory_(VK_NULL_HANDLE)
        , batchedVertexStagingBufferDataPtr_(NULL)
        , batchedVertexCount_(0)
        , batchedVertexBuffer_(VK_NULL_HANDLE)
        , batchedVertexBufferMemory_(VK_NULL_HANDLE)
        , vertexShaderUniformBuffer_(VK_NULL_HANDLE)
        , vertexShaderUniformBufferMemory_(VK_NULL_HANDLE)
        , fragmentShaderUniformBuffer_(VK_NULL_HANDLE)
        , fragmentShaderUniformBufferMemory_(VK_NULL_HANDLE)
        , screenVertexBuffer_(VK_NULL_HANDLE)
        , screenVertexBufferMemory_(VK_NULL_HANDLE)
        , defaultTexture_({})
        , primaryDescriptorSetLayout_(VK_NULL_HANDLE)
        , secondaryDescriptorSetLayout_(VK_NULL_HANDLE)
        , descriptorPool_(VK_NULL_HANDLE)
        , primaryDescriptorSet_(VK_NULL_HANDLE)
        , secondaryDescriptorSet_(VK_NULL_HANDLE)
        , primaryPipelineLayout_(VK_NULL_HANDLE)
        , secondaryPipelineLayout_(VK_NULL_HANDLE)
        , vertexBatchShaderModule_(VK_NULL_HANDLE)
        , fragmentBatchShaderModule_(VK_NULL_HANDLE)
        , primaryRenderPass1_(VK_NULL_HANDLE)
        , primaryRenderPass2_(VK_NULL_HANDLE)
        , primaryRenderPass3_(VK_NULL_HANDLE)
        , primaryRenderPass4_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline1_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline2_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline3_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline4_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline5_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline6_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline7_(VK_NULL_HANDLE)
        , primaryGraphicsPipeline8_(VK_NULL_HANDLE)
        , vertexScreenShaderModule_(VK_NULL_HANDLE)
        , screenTexture_({})
        , depthImage_(VK_NULL_HANDLE)
        , depthImageMemory_(VK_NULL_HANDLE)
        , depthImageView_(VK_NULL_HANDLE)
        , msaaImage_(VK_NULL_HANDLE)
        , msaaImageMemory_(VK_NULL_HANDLE)
        , msaaImageView_(VK_NULL_HANDLE)
        , msaaDepthImage_(VK_NULL_HANDLE)
        , msaaDepthImageMemory_(VK_NULL_HANDLE)
        , msaaDepthImageView_(VK_NULL_HANDLE)
        , primaryFrameBuffer1_(VK_NULL_HANDLE)
        , primaryFrameBuffer2_(VK_NULL_HANDLE)
        , primaryFrameBuffer3_(VK_NULL_HANDLE)
        , primaryFrameBuffer4_(VK_NULL_HANDLE)
        , fragmentScreenShaderModule_(VK_NULL_HANDLE)
        , secondaryRenderPass_(VK_NULL_HANDLE)
        , secondaryGraphicsPipeline_(VK_NULL_HANDLE)
        , currentSwapchainFramebufferIndex_(0)
    {
        CreateVulkanInstance();

        CreateSurface();

        PickPhysicalDevice();

        CreateLogicalDevice();

        CreateCommandPool();

        CreateCommandBuffers();

        CreateSyncObjects();

        CreateSwapchain();

        CreateInstanceDataBuffers();

        CreateBatchedIndexDataBuffers();

        CreateBatchedVertexDataBuffers();

        CreateOtherDataBuffers();

        CreateDefaultTexture();

        CreateDescriptorSetLayouts();

        CreateDescriptorPool();

        CreateDescriptorSets();

        CreatePipelineLayouts();

        CreateBatchShaderModules();

        CreatePrimaryRenderPasses();

        CreatePrimaryGraphicsPipelines();

        CreatePrimaryFrameBufferAttachments();

        CreatePrimaryFrameBuffers();

        DeleteBatchShaderModules();

        CreateScreenShaderModules();

        CreateSecondaryRenderPass();

        CreateSecondaryGraphicsPipeline();

        CreateSwapchainFramebuffers();

        DeleteScreenShaderModules();

        textureUnitStalenessValues_.resize(NUMBER_OF_TEXTURE_UNITS, 1);

        AcquireNextImage();
    }

    Vulkan_Renderer::~Vulkan_Renderer()
    {
        DeleteAllTextures(); // this calls vkDeviceWaitIdle

        DeleteAllMeshes(); // this calls vkDeviceWaitIdle

        DeleteSwapchainFramebuffers();

        DeleteSecondaryGraphicsPipeline();

        DeleteSecondaryRenderPass();

        DeletePrimaryFrameBuffers();

        DeletePrimaryFrameBufferAttachments();

        DeletePrimaryGraphicsPipelines();

        DeletePrimaryRenderPasses();

        DeletePipelineLayouts();

        DeleteDescriptorPool();

        DeleteDescriptorSetLayouts();

        DeleteDefaultTexture();

        DeleteOtherDataBuffers();

        DeleteBatchedVertexDataBuffers();

        DeleteBatchedIndexDataBuffers();

        DeleteInstanceDataBuffers();

        DeleteSwapchain();

        DeleteSyncObjects();

        DeleteCommandPool();

        DeleteLogicalDevice();

        UnPickPhysicalDevice();

        DeleteSurface();

        DeleteVulkanInstance();
    }

    void Vulkan_Renderer::SetInstanceBufferCapacity(unsigned int capacity)
    {
        vkDeviceWaitIdle(logicalDevice_);
        instanceBufferCapacity_ = capacity;
        instanceCount_ = 0;
        DeleteInstanceDataBuffers();
        CreateInstanceDataBuffers();
    }

    void Vulkan_Renderer::SetBatchedIndexBufferCapacity(unsigned int capacity)
    {
        vkDeviceWaitIdle(logicalDevice_);
        batchedIndexBufferCapacity_ = capacity;
        batchedIndexCount_ = 0;
        DeleteBatchedIndexDataBuffers();
        CreateBatchedIndexDataBuffers();
    }

    void Vulkan_Renderer::SetBatchedVertexBufferCapacity(unsigned int capacity)
    {
        vkDeviceWaitIdle(logicalDevice_);
        batchedVertexBufferCapacity_ = capacity;
        batchedVertexCount_ = 0;
        DeleteBatchedVertexDataBuffers();
        CreateBatchedVertexDataBuffers();
    }

    void Vulkan_Renderer::CreateTexture(
        unsigned int& textureId,
        unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int bytesPerPixel,
        bool multisampleAntiAliasing,
        bool mipMaps)
    {
        unsigned int textureUnit = (unsigned int)textureMap_.Size();
        if (textureUnit >= NUMBER_OF_TEXTURE_UNITS)
        {
            textureUnit = NUMBER_OF_TEXTURE_UNITS - 1;
        }

        Vulkan_Texture newTexture;
        CreateTexture(data, width, height, bytesPerPixel, multisampleAntiAliasing, mipMaps, newTexture);
        textureMap_.Add(textureId, newTexture);
        textureIdToUnitBiMap_.Add(textureId, textureUnit);
    }

    bool Vulkan_Renderer::DeleteTexture(unsigned int textureId)
    {
        vkDeviceWaitIdle(logicalDevice_);

        AutoIdMap<Vulkan_Texture>::iterator iter = textureMap_.Find(textureId);
        if (iter != textureMap_.IteratorPastTheEnd())
        {
            DeleteTexture(iter->second);
            textureMap_.Erase(iter);
            textureIdToUnitBiMap_.EraseKey(textureId);
            return true;
        }

        return false;
    }

    void Vulkan_Renderer::DeleteAllTextures()
    {
        vkDeviceWaitIdle(logicalDevice_);

        for (AutoIdMap<Vulkan_Texture>::iterator iter = textureMap_.IteratorAtBeginning();
            iter != textureMap_.IteratorPastTheEnd(); ++iter)
        {
            DeleteTexture(iter->second);
        }

        textureMap_.Clear();
        textureIdToUnitBiMap_.Clear();
    }

    void Vulkan_Renderer::AddPointLight(
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

    void Vulkan_Renderer::AddSpotLight(
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

    void Vulkan_Renderer::BeginRendering()
    {

    }

    void Vulkan_Renderer::Clear()
    {
        if (framebufferResized_)
        {
            HandleFramebufferResize();
            framebufferResized_ = false;
        }

        if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0 ||
            surfaceExtent_.width > 0 && surfaceExtent_.height > 0)
        {
            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

            VkMemoryBarrier renderingMemoryBarrier = {};
            renderingMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            renderingMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            renderingMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                1, &renderingMemoryBarrier,
                0, nullptr,
                0, nullptr
            );

            if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0)
            {
                VkRenderPass currentRenderPass;
                VkFramebuffer currentFrameBuffer;
                if (multisampleAntiAliasing_)
                {
                    currentRenderPass = primaryRenderPass4_;
                    currentFrameBuffer = primaryFrameBuffer4_;
                }
                else
                {
                    currentRenderPass = primaryRenderPass2_;
                    currentFrameBuffer = primaryFrameBuffer2_;
                }

                VkRenderPassBeginInfo primaryRenderPassInfo = {};
                primaryRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                primaryRenderPassInfo.renderPass = currentRenderPass;
                primaryRenderPassInfo.framebuffer = currentFrameBuffer;
                primaryRenderPassInfo.renderArea.offset = { 0, 0 };
                primaryRenderPassInfo.renderArea.extent = { frameBufferWidth_, frameBufferHeight_ };;

                vkCmdBeginRenderPass(commandBuffer, &primaryRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                std::array<VkClearAttachment, 2> primaryClearAttachments = {};

                VkClearAttachment& imageClearAttachment = primaryClearAttachments[0];
                imageClearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageClearAttachment.clearValue.color = { clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a };
                imageClearAttachment.colorAttachment = 0;

                VkClearAttachment& depthClearAttachment = primaryClearAttachments[1];
                depthClearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                depthClearAttachment.clearValue.depthStencil = { 1.0f, 0 };
                depthClearAttachment.colorAttachment = VK_ATTACHMENT_UNUSED;

                VkClearRect primaryClearRect = {};
                primaryClearRect.layerCount = 1;
                primaryClearRect.rect.offset = { 0, 0 };
                primaryClearRect.rect.extent = { frameBufferWidth_, frameBufferHeight_ };

                vkCmdClearAttachments(commandBuffer, (uint32_t)primaryClearAttachments.size(), primaryClearAttachments.data(), 1, &primaryClearRect);

                vkCmdEndRenderPass(commandBuffer);
            }

            if (surfaceExtent_.width > 0 && surfaceExtent_.height > 0)
            {
                VkRenderPassBeginInfo secondaryRenderPassInfo = {};
                secondaryRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                secondaryRenderPassInfo.renderPass = secondaryRenderPass_;
                secondaryRenderPassInfo.framebuffer = swapchainFramebuffers_[currentSwapchainFramebufferIndex_];
                secondaryRenderPassInfo.renderArea.offset = { 0, 0 };
                secondaryRenderPassInfo.renderArea.extent = surfaceExtent_;

                vkCmdBeginRenderPass(commandBuffer, &secondaryRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                VkClearAttachment secondaryColorClearAttachment = {};
                secondaryColorClearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                secondaryColorClearAttachment.clearValue.color = { borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a };
                secondaryColorClearAttachment.colorAttachment = 0;

                VkClearRect secondaryClearRect = {};
                secondaryClearRect.layerCount = 1;
                secondaryClearRect.rect.offset = { 0, 0 };
                secondaryClearRect.rect.extent = { surfaceExtent_.width, surfaceExtent_.height };

                vkCmdClearAttachments(commandBuffer, 1, &secondaryColorClearAttachment, 1, &secondaryClearRect);

                vkCmdEndRenderPass(commandBuffer);
            }

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE));
        }
    }

    void Vulkan_Renderer::ClearDepthOnly()
    {
        if (framebufferResized_)
        {
            HandleFramebufferResize();
            framebufferResized_ = false;
        }

        if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0 ||
            surfaceExtent_.width > 0 && surfaceExtent_.height > 0)
        {
            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

            VkMemoryBarrier renderingMemoryBarrier = {};
            renderingMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            renderingMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            renderingMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                1, &renderingMemoryBarrier,
                0, nullptr,
                0, nullptr
            );

            if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0)
            {
                VkRenderPass currentRenderPass;
                VkFramebuffer currentFrameBuffer;
                if (multisampleAntiAliasing_)
                {
                    currentRenderPass = primaryRenderPass4_;
                    currentFrameBuffer = primaryFrameBuffer4_;
                }
                else
                {
                    currentRenderPass = primaryRenderPass2_;
                    currentFrameBuffer = primaryFrameBuffer2_;
                }

                VkRenderPassBeginInfo primaryRenderPassInfo = {};
                primaryRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                primaryRenderPassInfo.renderPass = currentRenderPass;
                primaryRenderPassInfo.framebuffer = currentFrameBuffer;
                primaryRenderPassInfo.renderArea.offset = { 0, 0 };
                primaryRenderPassInfo.renderArea.extent = { frameBufferWidth_, frameBufferHeight_ };;

                vkCmdBeginRenderPass(commandBuffer, &primaryRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                std::array<VkClearAttachment, 1> primaryClearAttachments = {};

                VkClearAttachment& depthClearAttachment = primaryClearAttachments[0];
                depthClearAttachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                depthClearAttachment.clearValue.depthStencil = { 1.0f, 0 };
                depthClearAttachment.colorAttachment = VK_ATTACHMENT_UNUSED;

                VkClearRect primaryClearRect = {};
                primaryClearRect.layerCount = 1;
                primaryClearRect.rect.offset = { 0, 0 };
                primaryClearRect.rect.extent = { frameBufferWidth_, frameBufferHeight_ };

                vkCmdClearAttachments(commandBuffer, (uint32_t)primaryClearAttachments.size(), primaryClearAttachments.data(), 1, &primaryClearRect);

                vkCmdEndRenderPass(commandBuffer);
            }

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE));
        }
    }

    void Vulkan_Renderer::CreateMesh(
        unsigned int& meshId,
        const MeshVertex* meshVertexPtr,
        unsigned int meshVertexCount,
        const unsigned int* meshIndexPtr,
        unsigned int meshIndexCount)
    {
        Vulkan_Mesh newMesh;
        CreateMesh(meshVertexPtr, meshVertexCount, meshIndexPtr, meshIndexCount, newMesh);
        meshMap_.Add(meshId, newMesh);
    }

    bool Vulkan_Renderer::DeleteMesh(unsigned int meshId)
    {
        vkDeviceWaitIdle(logicalDevice_);

        AutoIdMap<Vulkan_Mesh>::iterator iter = meshMap_.Find(meshId);
        if (iter != meshMap_.IteratorPastTheEnd())
        {
            DeleteMesh(iter->second);
            meshMap_.Erase(iter);
            return true;
        }

        return false;
    }

    void Vulkan_Renderer::DeleteAllMeshes()
    {
        vkDeviceWaitIdle(logicalDevice_);

        for (AutoIdMap<Vulkan_Mesh>::iterator iter = meshMap_.IteratorAtBeginning();
            iter != meshMap_.IteratorPastTheEnd(); ++iter)
        {
            DeleteMesh(iter->second);
        }

        meshMap_.Clear();
    }

    bool Vulkan_Renderer::RenderMesh(
        unsigned int meshId,
        const MeshInstanceData* meshInstanceDataPtr,
        unsigned int meshInstanceCount)
    {
        if (meshInstanceCount == 0)
        {
            // Want to draw nothing, so draw nothing
            return true;
        }

        AutoIdMap<Vulkan_Mesh>::iterator iter = meshMap_.Find(meshId);
        if (iter == meshMap_.IteratorPastTheEnd())
        {
            LOG_ERROR_F("Mesh Id not found!");
            return false;
        }

        Vulkan_Mesh& mesh = iter->second;

        if(batchedVertexCount_ > 0 && batchedIndexCount_ > 0)
        {
            RenderBatch();
        }

        for (unsigned int i = 0; i < meshInstanceCount; ++i)
        {
            if (instanceCount_ == instanceBufferCapacity_)
            {
                RenderMeshToTexture(mesh);
            }

            const MeshInstanceData& currentMeshInstanceData = meshInstanceDataPtr[i];

            unsigned int textureId = currentMeshInstanceData.textureId;
            unsigned int specularId = currentMeshInstanceData.specularId;

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
                    LOG_ERROR_F("TextureId " << textureId << " not found!");
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
                    LOG_ERROR_F("SpecularId " << specularId << " not found!");
                    return false;
                }
            }

            if (getTextureFailed)
            {
                RenderMeshToTexture(mesh);
                GetTextureUnit(textureId, textureUnit);
                GetTextureUnit(specularId, specularUnit);
            }

            if (justRenderedToTexture_)
            {
                justRenderedToTexture_ = false;
                _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &instanceDataTransferFence_, VK_TRUE, UINT64_MAX));
            }

            InstanceData newInstance;
            newInstance.color = currentMeshInstanceData.color;
            newInstance.textureUnit = textureUnit;
            newInstance.specularUnit = specularUnit;
            newInstance.shininess = currentMeshInstanceData.shininess;
            newInstance.scale = currentMeshInstanceData.scale;
            newInstance.translation = currentMeshInstanceData.position;
            newInstance.orientation.x = currentMeshInstanceData.orientation.x;
            newInstance.orientation.y = currentMeshInstanceData.orientation.y;
            newInstance.orientation.z = currentMeshInstanceData.orientation.z;
            newInstance.orientation.w = currentMeshInstanceData.orientation.w;
            newInstance.lit = currentMeshInstanceData.lit;

            *(instanceStagingBufferDataPtr_ + instanceCount_++) = newInstance;
        }

        RenderMeshToTexture(mesh);

        return true;
    }

    bool Vulkan_Renderer::AddMeshToBatch(
        const MeshVertex* meshVertexPtr,
        unsigned int meshVertexCount,
        const unsigned int* meshIndexPtr,
        unsigned int meshIndexCount,
        unsigned int textureId,
        unsigned int specularId,
        const glm::vec3& position,
        const glm::quat& orientation,
        const glm::vec3& scale,
        const glm::vec4& color,
        float shininess,
        bool lit)
    {
        if ((batchedVertexCount_ + meshVertexCount) > batchedVertexBufferCapacity_ ||
            (batchedIndexCount_ + meshIndexCount) > batchedIndexBufferCapacity_)
        {
            if (meshVertexCount > batchedVertexBufferCapacity_ ||
                meshIndexCount > batchedIndexBufferCapacity_)
            {
                LOG_ERROR_F("Mesh larger then buffer!");
                return false;
            }

            RenderBatch();
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
                LOG_ERROR_F("TextureId " << textureId << " not found!");
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
                LOG_ERROR_F("SpecularId " << specularId << " not found!");
                return false;
            }
        }

        if (getTextureFailed)
        {
            RenderBatch();
            GetTextureUnit(textureId, textureUnit);
            GetTextureUnit(specularId, specularUnit);
        }

        size_t batchedVertexBufferOffset = batchedVertexCount_;

        if (justRenderedToBatch_)
        {
            justRenderedToBatch_ = false;
            _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &batchedDataTransferFence_, VK_TRUE, UINT64_MAX));
        }

        for (size_t j = 0; j < meshVertexCount; ++j)
        {
            const MeshVertex& currentMeshVertex = meshVertexPtr[j];

            BatchedVertexData newVertex;
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

            *(batchedVertexStagingBufferDataPtr_ + batchedVertexCount_++) = newVertex;
        }

        for (unsigned int j = 0; j < meshIndexCount; ++j)
        {
            uint32_t newIndex = (uint32_t)(batchedVertexBufferOffset + meshIndexPtr[j]);

            *(batchedIndexStagingBufferDataPtr_ + batchedIndexCount_++) = newIndex;
        }

        return true;
    }

    void Vulkan_Renderer::RenderBatch()
    {
        if (framebufferResized_)
        {
            HandleFramebufferResize();
            framebufferResized_ = false;
        }

        if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0 &&
            batchedVertexCount_ > 0 && batchedIndexCount_ > 0)
        {
            _VK_CHECK(vkResetFences(logicalDevice_, 1, &batchedDataTransferFence_));

            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

            VkBufferMemoryBarrier batchedVertexBufferUpdateBarrier = {};
            batchedVertexBufferUpdateBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            batchedVertexBufferUpdateBarrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            batchedVertexBufferUpdateBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            batchedVertexBufferUpdateBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedVertexBufferUpdateBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedVertexBufferUpdateBarrier.buffer = batchedVertexBuffer_;
            batchedVertexBufferUpdateBarrier.offset = 0;
            batchedVertexBufferUpdateBarrier.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr,
                1, &batchedVertexBufferUpdateBarrier,
                0, nullptr
            );

            VkBufferCopy vertexCopyRegion = {};
            vertexCopyRegion.size = sizeof(BatchedVertexData) * batchedVertexCount_;
            vkCmdCopyBuffer(commandBuffer, batchedVertexStagingBuffer_, batchedVertexBuffer_, 1, &vertexCopyRegion);

            VkBufferMemoryBarrier batchedVertexBufferUpdateBarrier2 = {};
            batchedVertexBufferUpdateBarrier2.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            batchedVertexBufferUpdateBarrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            batchedVertexBufferUpdateBarrier2.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            batchedVertexBufferUpdateBarrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedVertexBufferUpdateBarrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedVertexBufferUpdateBarrier2.buffer = batchedVertexBuffer_;
            batchedVertexBufferUpdateBarrier2.offset = 0;
            batchedVertexBufferUpdateBarrier2.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                0,
                0, nullptr,
                1, &batchedVertexBufferUpdateBarrier2,
                0, nullptr
            );

            VkBufferMemoryBarrier batchedIndexBufferUpdateBarrier = {};
            batchedIndexBufferUpdateBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            batchedIndexBufferUpdateBarrier.srcAccessMask = VK_ACCESS_INDEX_READ_BIT;
            batchedIndexBufferUpdateBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            batchedIndexBufferUpdateBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedIndexBufferUpdateBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedIndexBufferUpdateBarrier.buffer = batchedIndexBuffer_;
            batchedIndexBufferUpdateBarrier.offset = 0;
            batchedIndexBufferUpdateBarrier.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr,
                1, &batchedIndexBufferUpdateBarrier,
                0, nullptr
            );

            VkBufferCopy indexCopyRegion = {};
            indexCopyRegion.size = sizeof(uint32_t) * batchedIndexCount_;
            vkCmdCopyBuffer(commandBuffer, batchedIndexStagingBuffer_, batchedIndexBuffer_, 1, &indexCopyRegion);

            VkBufferMemoryBarrier batchedIndexBufferUpdateBarrier2 = {};
            batchedIndexBufferUpdateBarrier2.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            batchedIndexBufferUpdateBarrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            batchedIndexBufferUpdateBarrier2.dstAccessMask = VK_ACCESS_INDEX_READ_BIT;
            batchedIndexBufferUpdateBarrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedIndexBufferUpdateBarrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            batchedIndexBufferUpdateBarrier2.buffer = batchedIndexBuffer_;
            batchedIndexBufferUpdateBarrier2.offset = 0;
            batchedIndexBufferUpdateBarrier2.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                0,
                0, nullptr,
                1, &batchedIndexBufferUpdateBarrier2,
                0, nullptr
            );

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, batchedDataTransferFence_));

            // ---------------------------------------------------------------------

            _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &renderingFence_, VK_TRUE, UINT64_MAX));
            _VK_CHECK(vkResetFences(logicalDevice_, 1, &renderingFence_));

            ApplyTextureBindings();
            IncreaseTectureUnitStaleness();
            UpdateUniformBuffers();

            _VK_CHECK(vkResetCommandBuffer(renderingCommandBuffer_, 0));

            VkCommandBufferBeginInfo beginInfo2 = {};
            beginInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(renderingCommandBuffer_, &beginInfo2));

            VkMemoryBarrier renderingMemoryBarrier = {};
            renderingMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            renderingMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            renderingMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                renderingCommandBuffer_,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                1, &renderingMemoryBarrier,
                0, nullptr,
                0, nullptr
            );

            VkRenderPass currentRenderPass;
            VkFramebuffer currentFrameBuffer;
            VkPipeline currentGraphicsPipeline;
            if (multisampleAntiAliasing_)
            {
                if (depthTesting_)
                {
                    currentRenderPass = primaryRenderPass4_;
                    currentFrameBuffer = primaryFrameBuffer4_;
                    currentGraphicsPipeline = primaryGraphicsPipeline4_;
                }
                else
                {
                    currentRenderPass = primaryRenderPass3_;
                    currentFrameBuffer = primaryFrameBuffer3_;
                    currentGraphicsPipeline = primaryGraphicsPipeline3_;
                }
            }
            else
            {
                if (depthTesting_)
                {
                    currentRenderPass = primaryRenderPass2_;
                    currentFrameBuffer = primaryFrameBuffer2_;
                    currentGraphicsPipeline = primaryGraphicsPipeline2_;
                }
                else
                {
                    currentRenderPass = primaryRenderPass1_;
                    currentFrameBuffer = primaryFrameBuffer1_;
                    currentGraphicsPipeline = primaryGraphicsPipeline1_;
                }
            }

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = currentRenderPass;
            renderPassInfo.framebuffer = currentFrameBuffer;
            renderPassInfo.renderArea.offset = { (int32_t)(frameBufferWidth_ * cameraViewportX_), (int32_t)(frameBufferHeight_ * cameraViewportY_) };
            renderPassInfo.renderArea.extent = { (uint32_t)(frameBufferWidth_ * cameraViewportWidth_), (uint32_t)(frameBufferHeight_ * cameraViewportHeight_) };

            vkCmdBeginRenderPass(renderingCommandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(renderingCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, currentGraphicsPipeline);

            // viewport.y is non zero and viewport.height is negative to flip y axis
            // (making it behave like it does in OpenGL)
            // If VK_API_VERSION_1_1 < 1.1: requires VK_KHR_MAINTENANCE_1_EXTENSION_NAME
            VkViewport viewport = {};
            viewport.x = (float)frameBufferWidth_ * cameraViewportX_;
            viewport.y = (float)frameBufferHeight_ - (float)frameBufferHeight_ * cameraViewportY_;
            viewport.width = (float)frameBufferWidth_ * cameraViewportWidth_;
            viewport.height = -(float)frameBufferHeight_ * cameraViewportHeight_;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(renderingCommandBuffer_, 0, 1, &viewport);

            VkRect2D scissor = {};
            scissor.offset = { 0, 0 };
            scissor.extent = { frameBufferWidth_, frameBufferHeight_ };
            vkCmdSetScissor(renderingCommandBuffer_, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = { batchedVertexBuffer_ };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(renderingCommandBuffer_, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(renderingCommandBuffer_, batchedIndexBuffer_, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(renderingCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, primaryPipelineLayout_, 0, 1, &primaryDescriptorSet_, 0, nullptr);

            vkCmdDrawIndexed(renderingCommandBuffer_, (uint32_t)batchedIndexCount_, 1, 0, 0, 0);

            vkCmdEndRenderPass(renderingCommandBuffer_);

            _VK_CHECK(vkEndCommandBuffer(renderingCommandBuffer_));

            VkSubmitInfo submitInfo2 = {};
            submitInfo2.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo2.commandBufferCount = 1;
            submitInfo2.pCommandBuffers = &renderingCommandBuffer_;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo2, renderingFence_));

            batchedVertexCount_ = 0;
            batchedIndexCount_ = 0;

            justRenderedToBatch_ = true;
        }
    }

    void Vulkan_Renderer::FinishRendering()
    {
        if (framebufferResized_)
        {
            HandleFramebufferResize();
            framebufferResized_ = false;
        }

        if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0 &&
            surfaceExtent_.width > 0 && surfaceExtent_.height > 0)
        {
            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

            VkMemoryBarrier renderingMemoryBarrier = {};
            renderingMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            renderingMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            renderingMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                1, &renderingMemoryBarrier,
                0, nullptr,
                0, nullptr
            );

            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.image = screenTexture_.textureImage_;
            imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
            imageMemoryBarrier.subresourceRange.levelCount = 1;
            imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
            imageMemoryBarrier.subresourceRange.layerCount = 1;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier
            );

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = secondaryRenderPass_;
            renderPassInfo.framebuffer = swapchainFramebuffers_[currentSwapchainFramebufferIndex_];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = surfaceExtent_;

            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, secondaryGraphicsPipeline_);

            // viewport.y is non zero and viewport.height is negative to flip y axis
            // (making it behave like it does in OpenGL)
            // If VK_API_VERSION_1_1 < 1.1: requires VK_KHR_MAINTENANCE_1_EXTENSION_NAME
            VkViewport viewport = {};
            viewport.x = (float)viewportX_;
            viewport.y = (float)(viewportHeight_ + viewportY_);
            viewport.width = (float)viewportWidth_;
            viewport.height = -(float)viewportHeight_;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor = {};
            scissor.offset = { 0, 0 };
            scissor.extent = surfaceExtent_;
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = { screenVertexBuffer_ };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, secondaryPipelineLayout_, 0, 1, &secondaryDescriptorSet_, 0, nullptr);

            vkCmdDraw(commandBuffer, 6, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffer);

            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier
            );

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE));
        }
    }

    void Vulkan_Renderer::SwapBuffers()
    {
        if (swapchain_ != VK_NULL_HANDLE)
        {
            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

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
                commandBuffer,
                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier
            );

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &readyToPresentSemaphore_;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE));

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &readyToPresentSemaphore_;

            VkSwapchainKHR swapChains[] = { swapchain_ };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;

            presentInfo.pImageIndices = &currentSwapchainFramebufferIndex_;

            VkResult result = vkQueuePresentKHR(presentQueue_, &presentInfo);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
            {
                HandleSwapchainFramebufferResize();
            }
            else if (result != VK_SUCCESS)
            {
                LOG_ERROR_F("Vulkan Error: Failed to present swap chain image!");
            }
        }
        else
        {
            UpdateSurfaceCapabilities();

            if (surfaceExtent_.width > 0 && surfaceExtent_.height > 0)
            {
                CreateSwapchain();
                CreateSwapchainFramebuffers();
            }
        }

        AcquireNextImage();
    }

    // protected ---------------------------------------------------------------

    void Vulkan_Renderer::CreateVulkanInstance()
    {
        if (vulkanInstance_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: vulkanInstance_ already exists");
        }
        else if (debugMessenger_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: debugMessenger_ already exists");
        }
        else
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
                    LOG_ERROR_F("Vulkan Error: Validation layers requested, but not available!");
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
                    LOG_ERROR_F("Vulkan Error: Failed to find CreateDebugUtilsMessengerEXT funciton!");
                }
            }
        }
    }

    void Vulkan_Renderer::DeleteVulkanInstance()
    {
        if (s_enableValidationLayers_)
        {
            PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerFunction =
                (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkanInstance_, "vkDestroyDebugUtilsMessengerEXT");
            if (destroyDebugUtilsMessengerFunction != nullptr)
            {
                destroyDebugUtilsMessengerFunction(vulkanInstance_, debugMessenger_, nullptr);
                debugMessenger_ = VK_NULL_HANDLE;
            }
        }

        vkDestroyInstance(vulkanInstance_, nullptr);
        vulkanInstance_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateSurface()
    {
        if (surface_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: surface_ already exists");
        }
        else
        {
            windowPtr_->CreateWindowSurface(vulkanInstance_, surface_);
        }
    }

    void Vulkan_Renderer::DeleteSurface()
    {
        vkDestroySurfaceKHR(vulkanInstance_, surface_, nullptr);
        surface_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::PickPhysicalDevice()
    {
        if (physicalDevice_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: physicalDevice_ already selected");
        }
        else
        {
            uint32_t deviceCount = 0;
            _VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance_, &deviceCount, nullptr));

            if (deviceCount == 0)
            {
                LOG_ERROR_F("Vulkan Error: Failed to find GPUs with Vulkan support!");
            }

            std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
            _VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance_, &deviceCount, physicalDevices.data()));

            for (size_t i = 0; i < physicalDevices.size(); ++i)
            {
                const VkPhysicalDevice& physicalDevice = physicalDevices[i];

                // Find Required Queue Family Indices
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

                UpdateSurfaceCapabilities();

                viewportHeight_ = (unsigned int)surfaceExtent_.height;
                viewportWidth_ = (unsigned int)surfaceExtent_.width;

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
                    LOG_ERROR_F("Vulkan Error: Failed to find desired surface format!");
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
                    LOG_ERROR_F("Vulkan Error: Failed to find desired surface depth format!");
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
                    LOG_ERROR_F("Vulkan Error: Failed to find desired surface present mode!");
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
                LOG_ERROR_F("Vulkan Error: Failed to find a suitable GPU!");
            }
        }
    }

    void Vulkan_Renderer::UnPickPhysicalDevice()
    {
        physicalDevice_ = VK_NULL_HANDLE;
        graphicsQueueFamilyIndex_ = (uint32_t)-1;
        presentQueueFamilyIndex_ = (uint32_t)-1;
        surfaceCapabilities_ = {};
        surfaceExtent_ = {};
        surfaceFormat_ = {};
        formatProperties_ = {};
        depthFormat_ = VK_FORMAT_UNDEFINED;
        surfacePresentMode_ = VK_PRESENT_MODE_FIFO_KHR;
        physicalDeviceProperties_ = {};
        msaaSampleCount_ = VK_SAMPLE_COUNT_1_BIT;
    }

    void Vulkan_Renderer::CreateLogicalDevice()
    {
        if (logicalDevice_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: logicalDevice_ already exists");
        }
        else
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
    }

    void Vulkan_Renderer::DeleteLogicalDevice()
    {
        vkDestroyDevice(logicalDevice_, nullptr);
        logicalDevice_ = VK_NULL_HANDLE;
        graphicsQueue_ = VK_NULL_HANDLE;
        presentQueue_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateCommandPool()
    {
        if (commandPool_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: commandPool_ already exists");
        }
        else
        {
            VkCommandPoolCreateInfo commandPoolCreateInfo = {};
            commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            commandPoolCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex_;

            _VK_CHECK(vkCreateCommandPool(logicalDevice_, &commandPoolCreateInfo, nullptr, &commandPool_));
        }
    }

    void Vulkan_Renderer::DeleteCommandPool()
    {
        vkDestroyCommandPool(logicalDevice_, commandPool_, nullptr);
        commandPool_ = VK_NULL_HANDLE;
        renderingCommandBuffer_ = VK_NULL_HANDLE;
        commandBuffers_.clear();
    }

    void Vulkan_Renderer::CreateCommandBuffers()
    {
        if (renderingCommandBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: renderingCommandBuffer_ already exists");
        }
        else
        {
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool_;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            _VK_CHECK(vkAllocateCommandBuffers(logicalDevice_, &allocInfo, &renderingCommandBuffer_));
        }
    }

    void Vulkan_Renderer::CreateSyncObjects()
    {
        if (renderingFence_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: renderingFence_ already exists");
        }
        else if (batchedDataTransferFence_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedDataTransferFence_ already exists");
        }
        else if (instanceDataTransferFence_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: instanceDataTransferFence_ already exists");
        }
        else if (imageAvailableSemaphore_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: imageAvailableSemaphore_ already exists");
        }
        else if (readyToPresentSemaphore_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: readyToPresentSemaphore_ already exists");
        }
        else
        {
            VkFenceCreateInfo fenceInfo = {};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            _VK_CHECK(vkCreateFence(logicalDevice_, &fenceInfo, nullptr, &renderingFence_));
            _VK_CHECK(vkCreateFence(logicalDevice_, &fenceInfo, nullptr, &batchedDataTransferFence_));
            _VK_CHECK(vkCreateFence(logicalDevice_, &fenceInfo, nullptr, &instanceDataTransferFence_));

            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            _VK_CHECK(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &imageAvailableSemaphore_));
            _VK_CHECK(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &readyToPresentSemaphore_));
        }
    }

    void Vulkan_Renderer::DeleteSyncObjects()
    {
        vkDestroyFence(logicalDevice_, renderingFence_, nullptr);
        renderingFence_ = VK_NULL_HANDLE;
        vkDestroyFence(logicalDevice_, batchedDataTransferFence_, nullptr);
        batchedDataTransferFence_ = VK_NULL_HANDLE;
        vkDestroyFence(logicalDevice_, instanceDataTransferFence_, nullptr);
        instanceDataTransferFence_ = VK_NULL_HANDLE;
        vkDestroySemaphore(logicalDevice_, imageAvailableSemaphore_, nullptr);
        imageAvailableSemaphore_ = VK_NULL_HANDLE;
        vkDestroySemaphore(logicalDevice_, readyToPresentSemaphore_, nullptr);
        readyToPresentSemaphore_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateSwapchain()
    {
        if (surfaceExtent_.width == 0 || surfaceExtent_.height == 0)
        {
            LOG_ERROR_F("Vulkan Error: either surfaceExtent_ width or height is equal to 0");
        }
        else if (swapchain_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: swapchain_ already exists");
        }
        else
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
                CreateImageView(swapchainImages_[i], surfaceFormat_.format, VK_IMAGE_ASPECT_COLOR_BIT, 1, swapchainImageViews_[i]);
            }
        }
    }

    void Vulkan_Renderer::DeleteSwapchain()
    {
        swapchainImages_.clear();

        for (size_t i = 0; i < swapchainImageViews_.size(); ++i)
        {
            const VkImageView& imageView = swapchainImageViews_[i];
            vkDestroyImageView(logicalDevice_, imageView, nullptr);
        }
        swapchainImageViews_.clear();

        vkDestroySwapchainKHR(logicalDevice_, swapchain_, nullptr);
        swapchain_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateInstanceDataBuffers()
    {
        if (instanceStagingBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: instanceStagingBuffer_ already exists");
        }
        else if (instanceStagingBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: instanceStagingBufferMemory_ already exists");
        }
        else if (instanceBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: instanceBuffer_ already exists");
        }
        else if (instanceBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: instanceBufferMemory_ already exists");
        }
        else
        {
            VkDeviceSize instanceBufferSize = sizeof(InstanceData) * instanceBufferCapacity_;

            CreateBuffer(instanceBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, instanceStagingBuffer_, instanceStagingBufferMemory_);
            _VK_CHECK(vkMapMemory(logicalDevice_, instanceStagingBufferMemory_, 0, instanceBufferSize, 0, (void**)&instanceStagingBufferDataPtr_));

            CreateBuffer(instanceBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, instanceBuffer_, instanceBufferMemory_);
        }
    }

    void Vulkan_Renderer::DeleteInstanceDataBuffers()
    {
        vkDestroyBuffer(logicalDevice_, instanceBuffer_, nullptr);
        instanceBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, instanceBufferMemory_, nullptr);
        instanceBufferMemory_ = VK_NULL_HANDLE;
        vkUnmapMemory(logicalDevice_, instanceStagingBufferMemory_);
        vkDestroyBuffer(logicalDevice_, instanceStagingBuffer_, nullptr);
        instanceStagingBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, instanceStagingBufferMemory_, nullptr);
        instanceStagingBufferMemory_ = VK_NULL_HANDLE;
        instanceStagingBufferDataPtr_ = NULL;
        instanceCount_ = 0;
    }

    void Vulkan_Renderer::CreateBatchedIndexDataBuffers()
    {
        if (batchedIndexStagingBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedIndexStagingBuffer_ already exists");
        }
        else if (batchedIndexStagingBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedIndexStagingBufferMemory_ already exists");
        }
        else if (batchedIndexBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedIndexBuffer_ already exists");
        }
        else if (batchedIndexBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedIndexBufferMemory_ already exists");
        }
        else
        {
            VkDeviceSize batchedIndexBufferSize = sizeof(uint32_t) * batchedIndexBufferCapacity_;

            CreateBuffer(batchedIndexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, batchedIndexStagingBuffer_, batchedIndexStagingBufferMemory_);
            _VK_CHECK(vkMapMemory(logicalDevice_, batchedIndexStagingBufferMemory_, 0, batchedIndexBufferSize, 0, (void**)&batchedIndexStagingBufferDataPtr_));

            CreateBuffer(batchedIndexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, batchedIndexBuffer_, batchedIndexBufferMemory_);
        }
    }

    void Vulkan_Renderer::DeleteBatchedIndexDataBuffers()
    {
        vkDestroyBuffer(logicalDevice_, batchedIndexBuffer_, nullptr);
        batchedIndexBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, batchedIndexBufferMemory_, nullptr);
        batchedIndexBufferMemory_ = VK_NULL_HANDLE;
        vkUnmapMemory(logicalDevice_, batchedIndexStagingBufferMemory_);
        vkDestroyBuffer(logicalDevice_, batchedIndexStagingBuffer_, nullptr);
        batchedIndexStagingBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, batchedIndexStagingBufferMemory_, nullptr);
        batchedIndexStagingBufferMemory_ = VK_NULL_HANDLE;
        batchedIndexStagingBufferDataPtr_ = NULL;
        batchedIndexCount_ = 0;
    }

    void Vulkan_Renderer::CreateBatchedVertexDataBuffers()
    {
        if (batchedVertexStagingBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedVertexStagingBuffer_ already exists");
        }
        else if (batchedVertexStagingBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedVertexStagingBufferMemory_ already exists");
        }
        else if (batchedVertexBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedVertexBuffer_ already exists");
        }
        else if (batchedVertexBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: batchedVertexBufferMemory_ already exists");
        }
        else
        {
            VkDeviceSize batchedVertexBufferSize = sizeof(BatchedVertexData) * batchedVertexBufferCapacity_;

            CreateBuffer(batchedVertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, batchedVertexStagingBuffer_, batchedVertexStagingBufferMemory_);
            _VK_CHECK(vkMapMemory(logicalDevice_, batchedVertexStagingBufferMemory_, 0, batchedVertexBufferSize, 0, (void**)&batchedVertexStagingBufferDataPtr_));

            CreateBuffer(batchedVertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, batchedVertexBuffer_, batchedVertexBufferMemory_);
        }
    }

    void Vulkan_Renderer::DeleteBatchedVertexDataBuffers()
    {
        vkDestroyBuffer(logicalDevice_, batchedVertexBuffer_, nullptr);
        batchedVertexBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, batchedVertexBufferMemory_, nullptr);
        batchedVertexBufferMemory_ = VK_NULL_HANDLE;
        vkUnmapMemory(logicalDevice_, batchedVertexStagingBufferMemory_);
        vkDestroyBuffer(logicalDevice_, batchedVertexStagingBuffer_, nullptr);
        batchedVertexStagingBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, batchedVertexStagingBufferMemory_, nullptr);
        batchedVertexStagingBufferMemory_ = VK_NULL_HANDLE;
        batchedVertexStagingBufferDataPtr_ = NULL;
        batchedVertexCount_ = 0;
    }

    void Vulkan_Renderer::CreateOtherDataBuffers()
    {
        if (vertexShaderUniformBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: vertexShaderUniformBuffer_ already exists");
        }
        else if (vertexShaderUniformBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: vertexShaderUniformBufferMemory_ already exists");
        }
        else if (fragmentShaderUniformBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: fragmentShaderUniformBuffer_ already exists");
        }
        else if (fragmentShaderUniformBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: fragmentShaderUniformBufferMemory_ already exists");
        }
        else if (screenVertexBuffer_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: screenVertexBuffer_ already exists");
        }
        else if (screenVertexBufferMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: screenVertexBufferMemory_ already exists");
        }
        else
        {
            CreateBuffer(sizeof(VertexShaderUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexShaderUniformBuffer_, vertexShaderUniformBufferMemory_);

            CreateBuffer(sizeof(FragmentShaderUniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, fragmentShaderUniformBuffer_, fragmentShaderUniformBufferMemory_);

            float screenQuadVertices[] = {
                // positions   // textureCoordinates
                -1.0f,  1.0f,  0.0f, 0.0f,
                -1.0f, -1.0f,  0.0f, 1.0f,
                 1.0f, -1.0f,  1.0f, 1.0f,

                -1.0f,  1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f, 0.0f
            };

            size_t screenVerticesSize = sizeof(screenQuadVertices);
            VkBuffer screenVertexStagingBuffer;
            VkDeviceMemory screenVertexStagingBufferMemory;
            CreateBuffer(screenVerticesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, screenVertexStagingBuffer, screenVertexStagingBufferMemory);

            void* screenVertexStagingBufferDataPtr;
            _VK_CHECK(vkMapMemory(logicalDevice_, screenVertexStagingBufferMemory, 0, screenVerticesSize, 0, &screenVertexStagingBufferDataPtr));
            memcpy(screenVertexStagingBufferDataPtr, screenQuadVertices, screenVerticesSize);

            CreateBuffer(screenVerticesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, screenVertexBuffer_, screenVertexBufferMemory_);

            VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

            VkBufferCopy vertexCopyRegion = {};
            vertexCopyRegion.size = screenVerticesSize;
            vkCmdCopyBuffer(commandBuffer, screenVertexStagingBuffer, screenVertexBuffer_, 1, &vertexCopyRegion);

            EndSingleTimeCommands(commandBuffer);

            vkUnmapMemory(logicalDevice_, screenVertexStagingBufferMemory);
            vkDestroyBuffer(logicalDevice_, screenVertexStagingBuffer, nullptr);
            vkFreeMemory(logicalDevice_, screenVertexStagingBufferMemory, nullptr);
        }
    }

    void Vulkan_Renderer::DeleteOtherDataBuffers()
    {
        vkDestroyBuffer(logicalDevice_, screenVertexBuffer_, nullptr);
        screenVertexBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, screenVertexBufferMemory_, nullptr);
        screenVertexBufferMemory_ = VK_NULL_HANDLE;
        vkDestroyBuffer(logicalDevice_, fragmentShaderUniformBuffer_, nullptr);
        fragmentShaderUniformBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, fragmentShaderUniformBufferMemory_, nullptr);
        fragmentShaderUniformBufferMemory_ = VK_NULL_HANDLE;
        vkDestroyBuffer(logicalDevice_, vertexShaderUniformBuffer_, nullptr);
        vertexShaderUniformBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, vertexShaderUniformBufferMemory_, nullptr);
        vertexShaderUniformBufferMemory_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateDefaultTexture()
    {
        if (defaultTexture_.Initialized())
        {
            LOG_ERROR_F("Vulkan Error: defaultTexture_ already exists");
        }
        else
        {
            unsigned char data[4] = { 255, 255, 255, 255 };
            CreateTexture(data, 1, 1, 4, false, false, defaultTexture_);
        }
    }

    void Vulkan_Renderer::DeleteDefaultTexture()
    {
        DeleteTexture(defaultTexture_);
        defaultTexture_ = {};
    }

    void Vulkan_Renderer::CreateDescriptorSetLayouts()
    {
        if (primaryDescriptorSetLayout_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryDescriptorSetLayout_ already exists");
        }
        else if (secondaryDescriptorSetLayout_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: secondaryDescriptorSetLayout_ already exists");
        }
        else
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

            _VK_CHECK(vkCreateDescriptorSetLayout(logicalDevice_, &layoutInfo, nullptr, &primaryDescriptorSetLayout_));

            samplerLayoutBinding = {};
            samplerLayoutBinding.binding = 0;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.pImmutableSamplers = nullptr;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = 1;
            layoutInfo.pBindings = &samplerLayoutBinding;

            _VK_CHECK(vkCreateDescriptorSetLayout(logicalDevice_, &layoutInfo, nullptr, &secondaryDescriptorSetLayout_));
        }
    }

    void Vulkan_Renderer::DeleteDescriptorSetLayouts()
    {
        vkDestroyDescriptorSetLayout(logicalDevice_, secondaryDescriptorSetLayout_, nullptr);
        secondaryDescriptorSetLayout_ = VK_NULL_HANDLE;
        vkDestroyDescriptorSetLayout(logicalDevice_, primaryDescriptorSetLayout_, nullptr);
        primaryDescriptorSetLayout_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateDescriptorPool()
    {
        if (descriptorPool_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: descriptorPool_ already exists");
        }
        else
        {
            std::array<VkDescriptorPoolSize, 2> poolSizes = {};
            poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[0].descriptorCount = 2;
            poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[1].descriptorCount = NUMBER_OF_TEXTURE_UNITS + 1;

            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.maxSets = 2;

            _VK_CHECK(vkCreateDescriptorPool(logicalDevice_, &poolInfo, nullptr, &descriptorPool_));
        }
    }

    void Vulkan_Renderer::DeleteDescriptorPool()
    {
        vkDestroyDescriptorPool(logicalDevice_, descriptorPool_, nullptr);
        descriptorPool_ = VK_NULL_HANDLE;
        primaryDescriptorSet_ = VK_NULL_HANDLE;
        secondaryDescriptorSet_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateDescriptorSets()
    {
        if (primaryDescriptorSet_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryDescriptorSet_ already exists");
        }
        else if (secondaryDescriptorSet_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: secondaryDescriptorSet_ already exists");
        }
        else
        {
            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = descriptorPool_;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &primaryDescriptorSetLayout_;

            _VK_CHECK(vkAllocateDescriptorSets(logicalDevice_, &allocInfo, &primaryDescriptorSet_));

            allocInfo.pSetLayouts = &secondaryDescriptorSetLayout_;

            _VK_CHECK(vkAllocateDescriptorSets(logicalDevice_, &allocInfo, &secondaryDescriptorSet_));

            std::array<VkWriteDescriptorSet, 2 + NUMBER_OF_TEXTURE_UNITS + 1> descriptorWrites = {};

            VkDescriptorBufferInfo bufferInfo1 = {};
            bufferInfo1.buffer = vertexShaderUniformBuffer_;
            bufferInfo1.offset = 0;
            bufferInfo1.range = sizeof(VertexShaderUniformBufferObject);

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = primaryDescriptorSet_;
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
            descriptorWrites[1].dstSet = primaryDescriptorSet_;
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
                descriptorWrites[2 + i].dstSet = primaryDescriptorSet_;
                descriptorWrites[2 + i].dstBinding = 2;
                descriptorWrites[2 + i].dstArrayElement = (uint32_t)i;
                descriptorWrites[2 + i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[2 + i].descriptorCount = 1;
                descriptorWrites[2 + i].pImageInfo = &imageInfo;
            }

            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].dstSet = secondaryDescriptorSet_;
            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].dstBinding = 0;
            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].dstArrayElement = 0;
            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].descriptorCount = 1;
            descriptorWrites[2 + NUMBER_OF_TEXTURE_UNITS].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(logicalDevice_, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
    }

    void Vulkan_Renderer::CreatePipelineLayouts()
    {
        if (primaryPipelineLayout_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryPipelineLayout_ already exists");
        }
        else if (secondaryPipelineLayout_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: secondaryPipelineLayout_ already exists");
        }
        else
        {
            VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &primaryDescriptorSetLayout_;

            _VK_CHECK(vkCreatePipelineLayout(logicalDevice_, &pipelineLayoutInfo, nullptr, &primaryPipelineLayout_));

            pipelineLayoutInfo.pSetLayouts = &secondaryDescriptorSetLayout_;

            _VK_CHECK(vkCreatePipelineLayout(logicalDevice_, &pipelineLayoutInfo, nullptr, &secondaryPipelineLayout_));
        }
    }

    void Vulkan_Renderer::DeletePipelineLayouts()
    {
        vkDestroyPipelineLayout(logicalDevice_, secondaryPipelineLayout_, nullptr);
        secondaryPipelineLayout_ = VK_NULL_HANDLE;
        vkDestroyPipelineLayout(logicalDevice_, primaryPipelineLayout_, nullptr);
        primaryPipelineLayout_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateBatchShaderModules()
    {
        if (vertexBatchShaderModule_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: vertexBatchShaderModule_ already exists");
        }
        else if (fragmentBatchShaderModule_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: fragmentBatchShaderModule_ already exists");
        }
        else
        {
            vertexBatchShaderModule_ = CreateShaderModule(
                g_v_primary_vert_spv,
                sizeof(g_v_primary_vert_spv) / sizeof(const unsigned char)
            );

            fragmentBatchShaderModule_ = CreateShaderModule(
                g_v_primary_frag_spv,
                sizeof(g_v_primary_frag_spv) / sizeof(const unsigned char)
            );
        }
    }

    void Vulkan_Renderer::DeleteBatchShaderModules()
    {
        vkDestroyShaderModule(logicalDevice_, fragmentBatchShaderModule_, nullptr);
        fragmentBatchShaderModule_ = VK_NULL_HANDLE;
        vkDestroyShaderModule(logicalDevice_, vertexBatchShaderModule_, nullptr);
        vertexBatchShaderModule_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreatePrimaryRenderPasses()
    {
        if (primaryRenderPass1_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryRenderPass1_ already exists");
        }
        else if (primaryRenderPass2_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryRenderPass2_ already exists");
        }
        else if (primaryRenderPass3_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryRenderPass3_ already exists");
        }
        else if (primaryRenderPass4_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryRenderPass4_ already exists");
        }
        else
        {
            primaryRenderPass1_ = CreatePrimaryRenderPass(false, false);
            primaryRenderPass2_ = CreatePrimaryRenderPass(false, true);
            primaryRenderPass3_ = CreatePrimaryRenderPass(true, false);
            primaryRenderPass4_ = CreatePrimaryRenderPass(true, true);
        }
    }

    void Vulkan_Renderer::DeletePrimaryRenderPasses()
    {
        vkDestroyRenderPass(logicalDevice_, primaryRenderPass4_, nullptr);
        primaryRenderPass4_ = VK_NULL_HANDLE;
        vkDestroyRenderPass(logicalDevice_, primaryRenderPass3_, nullptr);
        primaryRenderPass3_ = VK_NULL_HANDLE;
        vkDestroyRenderPass(logicalDevice_, primaryRenderPass2_, nullptr);
        primaryRenderPass2_ = VK_NULL_HANDLE;
        vkDestroyRenderPass(logicalDevice_, primaryRenderPass1_, nullptr);
        primaryRenderPass1_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreatePrimaryGraphicsPipelines()
    {
        if (primaryGraphicsPipeline1_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryGraphicsPipeline1_ already exists");
        }
        else if (primaryGraphicsPipeline2_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryGraphicsPipeline2_ already exists");
        }
        else if (primaryGraphicsPipeline3_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryGraphicsPipeline3_ already exists");
        }
        else if (primaryGraphicsPipeline4_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: primaryGraphicsPipeline4_ already exists");
        }
        else
        {
            std::vector<VkVertexInputBindingDescription> bindingDescriptions1;
            bindingDescriptions1.resize(1);

            bindingDescriptions1[0].binding = 0;
            bindingDescriptions1[0].stride = sizeof(BatchedVertexData);
            bindingDescriptions1[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            std::vector<VkVertexInputAttributeDescription> attributeDescriptions1;
            attributeDescriptions1.resize(11);

            attributeDescriptions1[0].binding = 0;
            attributeDescriptions1[0].location = 0;
            attributeDescriptions1[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions1[0].offset = offsetof(BatchedVertexData, position);

            attributeDescriptions1[1].binding = 0;
            attributeDescriptions1[1].location = 1;
            attributeDescriptions1[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions1[1].offset = offsetof(BatchedVertexData, textureCoordinate);

            attributeDescriptions1[2].binding = 0;
            attributeDescriptions1[2].location = 2;
            attributeDescriptions1[2].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions1[2].offset = offsetof(BatchedVertexData, normal);

            attributeDescriptions1[3].binding = 0;
            attributeDescriptions1[3].location = 3;
            attributeDescriptions1[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions1[3].offset = offsetof(BatchedVertexData, color);

            attributeDescriptions1[4].binding = 0;
            attributeDescriptions1[4].location = 4;
            attributeDescriptions1[4].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions1[4].offset = offsetof(BatchedVertexData, textureUnit);

            attributeDescriptions1[5].binding = 0;
            attributeDescriptions1[5].location = 5;
            attributeDescriptions1[5].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions1[5].offset = offsetof(BatchedVertexData, specularUnit);

            attributeDescriptions1[6].binding = 0;
            attributeDescriptions1[6].location = 6;
            attributeDescriptions1[6].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions1[6].offset = offsetof(BatchedVertexData, shininess);

            attributeDescriptions1[7].binding = 0;
            attributeDescriptions1[7].location = 7;
            attributeDescriptions1[7].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions1[7].offset = offsetof(BatchedVertexData, scale);

            attributeDescriptions1[8].binding = 0;
            attributeDescriptions1[8].location = 8;
            attributeDescriptions1[8].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions1[8].offset = offsetof(BatchedVertexData, translation);

            attributeDescriptions1[9].binding = 0;
            attributeDescriptions1[9].location = 9;
            attributeDescriptions1[9].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions1[9].offset = offsetof(BatchedVertexData, orientation);

            attributeDescriptions1[10].binding = 0;
            attributeDescriptions1[10].location = 10;
            attributeDescriptions1[10].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions1[10].offset = offsetof(BatchedVertexData, lit);

            primaryGraphicsPipeline1_ = CreatePrimaryGraphicsPipeline(primaryRenderPass1_, bindingDescriptions1, attributeDescriptions1, false, false);
            primaryGraphicsPipeline2_ = CreatePrimaryGraphicsPipeline(primaryRenderPass2_, bindingDescriptions1, attributeDescriptions1, false, true);
            primaryGraphicsPipeline3_ = CreatePrimaryGraphicsPipeline(primaryRenderPass3_, bindingDescriptions1, attributeDescriptions1, true, false);
            primaryGraphicsPipeline4_ = CreatePrimaryGraphicsPipeline(primaryRenderPass4_, bindingDescriptions1, attributeDescriptions1, true, true);

            std::vector<VkVertexInputBindingDescription> bindingDescriptions2;
            bindingDescriptions2.resize(2);

            bindingDescriptions2[0].binding = 0;
            bindingDescriptions2[0].stride = sizeof(VertexData);
            bindingDescriptions2[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            bindingDescriptions2[1].binding = 1;
            bindingDescriptions2[1].stride = sizeof(InstanceData);
            bindingDescriptions2[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

            std::vector<VkVertexInputAttributeDescription> attributeDescriptions2;
            attributeDescriptions2.resize(11);

            attributeDescriptions2[0].binding = 0;
            attributeDescriptions2[0].location = 0;
            attributeDescriptions2[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions2[0].offset = offsetof(VertexData, position);

            attributeDescriptions2[1].binding = 0;
            attributeDescriptions2[1].location = 1;
            attributeDescriptions2[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions2[1].offset = offsetof(VertexData, textureCoordinate);

            attributeDescriptions2[2].binding = 0;
            attributeDescriptions2[2].location = 2;
            attributeDescriptions2[2].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions2[2].offset = offsetof(VertexData, normal);

            attributeDescriptions2[3].binding = 1;
            attributeDescriptions2[3].location = 3;
            attributeDescriptions2[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions2[3].offset = offsetof(InstanceData, color);

            attributeDescriptions2[4].binding = 1;
            attributeDescriptions2[4].location = 4;
            attributeDescriptions2[4].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions2[4].offset = offsetof(InstanceData, textureUnit);

            attributeDescriptions2[5].binding = 1;
            attributeDescriptions2[5].location = 5;
            attributeDescriptions2[5].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions2[5].offset = offsetof(InstanceData, specularUnit);

            attributeDescriptions2[6].binding = 1;
            attributeDescriptions2[6].location = 6;
            attributeDescriptions2[6].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions2[6].offset = offsetof(InstanceData, shininess);

            attributeDescriptions2[7].binding = 1;
            attributeDescriptions2[7].location = 7;
            attributeDescriptions2[7].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions2[7].offset = offsetof(InstanceData, scale);

            attributeDescriptions2[8].binding = 1;
            attributeDescriptions2[8].location = 8;
            attributeDescriptions2[8].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions2[8].offset = offsetof(InstanceData, translation);

            attributeDescriptions2[9].binding = 1;
            attributeDescriptions2[9].location = 9;
            attributeDescriptions2[9].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions2[9].offset = offsetof(InstanceData, orientation);

            attributeDescriptions2[10].binding = 1;
            attributeDescriptions2[10].location = 10;
            attributeDescriptions2[10].format = VK_FORMAT_R32_SFLOAT;
            attributeDescriptions2[10].offset = offsetof(InstanceData, lit);

            primaryGraphicsPipeline5_ = CreatePrimaryGraphicsPipeline(primaryRenderPass1_, bindingDescriptions2, attributeDescriptions2, false, false);
            primaryGraphicsPipeline6_ = CreatePrimaryGraphicsPipeline(primaryRenderPass2_, bindingDescriptions2, attributeDescriptions2, false, true);
            primaryGraphicsPipeline7_ = CreatePrimaryGraphicsPipeline(primaryRenderPass3_, bindingDescriptions2, attributeDescriptions2, true, false);
            primaryGraphicsPipeline8_ = CreatePrimaryGraphicsPipeline(primaryRenderPass4_, bindingDescriptions2, attributeDescriptions2, true, true);
        }
    }

    void Vulkan_Renderer::DeletePrimaryGraphicsPipelines()
    {
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline8_, nullptr);
        primaryGraphicsPipeline8_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline7_, nullptr);
        primaryGraphicsPipeline7_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline6_, nullptr);
        primaryGraphicsPipeline6_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline5_, nullptr);
        primaryGraphicsPipeline5_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline4_, nullptr);
        primaryGraphicsPipeline4_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline3_, nullptr);
        primaryGraphicsPipeline3_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline2_, nullptr);
        primaryGraphicsPipeline2_ = VK_NULL_HANDLE;
        vkDestroyPipeline(logicalDevice_, primaryGraphicsPipeline1_, nullptr);
        primaryGraphicsPipeline1_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreatePrimaryFrameBufferAttachments()
    {
        if (screenTexture_.Initialized())
        {
            LOG_ERROR_F("Vulkan Error: screenTexture_ already exists");
        }
        else
        {
            // Create Screen Texture
            // -----------------------------------------------------------------

            CreateImage(
                frameBufferWidth_,
                frameBufferHeight_,
                1,
                VK_SAMPLE_COUNT_1_BIT,
                s_textureFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                screenTexture_.textureImage_,
                screenTexture_.textureImageMemory_
            );

            CreateImageView(
                screenTexture_.textureImage_,
                s_textureFormat,
                VK_IMAGE_ASPECT_COLOR_BIT,
                1,
                screenTexture_.textureImageView_
            );

            TransitionImageLayout(
                screenTexture_.textureImage_,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_ASPECT_COLOR_BIT,
                1
            );

            VkSamplerCreateInfo samplerInfo = {};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_NEAREST;
            samplerInfo.minFilter = VK_FILTER_NEAREST;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 1.0f;
            samplerInfo.mipLodBias = 0.0f;

            _VK_CHECK(vkCreateSampler(logicalDevice_, &samplerInfo, nullptr, &screenTexture_.textureSampler_));

            ApplyScreenTextureBinding();
        }

        if (depthImage_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: depthImage_ already exists");
        }
        else if(depthImageMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: depthImageMemory_ already exists");
        }
        else if (depthImageView_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: depthImageView_ already exists");
        }
        else
        {
            // Create Depth Image
            // -----------------------------------------------------------------

            CreateImage(
                frameBufferWidth_,
                frameBufferHeight_,
                1,
                VK_SAMPLE_COUNT_1_BIT,
                depthFormat_,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                depthImage_,
                depthImageMemory_
            );

            CreateImageView(
                depthImage_,
                depthFormat_,
                VK_IMAGE_ASPECT_DEPTH_BIT,
                1,
                depthImageView_
            );

            TransitionImageLayout(
                depthImage_,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                VK_IMAGE_ASPECT_DEPTH_BIT,
                1
            );
        }

        if (msaaImage_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: msaaImage_ already exists");
        }
        else if (msaaImageMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: msaaImageMemory_ already exists");
        }
        else if (msaaImageView_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: msaaImageView_ already exists");
        }
        else
        {
            // Create MSAA Image
            // -----------------------------------------------------------------

            CreateImage(
                frameBufferWidth_,
                frameBufferHeight_,
                1,
                msaaSampleCount_,
                s_textureFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                msaaImage_,
                msaaImageMemory_
            );

            CreateImageView(
                msaaImage_,
                s_textureFormat,
                VK_IMAGE_ASPECT_COLOR_BIT,
                1,
                msaaImageView_
            );

            TransitionImageLayout(
                msaaImage_,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_ASPECT_COLOR_BIT,
                1
            );
        }

        if (msaaDepthImage_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: msaaDepthImage_ already exists");
        }
        else if (msaaDepthImageMemory_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: msaaDepthImageMemory_ already exists");
        }
        else if (msaaDepthImageView_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: msaaDepthImageView_ already exists");
        }
        else
        {
            // Create MSAA Depth Image
            // -----------------------------------------------------------------

            CreateImage(
                frameBufferWidth_,
                frameBufferHeight_,
                1,
                msaaSampleCount_,
                depthFormat_,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                msaaDepthImage_,
                msaaDepthImageMemory_
            );

            CreateImageView(
                msaaDepthImage_,
                depthFormat_,
                VK_IMAGE_ASPECT_DEPTH_BIT,
                1,
                msaaDepthImageView_
            );

            TransitionImageLayout(
                msaaDepthImage_,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                VK_IMAGE_ASPECT_DEPTH_BIT,
                1
            );
        }
    }

    void Vulkan_Renderer::DeletePrimaryFrameBufferAttachments()
    {
        vkDestroyImageView(logicalDevice_, msaaDepthImageView_, nullptr);
        msaaDepthImageView_ = VK_NULL_HANDLE;
        vkDestroyImage(logicalDevice_, msaaDepthImage_, nullptr);
        msaaDepthImage_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, msaaDepthImageMemory_, nullptr);
        msaaDepthImageMemory_ = VK_NULL_HANDLE;

        vkDestroyImageView(logicalDevice_, msaaImageView_, nullptr);
        msaaImageView_ = VK_NULL_HANDLE;
        vkDestroyImage(logicalDevice_, msaaImage_, nullptr);
        msaaImage_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, msaaImageMemory_, nullptr);
        msaaImageMemory_ = VK_NULL_HANDLE;

        vkDestroyImageView(logicalDevice_, depthImageView_, nullptr);
        depthImageView_ = VK_NULL_HANDLE;
        vkDestroyImage(logicalDevice_, depthImage_, nullptr);
        depthImage_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, depthImageMemory_, nullptr);
        depthImageMemory_ = VK_NULL_HANDLE;

        DeleteTexture(screenTexture_);
        screenTexture_ = {};
    }

    void Vulkan_Renderer::CreatePrimaryFrameBuffers()
    {
        if (primaryFrameBuffer1_)
        {
            LOG_ERROR_F("Vulkan Error: primaryFrameBuffer1_ already exists");
        }
        else
        {
            std::array<VkImageView, 1> attachments1 = {
                screenTexture_.textureImageView_
            };

            VkFramebufferCreateInfo framebufferCreateInfo1 = {};
            framebufferCreateInfo1.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo1.renderPass = primaryRenderPass1_;
            framebufferCreateInfo1.attachmentCount = (uint32_t)attachments1.size();
            framebufferCreateInfo1.pAttachments = attachments1.data();
            framebufferCreateInfo1.width = frameBufferWidth_;
            framebufferCreateInfo1.height = frameBufferHeight_;
            framebufferCreateInfo1.layers = 1;

            _VK_CHECK(vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo1, nullptr, &primaryFrameBuffer1_));
        }

        if (primaryFrameBuffer2_)
        {
            LOG_ERROR_F("Vulkan Error: primaryFrameBuffer2_ already exists");
        }
        else
        {
            std::array<VkImageView, 2> attachments2 = {
                screenTexture_.textureImageView_,
                depthImageView_
            };

            VkFramebufferCreateInfo framebufferCreateInfo2 = {};
            framebufferCreateInfo2.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo2.renderPass = primaryRenderPass2_;
            framebufferCreateInfo2.attachmentCount = (uint32_t)attachments2.size();
            framebufferCreateInfo2.pAttachments = attachments2.data();
            framebufferCreateInfo2.width = frameBufferWidth_;
            framebufferCreateInfo2.height = frameBufferHeight_;
            framebufferCreateInfo2.layers = 1;

            _VK_CHECK(vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo2, nullptr, &primaryFrameBuffer2_));
        }

        if (primaryFrameBuffer3_)
        {
            LOG_ERROR_F("Vulkan Error: primaryFrameBuffer3_ already exists");
        }
        else
        {
            std::array<VkImageView, 2> attachments3 = {
                msaaImageView_,
                screenTexture_.textureImageView_
            };

            VkFramebufferCreateInfo framebufferCreateInfo3 = {};
            framebufferCreateInfo3.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo3.renderPass = primaryRenderPass3_;
            framebufferCreateInfo3.attachmentCount = (uint32_t)attachments3.size();
            framebufferCreateInfo3.pAttachments = attachments3.data();
            framebufferCreateInfo3.width = frameBufferWidth_;
            framebufferCreateInfo3.height = frameBufferHeight_;
            framebufferCreateInfo3.layers = 1;

            _VK_CHECK(vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo3, nullptr, &primaryFrameBuffer3_));
        }

        if (primaryFrameBuffer4_)
        {
            LOG_ERROR_F("Vulkan Error: primaryFrameBuffer4_ already exists");
        }
        else
        {
            std::array<VkImageView, 3> attachments4 = {
                msaaImageView_,
                msaaDepthImageView_,
                screenTexture_.textureImageView_
            };

            VkFramebufferCreateInfo framebufferCreateInfo4 = {};
            framebufferCreateInfo4.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo4.renderPass = primaryRenderPass4_;
            framebufferCreateInfo4.attachmentCount = (uint32_t)attachments4.size();
            framebufferCreateInfo4.pAttachments = attachments4.data();
            framebufferCreateInfo4.width = frameBufferWidth_;
            framebufferCreateInfo4.height = frameBufferHeight_;
            framebufferCreateInfo4.layers = 1;

            _VK_CHECK(vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo4, nullptr, &primaryFrameBuffer4_));
        }
    }

    void Vulkan_Renderer::DeletePrimaryFrameBuffers()
    {
        vkDestroyFramebuffer(logicalDevice_, primaryFrameBuffer4_, nullptr);
        primaryFrameBuffer4_ = VK_NULL_HANDLE;
        vkDestroyFramebuffer(logicalDevice_, primaryFrameBuffer3_, nullptr);
        primaryFrameBuffer3_ = VK_NULL_HANDLE;
        vkDestroyFramebuffer(logicalDevice_, primaryFrameBuffer2_, nullptr);
        primaryFrameBuffer2_ = VK_NULL_HANDLE;
        vkDestroyFramebuffer(logicalDevice_, primaryFrameBuffer1_, nullptr);
        primaryFrameBuffer1_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateScreenShaderModules()
    {
        if (vertexScreenShaderModule_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: vertexScreenShaderModule_ already exists");
        }
        else if (fragmentScreenShaderModule_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: fragmentScreenShaderModule_ already exists");
        }
        else
        {
            vertexScreenShaderModule_ = CreateShaderModule(
                g_v_screen_vert_spv,
                sizeof(g_v_screen_vert_spv) / sizeof(const unsigned char)
            );

            fragmentScreenShaderModule_ = CreateShaderModule(
                g_v_screen_frag_spv,
                sizeof(g_v_screen_frag_spv) / sizeof(const unsigned char)
            );
        }
    }

    void Vulkan_Renderer::DeleteScreenShaderModules()
    {
        vkDestroyShaderModule(logicalDevice_, fragmentScreenShaderModule_, nullptr);
        fragmentScreenShaderModule_ = VK_NULL_HANDLE;
        vkDestroyShaderModule(logicalDevice_, vertexScreenShaderModule_, nullptr);
        vertexScreenShaderModule_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateSecondaryRenderPass()
    {
        if (secondaryRenderPass_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: secondaryRenderPass_ already exists");
        }
        else
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

            _VK_CHECK(vkCreateRenderPass(logicalDevice_, &renderPassCreateInfo, nullptr, &secondaryRenderPass_));
        }
    }

    void Vulkan_Renderer::DeleteSecondaryRenderPass()
    {
        vkDestroyRenderPass(logicalDevice_, secondaryRenderPass_, nullptr);
        secondaryRenderPass_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateSecondaryGraphicsPipeline()
    {
        if (secondaryGraphicsPipeline_ != VK_NULL_HANDLE)
        {
            LOG_ERROR_F("Vulkan Error: secondaryGraphicsPipeline_ already exists");
        }
        else
        {
            VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
            vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = vertexScreenShaderModule_;
            vertShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
            fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = fragmentScreenShaderModule_;
            fragShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

            VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0;
            bindingDescription.stride = 16;
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = 0;

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[1].offset = 8;

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
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
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
            pipelineInfo.layout = secondaryPipelineLayout_;
            pipelineInfo.renderPass = secondaryRenderPass_;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

            _VK_CHECK(vkCreateGraphicsPipelines(logicalDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &secondaryGraphicsPipeline_));
        }
    }

    void Vulkan_Renderer::DeleteSecondaryGraphicsPipeline()
    {
        vkDestroyPipeline(logicalDevice_, secondaryGraphicsPipeline_, nullptr);
        secondaryGraphicsPipeline_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateSwapchainFramebuffers()
    {
        swapchainFramebuffers_.resize(swapchainImageCount_);
        for (size_t i = 0; i < swapchainImageViews_.size(); ++i)
        {
            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = secondaryRenderPass_;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &swapchainImageViews_[i];
            framebufferCreateInfo.width = surfaceExtent_.width;
            framebufferCreateInfo.height = surfaceExtent_.height;
            framebufferCreateInfo.layers = 1;

            _VK_CHECK(vkCreateFramebuffer(logicalDevice_, &framebufferCreateInfo, nullptr, &swapchainFramebuffers_[i]));
        }
    }

    void Vulkan_Renderer::DeleteSwapchainFramebuffers()
    {
        for (size_t i = 0; i < swapchainFramebuffers_.size(); ++i)
        {
            const VkFramebuffer& framebuffer = swapchainFramebuffers_[i];
            vkDestroyFramebuffer(logicalDevice_, framebuffer, nullptr);
        }
        swapchainFramebuffers_.clear();
    }

    void Vulkan_Renderer::UpdateUniformBuffers()
    {
        VertexShaderUniformBufferObject vsubo = { viewMatrix_, projectionMatrix_ };
        void* data1;
        _VK_CHECK(vkMapMemory(logicalDevice_, vertexShaderUniformBufferMemory_, 0, sizeof(vsubo), 0, &data1));
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
        _VK_CHECK(vkMapMemory(logicalDevice_, fragmentShaderUniformBufferMemory_, 0, sizeof(fsubo), 0, &data2));
        memcpy(data2, &fsubo, sizeof(fsubo));
        vkUnmapMemory(logicalDevice_, fragmentShaderUniformBufferMemory_);
    }

    void Vulkan_Renderer::AcquireNextImage()
    {
        if (swapchain_ != VK_NULL_HANDLE)
        {
            VkResult result = vkAcquireNextImageKHR(logicalDevice_, swapchain_, UINT64_MAX, imageAvailableSemaphore_, VK_NULL_HANDLE, &currentSwapchainFramebufferIndex_);

            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                HandleSwapchainFramebufferResize();
                result = vkAcquireNextImageKHR(logicalDevice_, swapchain_, UINT64_MAX, imageAvailableSemaphore_, VK_NULL_HANDLE, &currentSwapchainFramebufferIndex_);
            }

            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            {
                LOG_ERROR_F("Vulkan Error: Failed to acquire swap chain image!");
            }

            _VK_CHECK(vkQueueWaitIdle(graphicsQueue_));

            ResetCommandBuffers();
            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

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
                commandBuffer,
                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier
            );

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &imageAvailableSemaphore_;
            submitInfo.pWaitDstStageMask = waitStages;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE));
        }
    }

    VkCommandBuffer Vulkan_Renderer::GetNextCommandBuffer()
    {
        if (nextCommandBufferIndex_ >= commandBuffers_.size())
        {
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = commandPool_;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer newCommandBuffer;
            _VK_CHECK(vkAllocateCommandBuffers(logicalDevice_, &allocInfo, &newCommandBuffer));
            commandBuffers_.push_back(newCommandBuffer);
        }
        nextCommandBufferIndex_++;
        return commandBuffers_[nextCommandBufferIndex_ - 1];
    }

    void Vulkan_Renderer::ResetCommandBuffers()
    {
        for (size_t i = 0; i < commandBuffers_.size(); ++i)
        {
            _VK_CHECK(vkResetCommandBuffer(commandBuffers_[i], 0));
        }
        nextCommandBufferIndex_ = 0;
    }

    VkSemaphore Vulkan_Renderer::GetCurrentSemaphore()
    {
        if (semaphores_.empty())
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkSemaphore newSemaphore;
            _VK_CHECK(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &newSemaphore));
            semaphores_.push_back(newSemaphore);
        }

        return semaphores_[currentSemaphoreIndex_];
    }

    VkSemaphore Vulkan_Renderer::GetNextSemaphore()
    {
        if (currentSemaphoreIndex_ >= semaphores_.size())
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkSemaphore newSemaphore;
            _VK_CHECK(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &newSemaphore));
            semaphores_.push_back(newSemaphore);
        }

        currentSemaphoreIndex_++;
        return semaphores_[currentSemaphoreIndex_];
    }

    void Vulkan_Renderer::ResetSemaphores()
    {
        currentSemaphoreIndex_ = 0;
    }

    void Vulkan_Renderer::RenderMeshToTexture(Vulkan_Mesh& mesh)
    {
        if (framebufferResized_)
        {
            HandleFramebufferResize();
            framebufferResized_ = false;
        }

        if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0 &&
            instanceCount_ > 0)
        {
            _VK_CHECK(vkResetFences(logicalDevice_, 1, &instanceDataTransferFence_));

            VkCommandBuffer commandBuffer = GetNextCommandBuffer();

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

            VkBufferMemoryBarrier instanceBufferUpdateBarrier = {};
            instanceBufferUpdateBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            instanceBufferUpdateBarrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            instanceBufferUpdateBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            instanceBufferUpdateBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            instanceBufferUpdateBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            instanceBufferUpdateBarrier.buffer = instanceBuffer_;
            instanceBufferUpdateBarrier.offset = 0;
            instanceBufferUpdateBarrier.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr,
                1, &instanceBufferUpdateBarrier,
                0, nullptr
            );

            VkBufferCopy instanceCopyRegion = {};
            instanceCopyRegion.size = sizeof(InstanceData) * instanceCount_;
            vkCmdCopyBuffer(commandBuffer, instanceStagingBuffer_, instanceBuffer_, 1, &instanceCopyRegion);

            VkBufferMemoryBarrier instanceBufferUpdateBarrier2 = {};
            instanceBufferUpdateBarrier2.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            instanceBufferUpdateBarrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            instanceBufferUpdateBarrier2.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
            instanceBufferUpdateBarrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            instanceBufferUpdateBarrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            instanceBufferUpdateBarrier2.buffer = instanceBuffer_;
            instanceBufferUpdateBarrier2.offset = 0;
            instanceBufferUpdateBarrier2.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                0,
                0, nullptr,
                1, &instanceBufferUpdateBarrier2,
                0, nullptr
            );

            _VK_CHECK(vkEndCommandBuffer(commandBuffer));

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, instanceDataTransferFence_));

            // ---------------------------------------------------------------------

            _VK_CHECK(vkWaitForFences(logicalDevice_, 1, &renderingFence_, VK_TRUE, UINT64_MAX));
            _VK_CHECK(vkResetFences(logicalDevice_, 1, &renderingFence_));

            ApplyTextureBindings();
            IncreaseTectureUnitStaleness();
            UpdateUniformBuffers();

            _VK_CHECK(vkResetCommandBuffer(renderingCommandBuffer_, 0));

            VkCommandBufferBeginInfo beginInfo2 = {};
            beginInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            _VK_CHECK(vkBeginCommandBuffer(renderingCommandBuffer_, &beginInfo2));

            VkMemoryBarrier renderingMemoryBarrier = {};
            renderingMemoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            renderingMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            renderingMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            vkCmdPipelineBarrier(
                renderingCommandBuffer_,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                0,
                1, &renderingMemoryBarrier,
                0, nullptr,
                0, nullptr
            );

            VkRenderPass currentRenderPass;
            VkFramebuffer currentFrameBuffer;
            VkPipeline currentGraphicsPipeline;
            if (multisampleAntiAliasing_)
            {
                if (depthTesting_)
                {
                    currentRenderPass = primaryRenderPass4_;
                    currentFrameBuffer = primaryFrameBuffer4_;
                    currentGraphicsPipeline = primaryGraphicsPipeline8_;
                }
                else
                {
                    currentRenderPass = primaryRenderPass3_;
                    currentFrameBuffer = primaryFrameBuffer3_;
                    currentGraphicsPipeline = primaryGraphicsPipeline7_;
                }
            }
            else
            {
                if (depthTesting_)
                {
                    currentRenderPass = primaryRenderPass2_;
                    currentFrameBuffer = primaryFrameBuffer2_;
                    currentGraphicsPipeline = primaryGraphicsPipeline6_;
                }
                else
                {
                    currentRenderPass = primaryRenderPass1_;
                    currentFrameBuffer = primaryFrameBuffer1_;
                    currentGraphicsPipeline = primaryGraphicsPipeline5_;
                }
            }

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = currentRenderPass;
            renderPassInfo.framebuffer = currentFrameBuffer;
            renderPassInfo.renderArea.offset = { (int32_t)(frameBufferWidth_ * cameraViewportX_), (int32_t)(frameBufferHeight_ * cameraViewportY_) };
            renderPassInfo.renderArea.extent = { (uint32_t)(frameBufferWidth_ * cameraViewportWidth_), (uint32_t)(frameBufferHeight_ * cameraViewportHeight_) };

            vkCmdBeginRenderPass(renderingCommandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(renderingCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, currentGraphicsPipeline);

            // viewport.y is non zero and viewport.height is negative to flip y axis
            // (making it behave like it does in OpenGL)
            // If VK_API_VERSION_1_1 < 1.1: requires VK_KHR_MAINTENANCE_1_EXTENSION_NAME
            VkViewport viewport = {};
            viewport.x = (float)frameBufferWidth_ * cameraViewportX_;
            viewport.y = (float)frameBufferHeight_ - (float)frameBufferHeight_ * cameraViewportY_;
            viewport.width = (float)frameBufferWidth_ * cameraViewportWidth_;
            viewport.height = -(float)frameBufferHeight_ * cameraViewportHeight_;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(renderingCommandBuffer_, 0, 1, &viewport);

            VkRect2D scissor = {};
            scissor.offset = { 0, 0 };
            scissor.extent = { frameBufferWidth_, frameBufferHeight_ };
            vkCmdSetScissor(renderingCommandBuffer_, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = { mesh.vertexBuffer_, instanceBuffer_ };
            VkDeviceSize offsets[] = { 0, 0 };
            vkCmdBindVertexBuffers(renderingCommandBuffer_, 0, 2, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(renderingCommandBuffer_, mesh.indexBuffer_, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(renderingCommandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, primaryPipelineLayout_, 0, 1, &primaryDescriptorSet_, 0, nullptr);

            vkCmdDrawIndexed(renderingCommandBuffer_, (uint32_t)mesh.indexCount_, (uint32_t)instanceCount_, 0, 0, 0);

            vkCmdEndRenderPass(renderingCommandBuffer_);

            _VK_CHECK(vkEndCommandBuffer(renderingCommandBuffer_));

            VkSubmitInfo submitInfo2 = {};
            submitInfo2.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo2.commandBufferCount = 1;
            submitInfo2.pCommandBuffers = &renderingCommandBuffer_;

            _VK_CHECK(vkQueueSubmit(graphicsQueue_, 1, &submitInfo2, renderingFence_));

            instanceCount_ = 0;

            justRenderedToTexture_ = true;
        }
    }

    void Vulkan_Renderer::HandleFramebufferResize()
    {
        vkDeviceWaitIdle(logicalDevice_);

        frameBufferWidth_ = pendingFrameBufferWidth_;
        frameBufferHeight_ = pendingFrameBufferHeight_;

        DeletePrimaryFrameBuffers();
        DeletePrimaryFrameBufferAttachments();

        if (frameBufferWidth_ > 0 && frameBufferHeight_ > 0)
        {
            CreatePrimaryFrameBufferAttachments();
            CreatePrimaryFrameBuffers();
        }
    }

    void Vulkan_Renderer::HandleSwapchainFramebufferResize()
    {
        vkDeviceWaitIdle(logicalDevice_);

        DeleteSwapchainFramebuffers();
        DeleteSwapchain();

        UpdateSurfaceCapabilities();

        if (surfaceExtent_.width > 0 && surfaceExtent_.height > 0)
        {
            CreateSwapchain();
            CreateSwapchainFramebuffers();
        }
    }

    void Vulkan_Renderer::UpdateSurfaceCapabilities()
    {
        _VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice_, surface_, &surfaceCapabilities_));

        if (surfaceCapabilities_.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            surfaceExtent_ = surfaceCapabilities_.currentExtent;
        }
        else
        {
            // The window manager does not have a preferred size for the
            // surface. The application must choose an appropriate size.

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
    }

    void Vulkan_Renderer::CreateImageView(
        VkImage image,
        VkFormat format,
        VkImageAspectFlags imageAspectFlags,
        uint32_t mipLevels,
        VkImageView& imageView)
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

        _VK_CHECK(vkCreateImageView(logicalDevice_, &imageViewCreateInfo, nullptr, &imageView));

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

    void Vulkan_Renderer::CreateBuffer(
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

        LOG_ERROR_F("Vulkan Error: Failed to find suitable memory type!");

        return (uint32_t)-1;
    }

    void Vulkan_Renderer::CreateMesh(
        const MeshVertex* meshVertexPtr,
        unsigned int meshVertexCount,
        const unsigned int* meshIndexPtr,
        unsigned int meshIndexCount,
        Vulkan_Mesh& mesh)
    {
        mesh.vertexCount_ = meshVertexCount;
        VkDeviceSize vertexBufferSize = sizeof(VertexData) * meshVertexCount;
        VkBuffer vertexStagingBuffer;
        VkDeviceMemory vertexStagingBufferMemory;
        CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexStagingBuffer, vertexStagingBufferMemory);

        void* vertexStagingBufferDataPtr;
        _VK_CHECK(vkMapMemory(logicalDevice_, vertexStagingBufferMemory, 0, vertexBufferSize, 0, &vertexStagingBufferDataPtr));
        memcpy(vertexStagingBufferDataPtr, meshVertexPtr, vertexBufferSize);

        CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mesh.vertexBuffer_, mesh.vertexBufferMemory_);

        VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy vertexCopyRegion = {};
        vertexCopyRegion.size = vertexBufferSize;
        vkCmdCopyBuffer(commandBuffer, vertexStagingBuffer, mesh.vertexBuffer_, 1, &vertexCopyRegion);

        EndSingleTimeCommands(commandBuffer);

        vkUnmapMemory(logicalDevice_, vertexStagingBufferMemory);
        vkDestroyBuffer(logicalDevice_, vertexStagingBuffer, nullptr);
        vkFreeMemory(logicalDevice_, vertexStagingBufferMemory, nullptr);

        mesh.indexCount_ = meshIndexCount;
        VkDeviceSize indexBufferSize = sizeof(unsigned int) * meshIndexCount;
        VkBuffer indexStagingBuffer;
        VkDeviceMemory indexStagingBufferMemory;
        CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indexStagingBuffer, indexStagingBufferMemory);

        void* indexStagingBufferDataPtr;
        _VK_CHECK(vkMapMemory(logicalDevice_, indexStagingBufferMemory, 0, indexBufferSize, 0, &indexStagingBufferDataPtr));
        memcpy(indexStagingBufferDataPtr, meshIndexPtr, indexBufferSize);

        CreateBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mesh.indexBuffer_, mesh.indexBufferMemory_);

        commandBuffer = BeginSingleTimeCommands();

        VkBufferCopy indexCopyRegion = {};
        indexCopyRegion.size = indexBufferSize;
        vkCmdCopyBuffer(commandBuffer, indexStagingBuffer, mesh.indexBuffer_, 1, &indexCopyRegion);

        EndSingleTimeCommands(commandBuffer);

        vkUnmapMemory(logicalDevice_, indexStagingBufferMemory);
        vkDestroyBuffer(logicalDevice_, indexStagingBuffer, nullptr);
        vkFreeMemory(logicalDevice_, indexStagingBufferMemory, nullptr);
    }

    void Vulkan_Renderer::DeleteMesh(Vulkan_Mesh& mesh)
    {
        vkDestroyBuffer(logicalDevice_, mesh.vertexBuffer_, nullptr);
        mesh.vertexBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, mesh.vertexBufferMemory_, nullptr);
        mesh.vertexBufferMemory_ = VK_NULL_HANDLE;
        vkDestroyBuffer(logicalDevice_, mesh.indexBuffer_, nullptr);
        mesh.indexBuffer_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, mesh.indexBufferMemory_, nullptr);
        mesh.indexBufferMemory_ = VK_NULL_HANDLE;
    }

    void Vulkan_Renderer::CreateTexture(
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
            mipLevels = CalculateMipLevels(width, height);
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
            VK_IMAGE_ASPECT_COLOR_BIT,
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

        CreateImageView(
            texture.textureImage_,
            s_textureFormat,
            VK_IMAGE_ASPECT_COLOR_BIT,
            mipLevels,
            texture.textureImageView_
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

    void Vulkan_Renderer::DeleteTexture(Vulkan_Texture& texture)
    {
        vkDestroySampler(logicalDevice_, texture.textureSampler_, nullptr);
        texture.textureSampler_ = VK_NULL_HANDLE;
        vkDestroyImageView(logicalDevice_, texture.textureImageView_, nullptr);
        texture.textureImageView_ = VK_NULL_HANDLE;
        vkDestroyImage(logicalDevice_, texture.textureImage_, nullptr);
        texture.textureImage_ = VK_NULL_HANDLE;
        vkFreeMemory(logicalDevice_, texture.textureImageMemory_, nullptr);
        texture.textureImageMemory_ = VK_NULL_HANDLE;
    }

    int Vulkan_Renderer::GetTextureUnit(unsigned int textureId, float& textureUnit)
    {
        UniqueBiMap<unsigned int, unsigned int>::iterator iter = textureIdToUnitBiMap_.FindKey(textureId);
        if (iter != textureIdToUnitBiMap_.IteratorPastTheEnd())
        {
            unsigned int textureUnit_uint = iter->second;
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

    bool Vulkan_Renderer::GetStalestTextureUnit(unsigned int& textureUnit) const
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

    void Vulkan_Renderer::IncreaseTectureUnitStaleness()
    {
        for (size_t i = 0; i < NUMBER_OF_TEXTURE_UNITS; ++i)
        {
            textureUnitStalenessValues_[i]++;
        }
    }

    bool Vulkan_Renderer::BindTexture(
        unsigned int textureId,
        unsigned int textureUnit)
    {
        if (textureUnit < NUMBER_OF_TEXTURE_UNITS)
        {
            AutoIdMap<Vulkan_Texture>::iterator iter = textureMap_.Find(textureId);
            if (iter != textureMap_.IteratorPastTheEnd())
            {
                textureIdToUnitBiMap_.Add(textureId, textureUnit);
                return true;
            }
        }

        return false;
    }

    void Vulkan_Renderer::ApplyTextureBindings()
    {
        std::array<VkDescriptorImageInfo, NUMBER_OF_TEXTURE_UNITS> imageInfos = {};
        std::array<VkWriteDescriptorSet, NUMBER_OF_TEXTURE_UNITS> descriptorWrites = {};

        for (unsigned int i = 0; i < NUMBER_OF_TEXTURE_UNITS; i++)
        {
            Vulkan_Texture* currentTexturePtr;
            if (textureIdToUnitBiMap_.FindValue(i) != textureIdToUnitBiMap_.IteratorPastTheEnd())
            {
                const unsigned int& textureId = textureIdToUnitBiMap_.FindValue(i)->first;
                currentTexturePtr = &textureMap_.Find(textureId)->second;
            }
            else
            {
                currentTexturePtr = &defaultTexture_;
            }

            imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfos[i].imageView = currentTexturePtr->textureImageView_;
            imageInfos[i].sampler = currentTexturePtr->textureSampler_;

            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = primaryDescriptorSet_;
            descriptorWrites[i].dstBinding = 2;
            descriptorWrites[i].dstArrayElement = (uint32_t)i;
            descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].pImageInfo = &imageInfos[i];
        }

        vkUpdateDescriptorSets(logicalDevice_, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

    void Vulkan_Renderer::ApplyScreenTextureBinding()
    {
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = screenTexture_.textureImageView_;
        imageInfo.sampler = screenTexture_.textureSampler_;

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = secondaryDescriptorSet_;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(logicalDevice_, 1, &descriptorWrite, 0, nullptr);
    }

    void Vulkan_Renderer::TransitionImageLayout(
        VkImage image,
        VkImageLayout oldLayout,
        VkImageLayout newLayout,
        VkImageAspectFlags aspectMask,
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
        imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = mipLevels;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_NONE;
        VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_NONE;

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
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }
        else
        {
            LOG_ERROR_F("Vulkan Error: Unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage,
            destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier
        );

        EndSingleTimeCommands(commandBuffer);
    }

    void Vulkan_Renderer::CopyBufferToImage(
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

    void Vulkan_Renderer::GenerateMipmaps(
        VkImage image,
        int32_t texWidth,
        int32_t texHeight,
        uint32_t mipLevels)
    {
        if (!(formatProperties_.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            LOG_ERROR_F("Vulkan Error: Texture image format does not support linear blitting!");
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

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier
            );

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

            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier
            );

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        imageMemoryBarrier.subresourceRange.baseMipLevel = mipLevels - 1;
        imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(
            commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier
        );

        EndSingleTimeCommands(commandBuffer);
    }

    VkCommandBuffer Vulkan_Renderer::BeginSingleTimeCommands()
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

    void Vulkan_Renderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
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

    VkShaderModule Vulkan_Renderer::CreateShaderModule(
        const unsigned char* data,
        size_t size)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = size;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(data);

        VkShaderModule shaderModule;
        _VK_CHECK(vkCreateShaderModule(logicalDevice_, &createInfo, nullptr, &shaderModule));

        return shaderModule;
    }

    VkRenderPass Vulkan_Renderer::CreatePrimaryRenderPass(
        bool msaa,
        bool depthTesting)
    {
        VkSampleCountFlagBits sampleCount = msaa ? msaaSampleCount_ : VK_SAMPLE_COUNT_1_BIT;

        size_t attachmentCount = 1;
        if (depthTesting) attachmentCount++;
        if (msaa) attachmentCount++;

        std::vector<VkAttachmentDescription> attachmentDescriptions(attachmentCount);
        memset(&attachmentDescriptions[0], 0, sizeof(VkAttachmentDescription) * attachmentDescriptions.size());

        std::vector<VkAttachmentReference> attachmentReferences(attachmentCount);
        memset(&attachmentReferences[0], 0, sizeof(VkAttachmentReference) * attachmentReferences.size());

        uint32_t nextAttachmentIndex = 0;

        VkAttachmentDescription& colorAttachmentDescription = attachmentDescriptions[nextAttachmentIndex];
        colorAttachmentDescription.format = s_textureFormat;
        colorAttachmentDescription.samples = sampleCount;
        colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference& colorAttachmentReference = attachmentReferences[nextAttachmentIndex];
        colorAttachmentReference.attachment = nextAttachmentIndex;
        colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        nextAttachmentIndex++;

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

        if (depthTesting)
        {
            VkAttachmentDescription& depthAttachmentDescription = attachmentDescriptions[nextAttachmentIndex];
            depthAttachmentDescription = {};
            depthAttachmentDescription.format = depthFormat_;
            depthAttachmentDescription.samples = sampleCount;
            depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // VK_ATTACHMENT_STORE_OP_DONT_CARE
            depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference& depthAttachmentReference = attachmentReferences[nextAttachmentIndex];
            depthAttachmentReference.attachment = nextAttachmentIndex;
            depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            nextAttachmentIndex++;

            subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

            subpssDependency.srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpssDependency.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpssDependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        }

        if (msaa)
        {
            VkAttachmentDescription& colorResolveAttachmentDescription = attachmentDescriptions[nextAttachmentIndex];
            colorResolveAttachmentDescription.format = s_textureFormat;
            colorResolveAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
            colorResolveAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorResolveAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorResolveAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorResolveAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorResolveAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorResolveAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

            VkAttachmentReference& colorAttachmentResolveReference = attachmentReferences[nextAttachmentIndex];
            colorAttachmentResolveReference.attachment = nextAttachmentIndex;
            colorAttachmentResolveReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            nextAttachmentIndex++;

            subpassDescription.pResolveAttachments = &colorAttachmentResolveReference;
        }

        VkRenderPassCreateInfo renderPassCreateInfo = {};
        renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreateInfo.attachmentCount = (uint32_t)attachmentDescriptions.size();
        renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpassDescription;
        renderPassCreateInfo.dependencyCount = 1;
        renderPassCreateInfo.pDependencies = &subpssDependency;

        VkRenderPass renderPass;
        _VK_CHECK(vkCreateRenderPass(logicalDevice_, &renderPassCreateInfo, nullptr, &renderPass));
        return renderPass;
    }

    VkPipeline Vulkan_Renderer::CreatePrimaryGraphicsPipeline(
        VkRenderPass& renderPass,
        const std::vector<VkVertexInputBindingDescription>& vertexInputBindingDescriptions,
        const std::vector<VkVertexInputAttributeDescription>& vertexInputAttributeDescriptions,
        bool msaa,
        bool depthTesting)
    {
        VkSampleCountFlagBits sampleCount = msaa ? msaaSampleCount_ : VK_SAMPLE_COUNT_1_BIT;

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

        vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)vertexInputBindingDescriptions.size();
        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)vertexInputAttributeDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();
        vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();

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
        multisampling.rasterizationSamples = sampleCount;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
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
        pipelineInfo.layout = primaryPipelineLayout_;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkPipelineDepthStencilStateCreateInfo depthStencil;
        if (depthTesting)
        {
            depthStencil = {};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = VK_TRUE;
            depthStencil.depthWriteEnable = VK_TRUE;
            depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencil.depthBoundsTestEnable = VK_FALSE;
            depthStencil.stencilTestEnable = VK_FALSE;

            pipelineInfo.pDepthStencilState = &depthStencil;
        }

        VkPipeline graphicsPipeline;
        _VK_CHECK(vkCreateGraphicsPipelines(logicalDevice_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));
        return graphicsPipeline;
    }

#ifndef NDEBUG
    const bool Vulkan_Renderer::s_enableValidationLayers_ = true;
    const std::vector<const char*> Vulkan_Renderer::s_validationLayers_ =
    {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    const bool Vulkan_Renderer::s_enableValidationLayers_ = false;
    const std::vector<const char*> Vulkan_Renderer::s_validationLayers_ = {};
#endif

    const std::vector<const char*> Vulkan_Renderer::s_deviceExtensions_ =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const bool Vulkan_Renderer::s_requireSamplerAnisotropy_ = true;

    const VkFormat Vulkan_Renderer::s_desiredSurfaceFormat = VK_FORMAT_B8G8R8A8_UNORM;
    const VkFormat Vulkan_Renderer::s_textureFormat = VK_FORMAT_R8G8B8A8_UNORM;
}