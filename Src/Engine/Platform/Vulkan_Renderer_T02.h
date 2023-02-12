#pragma once

#include "Engine/Renderer.h"

#include "vulkan/vulkan.h"



namespace Project001
{
    class Vulkan_Renderer_T02 : public Renderer
    {
    public:
        Vulkan_Renderer_T02(
            Window* windowPtr,
            unsigned int width,
            unsigned int height,
            bool multisampleAntaiAliasing);

        ~Vulkan_Renderer_T02() override;

        void SetDepthTesting(bool depthTesting) override;

        void SetMultisampleAntiAliasing(bool multisampleAntaiAliasing) override;

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
            unsigned int textureUnit,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel,
            bool mipMaps) override
        {
            return true;
        }

        bool BindTexture(
            unsigned int textureId,
            unsigned int textureUnit) override
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

        void FinishRendering() override;

        void SwapBuffers() override;

    protected:
        struct UniformBufferObject
        {
            alignas(16) glm::mat4 u_View;
            alignas(16) glm::mat4 u_Projection;
        };

        void CreateVulkanInstance();
        void DeleteVulkanInstance();

        void CreateSurface();
        void DeleteSurface();

        void PickPhysicalDevice();

        void CreateLogicalDevice();
        void DeleteLogicalDevice();

        void CreateSwapChain();
        void DeleteSwapChain();

        void CreateDataBuffers();
        void DeleteDataBuffers();

        void CreateDescriptorSetLayout();
        void DeleteDescriptorSetLayout();

        void CreateDescriptorPool();
        void DeleteDescriptorPool();

        void CreateDescriptorSets();

        void CreateRenderPass();
        void DeleteRenderPass();

        void CreateGraphicsPipeline();
        void DeleteGraphicsPipeline();

        void CreateCommandPool();
        void DeleteCommandPool();

        void CreateCommandBuffers();

        void CreateSyncObjects();
        void DeleteSyncObjects();






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

        std::vector<char> ReadFile(const char* const filePath);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        static const bool s_enableValidationLayers_;
        static const std::vector<const char*> s_validationLayers_;

        static const std::vector<const char*> s_deviceExtensions_;

        static const bool s_requireSamplerAnisotropy_;

        static const unsigned int s_indexBufferCapacity_;
        static const unsigned int s_vertexBufferCapacity_;

        Window* windowPtr_;
        unsigned int frameBufferWidth_;
        unsigned int frameBufferHeight_;
        bool multisampleAntaiAliasing_;
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

        VkBuffer uniformBuffer_;
        VkDeviceMemory uniformBufferMemory_;

        VkDescriptorSetLayout descriptorSetLayout_;

        VkDescriptorPool descriptorPool_;

        VkDescriptorSet descriptorSet_;

        VkRenderPass renderPass_;
        std::vector<VkFramebuffer> swapchainFramebuffers_;

        VkPipelineLayout pipelineLayout_;
        VkPipeline graphicsPipeline_;

        VkCommandPool commandPool_;

        VkCommandBuffer commandBuffer_;

        VkFence mainFence_;

        uint32_t currentSwapchainFramebufferIndex_;

    private:
    };

    inline void Vulkan_Renderer_T02::SetDepthTesting(
        bool depthTesting)
    {
        depthTesting_ = depthTesting;
        depthTestingChanged_ = true;
    }

    inline void Vulkan_Renderer_T02::SetMultisampleAntiAliasing(
        bool multisampleAntaiAliasing)
    {
        multisampleAntaiAliasing_ = multisampleAntaiAliasing;
        msaaChanged_ = true;
    }

    inline void Vulkan_Renderer_T02::SetFramebufferSize(
        unsigned int width,
        unsigned int height)
    {
        frameBufferWidth_ = width;
        frameBufferHeight_ = height;
        framebufferResized_ = true;
    }

    inline void Vulkan_Renderer_T02::GetViewport(
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

    inline void Vulkan_Renderer_T02::SetViewport(
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

    inline void Vulkan_Renderer_T02::SetBorderColor(const glm::vec4& color)
    {
        borderColor_ = color;
    }

    inline void Vulkan_Renderer_T02::SetClearColor(const glm::vec4& color)
    {
        clearColor_ = color;
    }

    inline void Vulkan_Renderer_T02::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        viewMatrix_ = viewMatrix;
    }

    inline void Vulkan_Renderer_T02::SetViewPosition(const glm::vec3& viewPosition)
    {
        viewPosition_ = viewPosition;
    }

    inline void Vulkan_Renderer_T02::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        projectionMatrix_ = projectionMatrix;
    }
}