#pragma once

#include "Typedefs.h"
#include "volk.h"

namespace VB
{
    struct VulkanBase
    {
        VkInstance instance{};
        VkPhysicalDevice graphicsCard{};
        VkDevice device{};
        VkQueue graphicsQueue{};
        u32 queueFamilyIndices[QUEUE_FAMILY_INDEX_COUNT]{};
        VkMemoryPropertyFlags memPropFlags{VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT};
        VkPhysicalDeviceMemoryProperties2 deviceMemProp{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2, .pNext = nullptr};
        VkPhysicalDeviceMaintenance4Properties deviceMaint4Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES, .pNext = nullptr};
        VkPhysicalDeviceMaintenance3Properties deviceMaint3Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES, .pNext = &deviceMaint4Prop};
        VkPhysicalDeviceVulkan13Properties device13Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES, .pNext = &deviceMaint3Prop};
        VkPhysicalDeviceVulkan12Properties device12Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES, .pNext = &device13Prop};
        VkPhysicalDeviceVulkan11Properties device11Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES, .pNext = &device12Prop};
        VkPhysicalDeviceProperties2 deviceProp{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = &device11Prop};
        VkPhysicalDeviceVulkan13Features device13Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, .pNext = nullptr};
        VkPhysicalDeviceVulkan12Features device12Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &device13Features};
        VkPhysicalDeviceVulkan11Features device11Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, .pNext = &device12Features};
        VkPhysicalDeviceFeatures2 deviceFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &device11Features};
        VkSurfaceCapabilities2KHR deviceCapabilities{.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR, .pNext = nullptr};
    };

    void initialize(VulkanBase &data);
    void destroy(VulkanBase &data);
    void create_instance(VulkanBase &data);
    void select_graphics_card(VulkanBase &data);
    void create_device_and_queues(VulkanBase &data);
    bool is_equal(const char *first, const char *second);
}
