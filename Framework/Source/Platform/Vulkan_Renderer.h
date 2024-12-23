// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "AutoIdMap.h"
#include "RenderData.h"
#include "Renderer.h"
#include "UniqueBiMap.h"

#include "vulkan/vulkan.h"



namespace Project001
{
    class Vulkan_Renderer : public Renderer
    {
    public:
        Vulkan_Renderer(const RendererInfo& rendererInfo);

        ~Vulkan_Renderer() override;

        bool GetDepthTesting() const override;
        void SetDepthTesting(bool depthTesting) override;

        void SetMultisampleAntiAliasing(bool multisampleAntiAliasing) override;

        void SetInstanceBufferCapacity(unsigned int capacity) override;

        void SetBatchedIndexBufferCapacity(unsigned int capacity) override;
        void SetBatchedVertexBufferCapacity(unsigned int capacity) override;

        void GetFramebufferSize(
            unsigned int& width,
            unsigned int& height) const override;

        void SetFramebufferSize(
            unsigned int width,
            unsigned int height) override;

        // viewport origin is the bottom left corner
        void GetViewport(
            unsigned int& x,
            unsigned int& y,
            unsigned int& width,
            unsigned int& height) override;

        void SetViewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) override;

        void GetCameraViewport(
            float& x,
            float& y,
            float& width,
            float& height) override;

        void SetCameraViewport(
            float x,
            float y,
            float width,
            float height) override;

        // Border color for the borders when the framebuffer's aspect ratio is
        // different from the window's.
        // The alpha value of the border color sets the windows overall
        // transparency.
        void SetBorderColor(const glm::vec4& color) override;

        void SetClearColor(const glm::vec4& color) override;

        void CreateTexture(
            unsigned int& textureId,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool multisampleAntiAliasing,
            bool mipMaps) override;

        bool DeleteTexture(unsigned int textureId) override;

        void DeleteAllTextures() override;

        void SetViewMatrix(const glm::mat4& viewMatrix) override;

        void SetViewPosition(const glm::vec3& viewPosition) override;

        void SetProjectionMatrix(const glm::mat4& projectionMatrix) override;

        void SetDirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override;

