#pragma once

// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html
// https://registry.khronos.org/vulkan/specs/1.3/styleguide.html

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

        // There is no global state in Vulkan. All per-application state is
        // stored in a VkInstance object. Creating a VkInstance object
        // initializes the Vulkan library and allows the application to pass
        // information about itself to the implementation.
        VkInstance vkInstance_;
        // A messenger object which handles passing along debug messages to a
        // proveded callback.
        VkDebugUtilsMessengerEXT vkDebugMessenger_;

        // Native platform surface or window objects are abstracted by vulkan
        // surface objects.
        VkSurfaceKHR vkSurface_;
        // Represents a physical device installed in the system.
        VkPhysicalDevice vkPhysicalDevice_;
        // A logical device is created as a connection to a physical device.
        // Multiple logical devices can be made for a physical device. Logical
        // devices are independent.
        VkDevice vkDevice_;

        // Creating a logical device also creates the queues associated with
        // said device. A queue executes command buffers and sparse binding
        // operations on a device.
        VkQueue vkGraphicsQueue_;
        VkQueue vkPresentQueue_;

        // A swapchain object provides the ability to present rendering results
        // to a surface. It is an abstraction for an array of presentable images
        // that are associated with a surface.
        // One image is displayed at a time, but multiple images can be queued.
        // An application renders to an image and then queues it for
        // presentation.
        // A native window cannot be associated with more than one non-retired
        // swapchain at a time. Swapchains cannot be created for native windows
        // that nav ea non-Vulkan graphics API sufrace associated with them.
        VkSwapchainKHR vkSwapChain_;
        // Images represent miltidimensional (up to 3) arrays of data
        std::vector<VkImage> vkSwapChainImages_;
        // An image's format
        VkFormat vkSwapChainImageFormat_;
        // Extents are used to describe the size of a rectangular region of
        // pixels for a image or framebuffer
        VkExtent2D vkSwapChainExtent_;
        // Image objects are not directly accessed by pipeline shaders for
        // reading or writing image data. Instead, image views representing
        // contiguous ranges of the image subresources and containing additional
        // metadata are used.
        std::vector<VkImageView> vkSwapChainImageViews_;
        // Framebuffers represent a collection of specific memory attachments
        // that a render pass instance uses.
        std::vector<VkFramebuffer> vkSwapChainFramebuffers_;

        // This object represents a collection of attachments, subpasses,
        // dependencies between subpasses, and how describes how the attachments
        // are used over the course of the subpasses.
        // A subpass represents a phase of rendering that reads and writes a
        // subset of the attachments in a render pass. Rendering commands are
        // recorded into a particular subpass of the render pass instance.
        VkRenderPass vkRenderPass_;
        // A pipeline layout represents a sequence of descriptor sets with each
        // having a specific layout. This sequence of layouts is used to
        // determine the interface between shader stages and shader resources.
        VkPipelineLayout vkPipelineLayout_;
        // Compute, ray tracing, and graphics pipielines are each represented by
        // this handle. The graphics pipeline is the model that descirbes what
        // steps the graphics system preforms to render. Graphics pipeline
        // consist of multiple shader stages, multiple fixed-function stages,
        // and a pipeline layout.
        VkPipeline vkGraphicsPipeline_;

        // Command pools are opaque objects that command buffer memory is
        // allocated from, and which allow the implementation to amortize
        // (to liquidate by installment payments?) the cost of resource
        // creation across multiple command buffers. Command pools are
        // externally synchronized, meaning that a command pool must not be used
        // concurrently in multiple threads.
        // VkCommandPool objects are parents of VkCommandBuffer objects.
        VkCommandPool vkCommandPool_;
        // Command buffers are objects used to record commands which can be
        // subsequently submitted to a device queue for execution. There are two
        // levels of command buffers - primary command buffers, which can
        // execute secondary command buffers, and which are submitted to queues,
        // and secondary command buffers, which can be executed by primary
        // command buffers, and which are not directly submitted to queues.
        // Each command buffer amangers state independently of other command
        // buffers.
        std::vector<VkCommandBuffer> vkCommandBuffers_;

        // Semephores are a synchronization primative that can be used to insert
        // a dependency between queue operations or between a queue operation
        // and the host.
        std::vector<VkSemaphore> vkImageAvailableSemaphores_;
        std::vector<VkSemaphore> vkRenderFinishedSemaphores_;
        // Fences are a synchronization primative that can be used to insert a
        // dependency from a queue to the host.
        std::vector<VkFence> vkInFlightFences_;
        uint32_t currentFrame_;

        bool framebufferResized_;

    private:
    };
}