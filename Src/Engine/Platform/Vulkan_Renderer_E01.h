#pragma once

#include "Engine/Renderer.h"

#include "vulkan/vulkan.h"



namespace Project001
{
    class Vulkan_Renderer_E01 : public Renderer
    {
    public:
        Vulkan_Renderer_E01(
            Window* windowPtr,
            unsigned int width,
            unsigned int height,
            bool multisampleAntiAliasing);

        ~Vulkan_Renderer_E01() override;

        void SetDepthTesting(
            bool depthTesting) override;

        void SetMultisampleAntiAliasing(
            bool multisampleAntiAliasing) override;

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

        bool CreateTexture(
            unsigned int& textureId,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool multisampleAntiAliasing,
            bool mipMaps) override
        {
            return true;
        }

        bool DeleteTexture(unsigned int textureId) override
        {
            return true;
        }

        void DeleteAllTextures() override
        {}

        void SetViewMatrix(const glm::mat4& viewMatrix) override;

        void SetViewPosition(const glm::vec3& viewPosition) override;

        void SetProjectionMatrix(const glm::mat4& projectionMatrix) override;

        void SetDirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override
        {}

        void AddPointLight(
            const glm::vec3& position,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override
        {}

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
            const glm::vec3& specular) override
        {}

        void ClearDirectionalLight() override
        {}

        void ClearPointLights() override
        {}

        void ClearSpotLights() override
        {}

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

        void FinishRendering() override
        {}

        void SwapBuffers() override;

    protected:
        struct UniformBufferObject
        {
            alignas(16) glm::mat4 u_View;
            alignas(16) glm::mat4 u_Projection;
        };

        void CreateVulkanInstance();
        void CleanUpVulkanInstance();

        void CreateSurface();

        void PickPhysicalDevice();

        void CreateLogicalDevice();

        void CreateRenderPass();
        void CleanUpRenderPass();
        void RecreateRenderPass();

        void CreateSwapChain();
        void CleanupSwapChain();
        void RecreateSwapChain();

        void CreateDescriptorSetLayout();

        void CreateGraphicsPipeline();
        void CleanUpGraphicsPipeline();
        void RecreateGraphicsPipeline();

        void CreateSyncObjects();
        void CleanUpSyncObjects();

        void CreateCommandPool();

        void CreateCommandBuffers();

        void CreateDataBuffers();
        void CleanUpDataBuffers();

        VkImageView CreateImageView(
            VkImage image,
            VkFormat format,
            VkImageAspectFlags imageAspectFlags,
            uint32_t mipLevels);

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

        std::vector<char> ReadFile(const char* filePath);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

        static const bool s_enableValidationLayers_;
        static const std::vector<const char*> s_validationLayers_;

        static const std::vector<const char*> s_deviceExtensions_;

        static const bool s_requireSamplerAnisotropy_;

        static const unsigned int s_indexBufferCapacity_;
        static const unsigned int s_vertexBufferCapacity_;

        Window* windowPtr_;
        unsigned int frameBufferWidth_;
        unsigned int frameBufferHeight_;
        bool multisampleAntiAliasing_;
        bool depthTesting_;

        VkClearColorValue clearColorValue_;

        unsigned int viewportX_;
        unsigned int viewportY_;
        unsigned int viewportWidth_;
        unsigned int viewportHeight_;

        glm::mat4 viewMatrix_;
        glm::vec3 viewPosition_;
        glm::mat4 projectionMatrix_;

        bool depthTestingChanged_;
        bool msaaChanged_;
        bool framebufferResized_;

        VkInstance vulkanInstance_;
        VkDebugUtilsMessengerEXT debugMessenger_;

        VkSurfaceKHR surface_;

        VkPhysicalDevice physicalDevice_;
        uint32_t graphicsQueueFamilyIndex_;
        uint32_t presentQueueFamilyIndex_;
        VkSurfaceCapabilitiesKHR surfaceCapabilities_;
        VkExtent2D surfaceExtent_;
        VkSurfaceFormatKHR surfaceFormat_;
        VkFormat depthFormat_;
        VkPresentModeKHR surfacePresentMode_;
        VkSampleCountFlagBits msaaSampleCount_;

        VkDevice logicalDevice_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        VkRenderPass renderPass_;

        VkSwapchainKHR swapChain_;
        uint32_t swapChainImageCount_;
        std::vector<VkImage> swapChainImages_;
        std::vector<VkImageView> swapChainImageViews_;
        VkImage colorImage_;
        VkDeviceMemory colorImageMemory_;
        VkImageView colorImageView_;
        VkImage depthImage_;
        VkDeviceMemory depthImageMemory_;
        VkImageView depthImageView_;
        std::vector<VkFramebuffer> swapChainFramebuffers_;

        VkDescriptorSetLayout descriptorSetLayout_;

        VkPipelineLayout pipelineLayout_;
        VkPipeline graphicsPipeline_;

        VkSemaphore imageAvailableSemaphore_;
        //VkSemaphore vertexDataUpdatedSemaphore_;
        VkSemaphore renderFinishedSemaphore_; // clearFinishedSemaphore
        VkFence inFlightFence_;

        VkCommandPool commandPool_;

        VkCommandBuffer commandBuffer_;

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

        VkBuffer uniformBuffer_;
        VkDeviceMemory uniformBufferMemory_;

        uint32_t currentFrame_;
        uint32_t currentImageIndex_;

    private:
    };

    inline void Vulkan_Renderer_E01::GetViewport(
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

    inline void Vulkan_Renderer_E01::SetViewport(
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

    inline void Vulkan_Renderer_E01::SetDepthTesting(
        bool depthTesting)
    {
        depthTesting_ = depthTesting;
        depthTestingChanged_ = true;
    }

    inline void Vulkan_Renderer_E01::SetMultisampleAntiAliasing(
        bool multisampleAntiAliasing)
    {
        multisampleAntiAliasing_ = multisampleAntiAliasing;
        msaaChanged_ = true;
    }

    inline void Vulkan_Renderer_E01::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        frameBufferWidth_ = width;
        frameBufferHeight_ = height;
        framebufferResized_ = true;
    }

    inline void Vulkan_Renderer_E01::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void Vulkan_Renderer_E01::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void Vulkan_Renderer_E01::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }
}