        void AddPointLight(
            const glm::vec3& position,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override;

        void AddSpotLight(
            const glm::vec3& position,
            const glm::vec3& direction,
            float cutoff,
            float outerCutoff,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override;

        void ClearDirectionalLight() override;

        void ClearPointLights() override;

        void ClearSpotLights() override;

        void BeginRendering() override;

        void Clear() override;

        void ClearDepthOnly() override;

        void CreateMesh(
            unsigned int& meshId,
            const MeshVertex* meshVertexPtr,
            unsigned int meshVertexCount,
            const unsigned int* meshIndexPtr,
            unsigned int meshIndexCount) override;

        bool DeleteMesh(unsigned int meshId) override;

        void DeleteAllMeshes() override;

        bool RenderMesh(
            unsigned int meshId,
            const MeshInstanceData* meshInstanceDataPtr,
            unsigned int meshInstanceCount) override;

        bool AddMeshToBatch(
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
            bool lit) override;

        void RenderBatch() override;

        void FinishRendering() override;

        void SwapBuffers() override;

    protected:
        struct Vulkan_Mesh
        {
            size_t indexCount_;
            VkBuffer indexBuffer_;
            VkDeviceMemory indexBufferMemory_;

            size_t vertexCount_;
            VkBuffer vertexBuffer_;
            VkDeviceMemory vertexBufferMemory_;

            bool Initialized()
            {
                return indexBuffer_ != VK_NULL_HANDLE &&
                    indexBufferMemory_ != VK_NULL_HANDLE &&
                    vertexBuffer_ != VK_NULL_HANDLE &&
                    vertexBufferMemory_ != VK_NULL_HANDLE;
            }
        };

        struct Vulkan_Texture
        {
            VkImage textureImage_;
            VkDeviceMemory textureImageMemory_;
            VkImageView textureImageView_;
            VkSampler textureSampler_;

            bool Initialized()
            {
                return textureImage_ != VK_NULL_HANDLE &&
                    textureImageMemory_ != VK_NULL_HANDLE &&
                    textureImageView_ != VK_NULL_HANDLE &&
                    textureSampler_ != VK_NULL_HANDLE;
            }
        };

        // Creates:
        //     vulkanInstance_
        //     debugMessenger_
        void CreateVulkanInstance();
        void DeleteVulkanInstance();

        // Requires:
        //     vulkanInstance_
        // Creates:
        //     surface_
        void CreateSurface();
        void DeleteSurface();

        // Requires:
        //     vulkanInstance_
        //     surface_
        // Gets:
        //     physicalDevice_
        // Sets:
        //     graphicsQueueFamilyIndex_
        //     presentQueueFamilyIndex_
        //     surfaceCapabilities_
        //     surfaceExtent_
        //     surfaceFormat_
        //     formatProperties_
        //     depthFormat_
        //     surfacePresentMode_
        //     physicalDeviceProperties_
        //     msaaSampleCount_
        void PickPhysicalDevice();
        void UnPickPhysicalDevice();

        // Requires:
        //     physicalDevice_
        //     graphicsQueueFamilyIndex_
        //     presentQueueFamilyIndex_
        // Creates:
        //     logicalDevice_
        // Gets:
        //     graphicsQueue_
        //     presentQueue_
        void CreateLogicalDevice();
        void DeleteLogicalDevice();

        // Requires:
        //     logicalDevice_
        //     graphicsQueueFamilyIndex_
        // Creates:
        //     commandPool_
        void CreateCommandPool();
        void DeleteCommandPool();

        // Requires:
        //     logicalDevice_
        //     commandPool_
        // Allocates:
        //     renderingCommandBuffer_
        void CreateCommandBuffers();

        // Requires:
        //     logicalDevice_
        // Creates:
        //     renderingFence_
        //     batchedDataTransferFence_
        //     instanceDataTransferFence_
        //     imageAvailableSemaphore_
        //     readyToPresentSemaphore_
        void CreateSyncObjects();
        void DeleteSyncObjects();

        // Requires:
        //     logicalDevice_
        //     surface_
        //     graphicsQueueFamilyIndex_
        //     presentQueueFamilyIndex_
        //     surfaceCapabilities_
        //     surfaceExtent_
        //     surfaceFormat_
        //     surfacePresentMode_
        // Creates:
        //     swapchain_
        //     swapchainImageViews_
        // Sets:
        //     swapchainImageCount_
        //     swapchainImages_
        void CreateSwapchain();
        void DeleteSwapchain();

        // Requires:
        //     physicalDevice_
        //     logicalDevice_
        // Creates:
        //     instanceStagingBuffer_
        //     instanceBuffer_
        // Allocates:
        //     instanceStagingBufferMemory_
        //     instanceBufferMemory_
        // Sets:
        //     instanceStagingBufferDataPtr_
        //     instanceCount_
        void CreateInstanceDataBuffers();
        void DeleteInstanceDataBuffers();

        // Requires:
        //     physicalDevice_
        //     logicalDevice_
        // Creates:
        //     batchedIndexStagingBuffer_
        //     batchedIndexBuffer_
        // Allocates:
        //     batchedIndexStagingBufferMemory_
        //     batchedIndexBufferMemory_
        // Sets:
        //     batchedIndexStagingBufferDataPtr_
        //     batchedIndexCount_
        void CreateBatchedIndexDataBuffers();
        void DeleteBatchedIndexDataBuffers();

        // Requires:
        //     physicalDevice_
        //     logicalDevice_
        // Creates:
        //     batchedVertexStagingBuffer_
        //     batchedVertexBuffer_
        // Allocates:
        //     batchedVertexStagingBufferMemory_
        //     batchedVertexBufferMemory_
        // Sets:
        //     batchedVertexStagingBufferDataPtr_
        //     batchedVertexCount_
        void CreateBatchedVertexDataBuffers();
        void DeleteBatchedVertexDataBuffers();

        // Requires:
        //     physicalDevice_
        //     logicalDevice_
        //     commandPool_
        // Creates:
        //     vertexShaderUniformBuffer_
        //     fragmentShaderUniformBuffer_
        //     screenVertexBuffer_
        // Allocates:
        //     vertexShaderUniformBufferMemory_
        //     fragmentShaderUniformBufferMemory_
        //     screenVertexBufferMemory_
        void CreateOtherDataBuffers();
        void DeleteOtherDataBuffers();

        // Requires:
        //     physicalDevice_
        //     logicalDevice_
        //     graphicsQueue_
        //     commandPool_
        //     commandBuffer_
        // Creates:
        //     defaultTexture_
        void CreateDefaultTexture();
        void DeleteDefaultTexture();

        // Requires:
        //     logicalDevice_
        // Creates:
        //     secondaryDescriptorSetLayout_
        //     primaryDescriptorSetLayout_
        void CreateDescriptorSetLayouts();
        void DeleteDescriptorSetLayouts();

        // Requires:
        //     logicalDevice_
        // Creates:
        //     descriptorPool_
        void CreateDescriptorPool();
        void DeleteDescriptorPool();

        // Requires:
        //     logicalDevice_
        //     descriptorPool_
        //     primaryDescriptorSetLayout_
        //     secondaryDescriptorSetLayout_
        //     vertexShaderUniformBuffer_
        //     fragmentShaderUniformBuffer_
        //     defaultTexture_
        // Creates:
        //     primaryDescriptorSet_
        //     secondaryDescriptorSet_
        void CreateDescriptorSets();

        // Requires:
        //     logicalDevice_
        //     primaryDescriptorSetLayout_
        //     secondaryDescriptorSetLayout_
        // Creates:
        //     primaryPipelineLayout_
        //     secondaryPipelineLayout_
        void CreatePipelineLayouts();
        void DeletePipelineLayouts();

        // Requires:
        //     logicalDevice_
        // Creates:
        //     vertexBatchShaderModule_
        //     fragmentBatchShaderModule_
        void CreateBatchShaderModules();
        void DeleteBatchShaderModules();

        // Requires:
        //     logicalDevice_
        //     depthFormat_
        //     msaaSampleCount_
        // Creates:
        //     primaryRenderPass1_
        //     primaryRenderPass2_
        //     primaryRenderPass3_
        //     primaryRenderPass4_
        void CreatePrimaryRenderPasses();
        void DeletePrimaryRenderPasses();

        // Requires:
        //     logicalDevice_
        //     msaaSampleCount_
        //     primaryPipelineLayout_
        //     vertexBatchShaderModule_
        //     fragmentBatchShaderModule_
        //     primaryRenderPass1_
        //     primaryRenderPass2_
        //     primaryRenderPass3_
        //     primaryRenderPass4_
        // Creates:
        //     primaryGraphicsPipeline1_
        //     primaryGraphicsPipeline2_
        //     primaryGraphicsPipeline3_
        //     primaryGraphicsPipeline4_
        void CreatePrimaryGraphicsPipelines();
        void DeletePrimaryGraphicsPipelines();

        // Requires:
        //     frameBufferWidth_
        //     frameBufferHeight_
        //     physicalDevice_
        //     depthFormat_
        //     msaaSampleCount_
        //     logicalDevice_
        //     graphicsQueue_
        //     commandPool_
        //     commandBuffer_
        // Creates:
        //     screenTexture_
        //     depthImage_
        //     depthImageView_
        //     msaaImage_
        //     msaaImageView_
        //     msaaDepthImage_
        //     msaaDepthImageView_
        // Allocates:
        //     depthImageMemory_
        //     msaaImageMemory_
        //     msaaDepthImageMemory_
        void CreatePrimaryFrameBufferAttachments();
        void DeletePrimaryFrameBufferAttachments();

        // Requires:
        //     frameBufferWidth_
        //     frameBufferHeight_
        //     logicalDevice_
        //     primaryRenderPass1_
        //     primaryRenderPass2_
        //     primaryRenderPass3_
        //     primaryRenderPass4_
        //     screenTexture_
        //     depthImageView_
        //     msaaImageView_
        //     msaaDepthImageView_
        // Creates:
        //     primaryFrameBuffer1_
        //     primaryFrameBuffer2_
        //     primaryFrameBuffer3_
        //     primaryFrameBuffer4_
        void CreatePrimaryFrameBuffers();
        void DeletePrimaryFrameBuffers();

        // Requires:
        //     logicalDevice_
        // Creates:
        //     vertexScreenShaderModule_
        //     fragmentScreenShaderModule_
        void CreateScreenShaderModules();
        void DeleteScreenShaderModules();

        // Requires:
        //     logicalDevice_
        //     surfaceFormat_
        // Creates:
        //     secondaryRenderPass_
        void CreateSecondaryRenderPass();
        void DeleteSecondaryRenderPass();

        // Requires:
        //     logicalDevice_
        //     secondaryPipelineLayout_
        //     vertexScreenShaderModule_
        //     fragmentScreenShaderModule_
        //     secondaryRenderPass_
        // Creates:
        //     secondaryGraphicsPipeline_
        void CreateSecondaryGraphicsPipeline();
        void DeleteSecondaryGraphicsPipeline();

        // Requires:
        //     logicalDevice_
        //     surfaceExtent_
        //     swapchainImageCount_
        //     swapchainImageViews_
        //     secondaryRenderPass_
        // Creates:
        //     swapchainFramebuffers_
        void CreateSwapchainFramebuffers();
        void DeleteSwapchainFramebuffers();

        // ---------------------------------------------------------------------

        // Used by:
        //     RenderBatch()
        //     RenderMeshToTexture(...)
        void UpdateUniformBuffers();

        // Used by:
        //     Vulkan_Renderer(...)
        //     SwapBuffers()
        void AcquireNextImage();

        // Used by:
        //     Clear()
        //     RenderBatch()
        //     FinishRendering()
        //     SwapBuffers()
        //     AcquireNextImage()
        //     RenderMeshToTexture(...)
        VkCommandBuffer GetNextCommandBuffer();

        // Used by:
        //     AcquireNextImage()
        void ResetCommandBuffers();

        // Unused
        VkSemaphore GetCurrentSemaphore();

        // Unused
        VkSemaphore GetNextSemaphore();

        // Unused
        void ResetSemaphores();

        // Used by:
        //     bool RenderMesh(...)
        void RenderMeshToTexture(Vulkan_Mesh& mesh);

        // Used by:
        //     Clear()
        //     RenderBatch()
        //     FinishRendering()
        void HandleFramebufferResize();

        // Used by:
        //     SwapBuffers()
        //     AcquireNextImage()
        void HandleSwapchainFramebufferResize();

        // Used by:
        //     SwapBuffers()
        //     PickPhysicalDevice()
        //     HandleSwapchainFramebufferResize()
        void UpdateSurfaceCapabilities();

        void CreateImageView(
            VkImage image,
            VkFormat format,
            VkImageAspectFlags imageAspectFlags,
            uint32_t mipLevels,
            VkImageView& imageView);

        void CreateImage(
            uint32_t width,
            uint32_t height,
            uint32_t mipLevels,
            VkSampleCountFlagBits sampleCountFlagBits,
            VkFormat format,
            VkImageTiling imageTiling,
            VkImageUsageFlags imageUsageFlags,
            VkMemoryPropertyFlags memoryPropertyFlags,
            VkImage& image,
            VkDeviceMemory& imageMemory);

        void CreateBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory);

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags memoryPropertyFlags);

