#pragma once

#include "vulkan_base.h"
#include "channel.h"

namespace VW
{
    struct SwapchainDeletion
    {
        VkSwapchainKHR swapchain{};
        VkImageView swapchainImageViews[SWAPCHAIN_IMAGE_COUNT];
    };

    struct PresentDataSOA
    {
        u32 swapchainImageIndicesDyn[MAX_WINDOWS]{};
        VkSwapchainKHR swapchainsDyn[MAX_WINDOWS]{};
        VkSemaphore acquireNextImageSemaphoresDyn[MAX_WINDOWS]{};
        VkSemaphore renderSemaphoresDyn[MAX_WINDOWS]{};
        VkCommandBuffer commandBuffersDyn[MAX_WINDOWS]{};
        VkPipelineStageFlags waitStageMasksDyn[MAX_WINDOWS]{};
    };

    // Used to store dynamic data for submit/present
    struct PresentData
    {
        u32 currentFrame = 0;            // Current multiplier used for offsets in non-dyn arrays
        u32 validWindowCount = 0;        // Count of acquired swapchain images
        u64 currentRenderTick = 0;       // Iteration of render
        u32 validWindows[MAX_WINDOWS]{}; // Array of valid window indices to access data from
        VkCommandBufferBeginInfo commandBufferBeginInfo[MAX_WINDOWS]{};
        VkViewport viewport[MAX_WINDOWS]{};
        VkRect2D scissor[MAX_WINDOWS]{};
        VkClearValue clearValue[MAX_WINDOWS]{};
        VkRenderingAttachmentInfo renderAttachmentInfo[MAX_WINDOWS]{};
        VkRenderingInfo renderInfo[MAX_WINDOWS]{};
        VkImageMemoryBarrier beginImageBarrier[MAX_WINDOWS]{};
        VkImageMemoryBarrier endImageBarrier[MAX_WINDOWS]{};
        PresentDataSOA soa[SWAPCHAIN_IMAGE_COUNT]{};
        VkSubmitInfo submitBatches[SWAPCHAIN_IMAGE_COUNT]{};
        VkPresentInfoKHR presentBatches[SWAPCHAIN_IMAGE_COUNT]{};
    };

    struct WindowData
    {
        u32 emptyDeletionQueueIndex = 0;
        u32 currentFrame = 0;
        HWND windowHandle = 0;
        VkMemoryRequirements imageMemoryRequirements{};
        VkRect2D area{};
        VkSurfaceKHR surface{};
        VkPresentModeKHR presentMode{};
        VkSurfaceFormatKHR surfaceFormat{};
        VkPipelineLayout graphicsPipelineLayout{};
        VkPipeline graphicsPipeline{};
        VkPipeline trianglePipeline{};
        VkSwapchainKHR swapchain{};
        u32 swapchainImageIndices[SWAPCHAIN_IMAGE_COUNT]{};
        VkCommandPool commandPools[SWAPCHAIN_IMAGE_COUNT]{};
        VkImage swapchainImages[SWAPCHAIN_IMAGE_COUNT]{};
        VkImageView swapchainImageViews[SWAPCHAIN_IMAGE_COUNT]{};
        VkCommandBuffer commandBuffers[SWAPCHAIN_IMAGE_COUNT]{};
        VkSemaphore acquireNextImageSemaphores[SWAPCHAIN_IMAGE_COUNT]{};
        VkSemaphore renderSemaphores[SWAPCHAIN_IMAGE_COUNT]{};
        SwapchainDeletion swapchainDeletionQueue[SWAPCHAIN_IMAGE_COUNT * 2]{};
        bool vsync = false;
    };

    struct VulkanWindows
    {
        u32 emptyWindowIndex = 0;
        VkFence renderFences[SWAPCHAIN_IMAGE_COUNT]{};
        WindowData windows[MAX_WINDOWS]{};
        PresentData presentData{};
    };

    void initialize(VkDevice device, VulkanWindows &data);
    void destroy(VkDevice device, VulkanWindows &data);
    void acquire(VkDevice device, VulkanWindows &data);
    void record(VulkanWindows &data);
    void map_to_soa(VulkanWindows &data);
    void render(VkDevice device, VulkanWindows &data, VkQueue graphicsQueue);
    bool is_empty(VulkanWindows &data);
    void create_window(VB::VulkanBase &base, VulkanWindows &data, CH::ConstructedWindowInfo message);
    void create_surface(VB::VulkanBase &data, WindowData &window, HINSTANCE instance, HWND hwnd);
    void select_surface_format(VkPhysicalDevice graphicsCard, WindowData &window);
    void select_present_mode(VkPhysicalDevice graphicsCard, WindowData &window);
    void create_swapchain(VkDevice device, WindowData &window);
    void get_swapchain_images(VkDevice device, WindowData &window);
    void create_swapchain_image_views(VkDevice device, WindowData &window);
    void create_graphics_pipeline_layout(VkDevice device, WindowData &window);
    void create_graphics_pipeline(VkDevice device, WindowData &window);
    void create_command_pools(VkDevice device, WindowData &window, u32 queueFamilyIndex);
    void create_command_buffers(VkDevice device, WindowData &window);
    void create_semaphores(VkDevice device, WindowData &window);
    void create_fences(VkDevice device, VulkanWindows &data);
    void load_shader_module(VkDevice device, PCWSTR fileName, VkShaderModule &shaderModule);

    void destroy_all_windows(VkInstance instance, VkDevice device, VulkanWindows &data);
    void destroy_window(VkInstance instance, VkDevice device, VulkanWindows &data, CH::DestroyedWindowInfo message);
    void resize(VkDevice device, VulkanWindows &data, CH::UpdatedWindowInfo message);
    void flush_deletion_queue(VkDevice device, WindowData &window);
}
