#pragma once

// Vulkan specific defines
#define DEFAULT_INSTANCE_EXTENSION_COUNT 3
#define DEFAULT_INSTANCE_EXTENSIONS "VK_KHR_surface", "VK_KHR_win32_surface", "VK_KHR_get_surface_capabilities2"
#define DEFAULT_DEVICE_EXTENSION_COUNT 1
#define DEFAULT_DEVICE_EXTENSIONS "VK_KHR_swapchain"
#define DEFAULT_MEMORY_PROPERTY_FLAG_COUNT 2
#define DEFAULT_MEMORY_PROPERTY_FLAGS VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
#define SWAPCHAIN_IMAGE_COUNT 2

#include "defines.h"
#include "volk.h"
#include <array.h>

namespace VKH
{
    // BASE
    struct VulkanSettings
    {
        struct InstanceSettings
        {
            struct ApplicationCreateInfoSettings
            {
                const char *appName{};
                u32 appVersion = 1;
            };

            struct InstanceCreateInfoSettings
            {
                const void *pNext{};
                VkInstanceCreateFlags flags = 0;
                u32 layerCount = 0;
                std::Array<const char *> layerNames{};
                u32 extensionCount = DEFAULT_INSTANCE_EXTENSION_COUNT;
                std::Array<const char *> extensionNames{DEFAULT_INSTANCE_EXTENSIONS};
            };

            ApplicationCreateInfoSettings appCIS{};
            InstanceCreateInfoSettings instanceCIS{};
        };

        struct PhysicalDeviceSettings
        {
            void *memPropertiesPNext{};
            void *propertiesPNext{};
            void *featuresPNext{};
            u32 extensionCount = DEFAULT_DEVICE_EXTENSION_COUNT;
            std::Array<const char *> extensionNames{DEFAULT_DEVICE_EXTENSIONS};
            u32 memPropFlagCount = DEFAULT_MEMORY_PROPERTY_FLAG_COUNT;
            std::Array<VkMemoryPropertyFlags> memPropFlags{DEFAULT_MEMORY_PROPERTY_FLAGS};
        };

        struct DeviceSettings
        {
            struct DeviceCreateInfoSettings
            {
                const void *pNext{};
                VkDeviceCreateFlags flags = 0;
                u32 layerCount = 0;
                std::Array<const char *> layerNames{};
                u32 extensionCount = DEFAULT_DEVICE_EXTENSION_COUNT;
                std::Array<const char *> extensionNames{DEFAULT_DEVICE_EXTENSIONS};
            };

            PhysicalDeviceSettings physicalDeviceSettings{};
            DeviceCreateInfoSettings deviceCIS{};
        };

        InstanceSettings instanceSettings{};
        DeviceSettings deviceSettings{};
    };

    // RENDERER
    struct RenderSettings
    {
        struct SurfaceSettings
        {
            struct SurfaceInfoSettings
            {
                void *pNext{};
            };

            struct SurfaceFormatSettings
            {
                VkSurfaceFormatKHR surfaceFormat{.format = VK_FORMAT_B8G8R8A8_SRGB,
                                                 .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
            };

            struct PresentModeSettings
            {
                VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            };

            SurfaceInfoSettings surfaceInfoSettings{};
            SurfaceFormatSettings surfaceFormatSettings{};
            PresentModeSettings presentModeSettings{};
        };

        struct SwapchainSettings
        {
            struct SwapchainCreateInfoSettings
            {
                void *pNext{};
                VkSwapchainCreateFlagsKHR flags = 0;
                u32 imageArrayLayers = 0;
                VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
                VkSharingMode imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                u32 queueFamilyIndexCount = 0;
                std::Array<u32> pQueueFamilyIndices{};
                VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
                VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                VkBool32 clipped = VK_TRUE;
                VkSwapchainKHR oldSwapchain = 0;
            };

            struct SwapchainImageViewCreateSettings
            {
                VkImageViewCreateFlags flags = 0;
                VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
                VkComponentMapping components{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                              .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                              .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                              .a = VK_COMPONENT_SWIZZLE_IDENTITY};
                VkImageSubresourceRange subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                            .baseMipLevel = 0,
                                                            .levelCount = 1,
                                                            .baseArrayLayer = 0,
                                                            .layerCount = 1};
            };

            SwapchainCreateInfoSettings swapchainCreateInfoSettings{};
            SwapchainImageViewCreateSettings imageViewCreateSettings{};
        };

        struct SyncSettings
        {
            struct SemaphoreCreateSettings
            {
                void *pNext{};
            };

            struct FenceCreateSettings
            {
                void *pNext{};
                VkFenceCreateFlags flags = VK_FENCE_CREATE_SIGNALED_BIT;
            };

            SemaphoreCreateSettings semaphoreCreateSettings{};
            FenceCreateSettings fenceCreateSettings{};
        };

        struct CommandSettings
        {
            struct CommandPoolCreateSettings
            {
                VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
                u32 queueFamilyIndex = 0;
            };

            struct CommandBufferAllocateSettings
            {
                VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            };

            CommandPoolCreateSettings commandPoolCreateSettings{};
            CommandBufferAllocateSettings commandBufferAllocateSettings{};
        };

        SurfaceSettings surfaceSettings{};
        SwapchainSettings swapchainSettings{};
        SyncSettings syncSettings{};
        CommandSettings commandSettings{};
    };
}