        void CreateMesh(
            const MeshVertex* meshVertexPtr,
            unsigned int meshVertexCount,
            const unsigned int* meshIndexPtr,
            unsigned int meshIndexCount,
            Vulkan_Mesh& mesh);

        void DeleteMesh(Vulkan_Mesh& mesh);

        void CreateTexture(
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool multisampleAntiAliasing,
            bool mipMaps,
            Vulkan_Texture& texture);

        void DeleteTexture(Vulkan_Texture& texture);

        // returns 2 if texture failed to bind because textureId isn't found
        // returns 1 if texture failed to bind because all texture units are
        //           occupied
        // returns 0 if texture successfuly bound to unit
        int GetTextureUnit(unsigned int textureId, float& textureUnit);

        // texture units in use have a staleness value of 0. If all texture
        // units are in use, this returns false
        bool GetStalestTextureUnit(unsigned int& textureUnit) const;

        void IncreaseTectureUnitStaleness();

        bool BindTexture(
            unsigned int textureId,
            unsigned int textureUnit);

        void ApplyTextureBindings();

        void ApplyScreenTextureBinding();

        void TransitionImageLayout(
            VkImage image,
            VkImageLayout oldLayout,
            VkImageLayout newLayout,
            VkImageAspectFlags aspectMask,
            uint32_t mipLevels);

