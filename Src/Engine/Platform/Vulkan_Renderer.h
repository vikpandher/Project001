#pragma once

#include "Engine/Renderer.h"

#include "vulkan/vulkan.h"

#include <string>



namespace Project001
{
    struct QueueFamilyIndices
    {
        bool valid_graphicsFamilyIndex;
        uint32_t graphicsFamilyIndex;

        bool valid_presentFamilyIndex;
        uint32_t presentFamilyIndex;

        bool isComplete() {
            return valid_graphicsFamilyIndex && valid_presentFamilyIndex;
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Vulkan_Renderer : public Renderer
    {
    public:
        Vulkan_Renderer(Window* windowPtr, unsigned int width, unsigned int height);
        ~Vulkan_Renderer() override;

        void SetDepthTesting(
            bool depthTesting) override {}

        void SetFramebufferSize(
            unsigned int width,
            unsigned int height) override {}

        void GetViewport(
            unsigned int& x,
            unsigned int& y,
            unsigned int& width,
            unsigned int& height) override {}

        void SetViewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height) override {}

        bool CreateTexture(
            unsigned int& textureId,
            unsigned int textureUnit,
            unsigned char* data,
            unsigned int width,
            unsigned int height,
            unsigned int bytesPerPixel) override { return true; }

        bool BindTexture(
            unsigned int textureId,
            unsigned int textureUnit) override { return true; }

        bool DeleteTexture(unsigned int textureId) override { return true; }

        void DeleteAllTextures() override {}

        void SetViewMatrix(const glm::mat4& viewMatrix) override {}
        void SetViewPosition(const glm::vec3& viewPosition) override {}
        void SetProjectionMatrix(const glm::mat4& projectionMatrix) override {}

        void SetDirectionalLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override {}
        void AddPointLight(
            const glm::vec3& position,
            float constant,
            float linear,
            float quadratic,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular) override {}
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
            const glm::vec3& specular) override {}

        void ClearDirectionalLight() override {}
        void ClearPointLights() override {}
        void ClearSpotLights() override {}

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
            bool lit) override { return true; }

        void PrepareCapabilities() override {}

        void Clear() override {}

        void Render() override;

        void SwapBuffers() override {}

    protected:
        void CreateVulkanInstance();
        void SetupDebugMessanger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();

        void RecreateSwapChain();
        void CleanupSwapChain();

        void DrawFrame();

        bool CheckValidationLayerSupport();

        std::vector<const char*> GetRequiredExtensions();

        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        bool IsDeviceSuitable(VkPhysicalDevice device);

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        std::vector<char> ReadFile(const std::string& filePath);

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        static const bool s_enableValidationLayers_;
        static const std::vector<const char*> s_validationLayers_;

        static const std::vector<const char*> s_deviceExtensions_;

        static const int s_maxFramesInFlight_;

        Window* windowPtr_;

        VkInstance vkInstance_;
        VkDebugUtilsMessengerEXT vkDebugMessenger_;

        VkSurfaceKHR vkSurface_;
        VkPhysicalDevice vkPhysicalDevice_;
        VkDevice vkDevice_;

        VkQueue vkGraphicsQueue_;
        VkQueue vkPresentQueue_;

        VkSwapchainKHR vkSwapChain_;
        std::vector<VkImage> vkSwapChainImages_;
        VkFormat vkSwapChainImageFormat_;
        VkExtent2D vkSwapChainExtent_;
        std::vector<VkImageView> vkSwapChainImageViews_;
        std::vector<VkFramebuffer> vkSwapChainFramebuffers_;

        VkRenderPass vkRenderPass_;
        VkPipelineLayout vkPipelineLayout_;
        VkPipeline vkGraphicsPipeline_;

        VkCommandPool vkCommandPool_;
        std::vector<VkCommandBuffer> vkCommandBuffers_;

        std::vector<VkSemaphore> vkImageAvailableSemaphores_;
        std::vector<VkSemaphore> vkRenderFinishedSemaphores_;
        std::vector<VkFence> vkInFlightFences_;
        uint32_t currentFrame_;

        bool framebufferResized_;

    private:
    };
}