#pragma once

#include "Engine/Renderer.h"

#include "vulkan/vulkan.h"




namespace Project001
{
    class Vulkan_Renderer : public Renderer
    {
    public:
        Vulkan_Renderer(
            Window* windowPtr,
            unsigned int width,
            unsigned int height,
            bool multisampleAntaiAliasing);

        ~Vulkan_Renderer() override;

        void SetDepthTesting(
            bool depthTesting) override
        {}

        void SetMultisampleAntiAliasing(
            bool multisampleAntaiAliasing) override
        {}

        void SetFramebufferSize(
            unsigned int width,
            unsigned int height) override
        {}

        // viewport origin is the bottom left corner
        void GetViewport(
            unsigned int& x,
            unsigned int& y,
            unsigned int& width,
            unsigned int& height) override
        {}

        void SetViewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) override
        {}

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

        void SetViewMatrix(const glm::mat4& viewMatrix) override
        {}

        void SetViewPosition(const glm::vec3& viewPosition) override
        {}

        void SetProjectionMatrix(const glm::mat4& projectionMatrix) override
        {}

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

        void BeginRendering() override
        {}

        void Clear() override
        {}

        bool AddMesh(
            const MeshVertex* meshVerticies,
            unsigned int meshVertexCount,
            const unsigned int* meshIndicies,
            unsigned int meshIndexCount,
            unsigned int textureIndex,
            unsigned int specularIndex,
            const glm::vec3& position,
            const glm::quat& orientation,
            const glm::vec3& scale,
            const glm::vec4& color,
            float shininess,
            bool translucent,
            bool lit) override
        {
            return true;
        }

        void Render() override
        {}

        void FinishRendering() override
        {}

        void SwapBuffers() override
        {}

    protected:
        void CreateVulkanInstance();
        void SetupDebugMessanger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();

        void CreateCommandPool();

        void CreateRenderPass();

        void CreateDescriptorSetLayout();

        void CreateSwapChain();
        void CreateImageViews();
        void CreateColorResources();
        void CreateDepthResources();
        void CreateFramebuffers();




        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugUtilsMessengerCreateInfo);

        VkFormat FindDepthFormat();

        VkFormat FindSupportedFormat(
            const std::vector<VkFormat>& formatCandidates,
            VkImageTiling imageTiling,
            VkFormatFeatureFlags formatFeatureFlags);

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

        uint32_t FindMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags memoryPropertyFlags);

        static const bool s_enableValidationLayers_;
        static const std::vector<const char*> s_validationLayers_;

        static const std::vector<const char*> s_deviceExtensions_;

        Window* windowPtr_;

        VkInstance vulkanInstance_;
        VkDebugUtilsMessengerEXT debugMessenger_;
        VkSurfaceKHR surface_;

        VkPhysicalDevice physicalDevice_;

        uint32_t graphicsQueueFamilyIndex_;
        uint32_t presentQueueFamilyIndex_;

        VkSurfaceCapabilitiesKHR swapChainSurfaceCapabilities_;
        VkSurfaceFormatKHR swapChainSurfaceFormat_;
        VkPresentModeKHR swapChainPresentMode_;
        VkExtent2D swapChainExtent_;
        VkSampleCountFlagBits swapChainMsaaSamples_;

        VkDevice logicalDevice_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        VkCommandPool commandPool_;

        VkRenderPass renderPass_;

        VkDescriptorSetLayout descriptorSetLayout_;

        VkSwapchainKHR swapChain_;
        std::vector<VkImage> swapChainImages_;
        std::vector<VkImageView> swapChainImageViews_;
        std::vector<VkFramebuffer> swapChainFramebuffers_;

        VkImage colorImage_;
        VkDeviceMemory colorImageMemory_;
        VkImageView colorImageView_;

        VkImage depthImage_;
        VkDeviceMemory depthImageMemory_;
        VkImageView depthImageView_;

    private:
    };
}