        void CopyBufferToImage(
            VkBuffer buffer,
            VkImage image,
            uint32_t width,
            uint32_t height);

        void GenerateMipmaps(
            VkImage image,
            int32_t texWidth,
            int32_t texHeight,
            uint32_t mipLevels);

        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

        VkShaderModule CreateShaderModule(
            const unsigned char* data,
            size_t size);

        VkRenderPass CreatePrimaryRenderPass(
            bool msaa,
            bool depthTesting);

        VkPipeline CreatePrimaryGraphicsPipeline(
            VkRenderPass& renderPass,
            const std::vector<VkVertexInputBindingDescription>& vertexInputBindingDescriptions,
            const std::vector<VkVertexInputAttributeDescription>& vertexInputAttributeDescriptions,
            bool msaa,
            bool depthTesting);

        static uint32_t CalculateMipLevels(unsigned int width, unsigned int height);

        static const bool s_enableValidationLayers_;
        static const std::vector<const char*> s_validationLayers_;

        static const std::vector<const char*> s_deviceExtensions_;

        static const bool s_requireSamplerAnisotropy_;

        static const VkFormat s_desiredSurfaceFormat;
        static const VkFormat s_textureFormat;

        Window* windowPtr_;
        unsigned int frameBufferWidth_;
        unsigned int frameBufferHeight_;
        unsigned int instanceBufferCapacity_;
        unsigned int batchedIndexBufferCapacity_;
        unsigned int batchedVertexBufferCapacity_;
        bool multisampleAntiAliasing_;
        bool depthTesting_;

