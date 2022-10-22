#pragma once

#include "vulkan_helper.h"
#include "window_manager.h"
#include <string.h>

namespace VM
{
    DWORD __stdcall updateLoop(LPVOID param);

    class VulkanManager
    {
    public:
        struct InstanceData
        {
            VkInstance instance{};
        };

        struct PhysicalDeviceProperties
        {
            VkPhysicalDeviceMemoryProperties2 deviceMemoryProperties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2};
            VkPhysicalDeviceMaintenance4Properties deviceMaintenance4Properties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES};
            VkPhysicalDeviceMaintenance3Properties deviceMaintenance3Properties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES, .pNext = &deviceMaintenance4Properties};
            VkPhysicalDeviceVulkan13Properties deviceVulkan13Properties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES, .pNext = &deviceMaintenance3Properties};
            VkPhysicalDeviceVulkan12Properties deviceVulkan12Properties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES, .pNext = &deviceVulkan13Properties};
            VkPhysicalDeviceVulkan11Properties deviceVulkan11Properties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES, .pNext = &deviceVulkan12Properties};
            VkPhysicalDeviceProperties2 deviceProperties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = &deviceVulkan11Properties};
        };

        struct PhysicalDeviceFeatures
        {
            VkPhysicalDeviceVulkan13Features deviceVulkan13Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
            VkPhysicalDeviceVulkan12Features deviceVulkan12Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &deviceVulkan13Features};
            VkPhysicalDeviceVulkan11Features deviceVulkan11Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, .pNext = &deviceVulkan12Features};
            VkPhysicalDeviceFeatures2 deviceFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &deviceVulkan11Features};
        };

        struct PhysicalDeviceData
        {
            VkPhysicalDevice physicalDevice{};
            PhysicalDeviceProperties properties{};
            PhysicalDeviceFeatures features{};
        };

        struct DeviceData
        {
            VkDevice device{};
            VkQueue graphicsQueue{};
            std::Array<VkQueue> computeQueues{};
            std::Array<VkQueue> transferQueues{};
            u32 graphicsQueueFamilyIndex = UINT_MAX;
            u32 computeQueueFamilyIndex = UINT_MAX;
            u32 transferQueueFamilyIndex = UINT_MAX;
            PhysicalDeviceData physicalDeviceData{};
        };

        struct RenderData
        {
            struct SurfaceData
            {
                VkSurfaceKHR surface{};
                VkSurfaceFormat2KHR surfaceFormat{};
                VkPresentModeKHR presentMode{};
            };

            struct SwapchainData
            {
                VkSwapchainKHR swapchain{};
                VkImage swapchainImages[SWAPCHAIN_IMAGE_COUNT]{};
                VkImageView swapchainImageViews[SWAPCHAIN_IMAGE_COUNT]{};
            };

            struct SyncData
            {
                VkSemaphore acquireSemaphores[SWAPCHAIN_IMAGE_COUNT]{};
                VkSemaphore renderSemaphores[SWAPCHAIN_IMAGE_COUNT]{};
                VkFence renderFences[SWAPCHAIN_IMAGE_COUNT]{};
            };

            struct CommandData
            {
                VkCommandPool commandPools[SWAPCHAIN_IMAGE_COUNT]{};
                VkCommandBuffer commandBuffer[SWAPCHAIN_IMAGE_COUNT]{};
            };

            SurfaceData surfaceData{};
            SwapchainData swapchainData{};
            SyncData syncData{};
            CommandData commandData{};
        };

        struct SceneData
        {
            struct Material
            {
            };

            struct Texture
            {
            };
        };

        InstanceData instanceData{};
        DeviceData deviceData{};
        std::Array<HWND> removeWindowQueue{};
        std::Array<RenderData> renderData{};
        SceneData sceneData{};

        VulkanManager(const VKH::VulkanSettings &settings);
        ~VulkanManager();

        void update();
        void createWindow(const VKH::RenderSettings &settings, u32 windowSettingsIndex);
        void removeWindow(HWND hwnd);
        void queueRemoveWindow(HWND hwnd);

    private:
        void createInstance(const VKH::VulkanSettings::InstanceSettings &settings);
        void selectPhysicalDevice(const VKH::VulkanSettings::PhysicalDeviceSettings &settings);
        void createDeviceAndQueues(const VKH::VulkanSettings::DeviceSettings &settings);
        void createSurfaceData(const VKH::RenderSettings::SurfaceSettings &settings, u32 index);
        void createSwapchainData(const VKH::RenderSettings::SwapchainSettings &settings, u32 index);
        void createSyncData(const VKH::RenderSettings::SyncSettings &settings, u32 index);
        void createCommandData(const VKH::RenderSettings::CommandSettings &settings, u32 index);
        void destroySurfaceData(u32 index);
        void destroySwapchainData(u32 index);
        void destroySyncData(u32 index);
        void destroyCommandData(u32 index);
    };
}