#pragma once

#include "vulkan_data.h"
#include "window_manager.h"

using namespace VKH;

namespace VKR
{
    class RenderManager
    {
    public:
        class RenderData
        {
        public:
            struct VulkanSurface
            {
                VkSurfaceKHR surface{};
                VkSurfaceFormat2KHR surfaceFormat{};
                VkPresentModeKHR presentMode{};
                VkSurfaceCapabilities2KHR surfaceCapabilities{.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR};
            };

            struct VulkanSwapchain
            {
                struct Deletion
                {
                    VkSwapchainKHR swapchain{};
                    VkImage images[SWAPCHAIN_IMAGE_COUNT]{};
                    VkImageView[SWAPCHAIN_IMAGE_COUNT]{};
                };

                VkSwapchainKHR swapchain{};
                VkImage images[SWAPCHAIN_IMAGE_COUNT]{};
                VkImageView imageViews[SWAPCHAIN_IMAGE_COUNT]{};
                u32 imageIndices[SWAPCHAIN_IMAGE_COUNT]{};
                Deletion deletionQueue{};
            };

            struct VulkanSync
            {
                VkFence renderFences[SWAPCHAIN_IMAGE_COUNT]{};
                VkSemaphore acquireSemaphores[SWAPCHAIN_IMAGE_COUNT]{};
                VkSemaphore renderSemaphores[SWAPCHAIN_IMAGE_COUNT]{};
            };

            struct VulkanCommand
            {
                VkCommandPool pools[SWAPCHAIN_IMAGE_COUNT]{};
                VkCommandBuffer buffers[SWAPCHAIN_IMAGE_COUNT]{};
            };

            VulkanSurface surfaceData{};
            VulkanSwapchain swapchainData{};
            VulkanSync syncData{};
            VulkanCommand commandData{};
            u32 windowIndex = 0;
            u32 currentFrame = 0;
            bool vsync = false;

            RenderData(VKD::VulkanData &vulkan, const VKH::RenderSettings &renderSettings, HWND hwnd);
            ~RenderData(VKD::VulkanData &vulkan);
            // Render
            void acquire();
            void record();
            void render();
            // Surface
            void create_surface(VKD::VulkanData &vulkan, HWND hwnd);
            void select_surface_format(VKD::VulkanData &vulkan, RenderSettings::SurfaceSettings::SurfaceFormatSettings &surfaceFormatSettings);
            void select_present_mode(VKD::VulkanData &vulkan, RenderSettings::SurfaceSettings::PresentModeSettings &presentModeSettings);
            // Swapchain
            void create_swapchain(VKD::VulkanData &vulkan, RenderSettings::SwapchainSettings &swapchainSettings, HWND hwnd);
            void get_swapchain_images(VKD::VulkanData &vulkan);
            void create_swapchain_image_views(VKD::VulkanData &vulkan, RenderSettings::ImageViewUsageSettings &imageViewUsageSettings, RenderSettings::ImageViewSettings &imageViewSettings);
            void flush_deletion_queue(VKD::VulkanData &vulkan);   // TODO
            void destroy_deletion_queue(VKD::VulkanData &vulkan); // TODO
            void resize(CH::UpdatedWindowInfo message);           // TODO
            // Sync
            void create_semaphores(VKD::VulkanData &vulkan, RenderSettings::SemaphoreSettings *semaphoreSettings);
            void create_fences(VKD::VulkanData &vulkan, RenderSettings::FenceSettings *fenceSettings);
            // Command obj
            void create_command_pools(VKD::VulkanData &vulkan, RenderSettings::CommandPoolSettings *commandPoolSettings);
            void create_command_buffers(VKD::VulkanData &vulkan, RenderSettings::CommandBufferSettings *commandBufferSettings);
        };

        struct Material
        {
            VkPipelineLayout graphicsPipelineLayout{};
            VkPipeline graphicsPipeline{};
        };

        VKD::VulkanData vulkan{};
        WM::WindowManager windowManager{};
        cstd::Array<RenderData> renderSurfaces{};

        RenderManager();
        // Windows
        void create_window(u32 settingsIndex);
        void destroy_window(u32 index);
        void destroy_extra(); // Destroy all windows but the main window
        // Materials
        void create_graphics_pipeline_layout();
        void create_graphics_pipeline();
        void load_shader_module(ShaderModuleSettings &shaderModuleSettings, PCWSTR fileName, VkShaderModule &shaderModule);
    };
}