        unsigned int viewportX_;
        unsigned int viewportY_;
        unsigned int viewportWidth_;
        unsigned int viewportHeight_;

        float cameraViewportX_;
        float cameraViewportY_;
        float cameraViewportWidth_;
        float cameraViewportHeight_;

        glm::vec4 borderColor_;
        glm::vec4 clearColor_;

        glm::mat4 viewMatrix_;
        glm::vec3 viewPosition_;
        glm::mat4 projectionMatrix_;

        DirectionalLight directionalLight_;
        std::vector<PointLight> pointLights_;
        std::vector<SpotLight> spotLights_;

        bool framebufferResized_;
        unsigned int pendingFrameBufferWidth_;
        unsigned int pendingFrameBufferHeight_;

        VkInstance vulkanInstance_;
        VkDebugUtilsMessengerEXT debugMessenger_;

        VkSurfaceKHR surface_;

        VkPhysicalDevice physicalDevice_;
        uint32_t graphicsQueueFamilyIndex_;
        uint32_t presentQueueFamilyIndex_;
        VkSurfaceCapabilitiesKHR surfaceCapabilities_;
        VkExtent2D surfaceExtent_;
        VkSurfaceFormatKHR surfaceFormat_;
        VkFormatProperties formatProperties_;
        VkFormat depthFormat_;
        VkPresentModeKHR surfacePresentMode_;
        VkPhysicalDeviceProperties physicalDeviceProperties_;
        VkSampleCountFlagBits msaaSampleCount_;

        VkDevice logicalDevice_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        VkCommandPool commandPool_;

        VkCommandBuffer renderingCommandBuffer_;

        std::vector<VkCommandBuffer> commandBuffers_;
        size_t nextCommandBufferIndex_;

        VkFence renderingFence_;
        bool justRenderedToBatch_;
        VkFence batchedDataTransferFence_;
        bool justRenderedToTexture_;
        VkFence instanceDataTransferFence_;
        VkSemaphore imageAvailableSemaphore_;
        VkSemaphore readyToPresentSemaphore_;

