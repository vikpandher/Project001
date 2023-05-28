#pragma once

#include "Engine/BiMap.h"
#include "Engine/Renderer.h"

#include "vulkan/vulkan.h"

#include <deque>



namespace Project001
{
    class Vulkan_Renderer : public Renderer
    {
    public:
        Vulkan_Renderer(
            Window* windowPtr,
            unsigned int frameBufferWidth,
            unsigned int frameBufferHeight,
            unsigned int indexBufferCapacity,
            unsigned int vertexBufferCapacity,
            bool multisampleAntiAliasing);

        ~Vulkan_Renderer() override;

        void SetDepthTesting(bool depthTesting) override;

        void SetMultisampleAntiAliasing(bool multisampleAntiAliasing) override;

        void SetIndexBufferCapacity(unsigned int capacity) override;

        void SetVertexBufferCapacity(unsigned int capacity) override;

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


        void SetBorderColor(const glm::vec4& color) override;

        void SetClearColor(const glm::vec4& color) override;

        bool CreateTexture(
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

        bool AddMesh(
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
            bool lit) override;

        void Render() override;

        void FinishRendering() override;

        void SwapBuffers() override;

    protected:
        struct Vulkan_Texture
        {
            VkImage textureImage_;
            VkDeviceMemory textureImageMemory_;
            VkImageView textureImageView_;
            VkSampler textureSampler_;
        };

        void CreateVulkanInstance();
        void DeleteVulkanInstance();

        void CreateSurface();
        void DeleteSurface();

        void PickPhysicalDevice();

        void CreateLogicalDevice();
        void DeleteLogicalDevice();

        void CreateCommandPool();
        void DeleteCommandPool();

        void CreateCommandBuffers();

        void CreateSyncObjects();
        void DeleteSyncObjects();

        void CreateSwapchain();
        void DeleteSwapchain();

        void CreateIndexDataBuffers();
        void DeleteIndexDataBuffers();

        void CreateVertexDataBuffers();
        void DeleteVertexDataBuffers();

        void CreateOtherDataBuffers();
        void DeleteOtherDataBuffers();

        void CreateDefaultTexture();
        void DeleteDefaultTexture();

        void CreateDescriptorSetLayouts();
        void DeleteDescriptorSetLayouts();

        void CreateDescriptorPool();
        void DeleteDescriptorPool();

        void CreateDescriptorSets();

        void CreatePipelineLayouts();
        void DeletePipelineLayouts();

        void CreateBatchShaderModules();
        void DeleteBatchShaderModules();

        void CreatePrimaryRenderPasses();
        void DeletePrimaryRenderPasses();

        void CreatePrimaryGraphicsPipelines();
        void DeletePrimaryGraphicsPipelines();

        void CreatePrimaryFrameBufferAttachments();
        void DeletePrimaryFrameBufferAttachments();

        void CreatePrimaryFrameBuffers();
        void DeletePrimaryFrameBuffers();

        void CreateScreenShaderModules();
        void DeleteScreenShaderModules();

        void CreateSecondaryRenderPass();
        void DeleteSecondaryRenderPass();

        void CreateSecondaryGraphicsPipeline();
        void DeleteSecondaryGraphicsPipeline();

        void CreateSwapchainFramebuffers();
        void DeleteSwapchainFramebuffers();




        void AcquireNextImage();

        void HandleFramebufferResize();

        void HandleSwapchainFramebufferResize();

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

        std::vector<char> ReadFile(const char* const filePath);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        VkRenderPass CreatePrimaryRenderPass(
            bool msaa,
            bool depthTesting);

        VkPipeline CreatePrimaryGraphicsPipeline(
            VkRenderPass& renderPass,
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
        unsigned int indexBufferCapacity_;
        unsigned int vertexBufferCapacity_;
        bool multisampleAntiAliasing_;
        bool depthTesting_;

        unsigned int viewportX_;
        unsigned int viewportY_;
        unsigned int viewportWidth_;
        unsigned int viewportHeight_;

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

        VkSwapchainKHR swapchain_;
        uint32_t swapchainImageCount_;
        std::vector<VkImage> swapchainImages_;
        std::vector<VkImageView> swapchainImageViews_;

        VkBuffer vertexStagingBuffer_;
        VkDeviceMemory vertexStagingBufferMemory_;
        VertexData* vertexStagingBufferDataPtr_;
        size_t vertexCount_;

        VkBuffer indexStagingBuffer_;
        VkDeviceMemory indexStagingBufferMemory_;
        uint32_t* indexStagingBufferDataPtr_;
        size_t indexCount_;

        VkBuffer vertexBuffer_;
        VkDeviceMemory vertexBufferMemory_;

        VkBuffer indexBuffer_;
        VkDeviceMemory indexBufferMemory_;

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

        VkShaderModule vertexBatchShaderModule_;
        VkShaderModule fragmentBatchShaderModule_;

        VkPipelineLayout primaryPipelineLayout_;
        VkPipelineLayout secondaryPipelineLayout_;

        VkRenderPass primaryRenderPass1_; // No  MSAA && No  DepthTesting
        VkRenderPass primaryRenderPass2_; // No  MSAA && Yes DepthTesting
        VkRenderPass primaryRenderPass3_; // Yes MSAA && No  DepthTesting
        VkRenderPass primaryRenderPass4_; // Yes MSAA && Yes DepthTesting

        VkPipeline primaryGraphicsPipeline1_;
        VkPipeline primaryGraphicsPipeline2_;
        VkPipeline primaryGraphicsPipeline3_;
        VkPipeline primaryGraphicsPipeline4_;

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

        VkCommandPool commandPool_;

        VkCommandBuffer commandBuffer_;

        VkFence mainFence_;
        VkSemaphore imageAvailableSemaphore_;
        VkSemaphore renderFinishedSemaphore_;

        uint32_t currentSwapchainFramebufferIndex_;

        std::deque<unsigned int> recycledTextureIds_;
        std::map<unsigned int, Vulkan_Texture> textureMap_;
        BiMap<unsigned int, unsigned int> textureIdToUnitBiMap_;
        std::vector<unsigned int> textureUnitStalenessValues_;

    private:
    };

    // public ------------------------------------------------------------------

    inline void Vulkan_Renderer::SetDepthTesting(bool depthTesting)
    {
        depthTesting_ = depthTesting;
    }

    inline void Vulkan_Renderer::SetMultisampleAntiAliasing(bool multisampleAntiAliasing)
    {
        multisampleAntiAliasing_ = multisampleAntiAliasing;
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