        std::vector<VkSemaphore> semaphores_; // Unused
        size_t currentSemaphoreIndex_;

        VkSwapchainKHR swapchain_;
        uint32_t swapchainImageCount_;
        std::vector<VkImage> swapchainImages_;
        std::vector<VkImageView> swapchainImageViews_;

        VkBuffer instanceStagingBuffer_;
        VkDeviceMemory instanceStagingBufferMemory_;
        InstanceData* instanceStagingBufferDataPtr_;
        size_t instanceCount_;
        VkBuffer instanceBuffer_;
        VkDeviceMemory instanceBufferMemory_;

        VkBuffer batchedIndexStagingBuffer_;
        VkDeviceMemory batchedIndexStagingBufferMemory_;
        uint32_t* batchedIndexStagingBufferDataPtr_;
        size_t batchedIndexCount_;
        VkBuffer batchedIndexBuffer_;
        VkDeviceMemory batchedIndexBufferMemory_;

        VkBuffer batchedVertexStagingBuffer_;
        VkDeviceMemory batchedVertexStagingBufferMemory_;
        BatchedVertexData* batchedVertexStagingBufferDataPtr_;
        size_t batchedVertexCount_;
        VkBuffer batchedVertexBuffer_;
        VkDeviceMemory batchedVertexBufferMemory_;

        VkBuffer vertexShaderUniformBuffer_;
        VkDeviceMemory vertexShaderUniformBufferMemory_;
        VkBuffer fragmentShaderUniformBuffer_;
        VkDeviceMemory fragmentShaderUniformBufferMemory_;
        VkBuffer screenVertexBuffer_;
        VkDeviceMemory screenVertexBufferMemory_;

        Vulkan_Texture defaultTexture_;

        VkDescriptorSetLayout primaryDescriptorSetLayout_;
        VkDescriptorSetLayout secondaryDescriptorSetLayout_;

        VkDescriptorPool descriptorPool_;

        VkDescriptorSet primaryDescriptorSet_;
        VkDescriptorSet secondaryDescriptorSet_;

        VkPipelineLayout primaryPipelineLayout_;
        VkPipelineLayout secondaryPipelineLayout_;

        VkShaderModule vertexBatchShaderModule_;
        VkShaderModule fragmentBatchShaderModule_;

        VkRenderPass primaryRenderPass1_; // No  MSAA && No  DepthTesting
        VkRenderPass primaryRenderPass2_; // No  MSAA && Yes DepthTesting
        VkRenderPass primaryRenderPass3_; // Yes MSAA && No  DepthTesting
        VkRenderPass primaryRenderPass4_; // Yes MSAA && Yes DepthTesting

        VkPipeline primaryGraphicsPipeline1_; // Batched   Render && No  MSAA && No  DepthTesting
        VkPipeline primaryGraphicsPipeline2_; // Batched   Render && No  MSAA && Yes DepthTesting
        VkPipeline primaryGraphicsPipeline3_; // Batched   Render && Yes MSAA && No  DepthTesting
        VkPipeline primaryGraphicsPipeline4_; // Batched   Render && Yes MSAA && Yes DepthTesting
        VkPipeline primaryGraphicsPipeline5_; // Instanced Render && No  MSAA && No  DepthTesting
        VkPipeline primaryGraphicsPipeline6_; // Instanced Render && No  MSAA && Yes DepthTesting
        VkPipeline primaryGraphicsPipeline7_; // Instanced Render && Yes MSAA && No  DepthTesting
        VkPipeline primaryGraphicsPipeline8_; // Instanced Render && Yes MSAA && Yes DepthTesting

        Vulkan_Texture screenTexture_;
        VkImage depthImage_;
        VkDeviceMemory depthImageMemory_;
        VkImageView depthImageView_;
        VkImage msaaImage_;
        VkDeviceMemory msaaImageMemory_;
        VkImageView msaaImageView_;
        VkImage msaaDepthImage_;
        VkDeviceMemory msaaDepthImageMemory_;
        VkImageView msaaDepthImageView_;

        VkFramebuffer primaryFrameBuffer1_;
        VkFramebuffer primaryFrameBuffer2_;
        VkFramebuffer primaryFrameBuffer3_;
        VkFramebuffer primaryFrameBuffer4_;

        VkShaderModule vertexScreenShaderModule_;
        VkShaderModule fragmentScreenShaderModule_;

        VkRenderPass secondaryRenderPass_;

        std::vector<VkFramebuffer> swapchainFramebuffers_;

        VkPipeline secondaryGraphicsPipeline_;

        uint32_t currentSwapchainFramebufferIndex_;

        AutoIdMap<Vulkan_Texture> textureMap_;
        UniqueBiMap<unsigned int, unsigned int> textureIdToUnitBiMap_;
        std::vector<unsigned int> textureUnitStalenessValues_;

        AutoIdMap<Vulkan_Mesh> meshMap_;
    };

    // public ------------------------------------------------------------------

    inline bool Vulkan_Renderer::GetDepthTesting() const
    {
        return depthTesting_;
    }

    inline void Vulkan_Renderer::SetDepthTesting(bool depthTesting)
    {
        depthTesting_ = depthTesting;
    }

    inline void Vulkan_Renderer::SetMultisampleAntiAliasing(bool multisampleAntiAliasing)
    {
        multisampleAntiAliasing_ = multisampleAntiAliasing;
    }

    inline void Vulkan_Renderer::GetFramebufferSize(
        unsigned int& width,
        unsigned int& height) const
    {
        width = pendingFrameBufferWidth_;
        height = pendingFrameBufferHeight_;
    }

    inline void Vulkan_Renderer::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        pendingFrameBufferWidth_ = width;
        pendingFrameBufferHeight_ = height;
        framebufferResized_ = true;
    }

    inline void Vulkan_Renderer::GetViewport(
        unsigned int& x,
        unsigned int& y,
        unsigned int& width,
        unsigned int& height)
    {
        x = viewportX_;
        y = viewportY_;
        width = viewportWidth_;
        height = viewportHeight_;
    }

    inline void Vulkan_Renderer::SetViewport(
        unsigned int x,
        unsigned int y,
        unsigned int width,
        unsigned int height)
    {
        viewportX_ = x;
        viewportY_ = y;
        viewportWidth_ = width;
        viewportHeight_ = height;
    }

    inline void Vulkan_Renderer::GetCameraViewport(
        float& x,
        float& y,
        float& width,
        float& height)
    {
        x = cameraViewportX_;
        y = cameraViewportY_;
        width = cameraViewportWidth_;
        height = cameraViewportHeight_;
    }

    inline void Vulkan_Renderer::SetCameraViewport(
        float x,
        float y,
        float width,
        float height)
    {
        cameraViewportX_ = x;
        cameraViewportY_ = y;
        cameraViewportWidth_ = width;
        cameraViewportHeight_ = height;
    }

    inline void Vulkan_Renderer::SetBorderColor(const glm::vec4& color)
    {
        borderColor_ = color;
    }

    inline void Vulkan_Renderer::SetClearColor(const glm::vec4& color)
    {
        clearColor_ = color;
    }

    inline void Vulkan_Renderer::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void Vulkan_Renderer::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void Vulkan_Renderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }

    inline void Vulkan_Renderer::SetDirectionalLight(
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular)
    {
        directionalLight_.direction = direction;
        directionalLight_.ambient = ambient;
        directionalLight_.diffuse = diffuse;
        directionalLight_.specular = specular;
    }

    inline void Vulkan_Renderer::ClearDirectionalLight()
    {
        directionalLight_.direction = glm::vec3(0.0f, 0.0f, -1.0f);
        directionalLight_.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        directionalLight_.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    inline void Vulkan_Renderer::ClearPointLights()
    {
        pointLights_.clear();
    }

    inline void Vulkan_Renderer::ClearSpotLights()
    {
        spotLights_.clear();
    }

    // protected ---------------------------------------------------------------

    inline uint32_t Vulkan_Renderer::CalculateMipLevels(unsigned int width, unsigned int height)
    {
        return (uint32_t)std::floor(std::log2(std::max(width, height))) + 1;
    }
}