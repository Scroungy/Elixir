#include "vulkan_base.h"

namespace VB
{
    void initialize(VulkanBase &data)
    {
        // Initialize vulkan
        create_instance(data);
        select_graphics_card(data);
        create_device_and_queues(data);
    }

    void destroy(VulkanBase &data)
    {
        vkDestroyDevice(data.device, nullptr);
        vkDestroyInstance(data.instance, nullptr);
    }

    void create_instance(VulkanBase &data)
    {
        volkInitialize();
        const char *instanceExtensionNames[INSTANCE_EXTENSION_COUNT] = {"VK_KHR_surface", "VK_KHR_win32_surface", "VK_KHR_get_surface_capabilities2"};
        VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Elixir",
            .applicationVersion = 1,
            .pEngineName = "ElixirEngine",
            .engineVersion = 1,
            .apiVersion = VK_API_VERSION_1_3};
        VkInstanceCreateInfo instanceInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = INSTANCE_EXTENSION_COUNT,
            .ppEnabledExtensionNames = instanceExtensionNames};
        vkCreateInstance(&instanceInfo, nullptr, &data.instance);
        if (data.instance == 0)
        {
            ExitProcess(1);
        }
        volkLoadInstance(data.instance);
    }

    void select_graphics_card(VulkanBase &data)
    {
        u32 graphicsCardCount = 0;
        VkPhysicalDevice graphicsCards[5]{};
        vkEnumeratePhysicalDevices(data.instance, &graphicsCardCount, nullptr);
        vkEnumeratePhysicalDevices(data.instance, &graphicsCardCount, graphicsCards);
        const char *deviceExtensionNames[DEVICE_EXTENSION_COUNT] = {"VK_KHR_swapchain"};
        if (graphicsCardCount == 0)
        {
            ExitProcess(1);
        }
        for (u32 k = 0; k < graphicsCardCount; k++)
        {
            u32 metMemoryTypes = 0;
            u32 extensionCount = 0;
            u32 metExtensions = 0;
            VkExtensionProperties graphicsCardExtensions[256]{};
            vkGetPhysicalDeviceProperties2(graphicsCards[k], &data.deviceProp);
            vkGetPhysicalDeviceMemoryProperties2(graphicsCards[k], &data.deviceMemProp);
            vkGetPhysicalDeviceFeatures2(graphicsCards[k], &data.deviceFeatures);
            vkEnumerateDeviceExtensionProperties(graphicsCards[k], nullptr, &extensionCount, nullptr);
            vkEnumerateDeviceExtensionProperties(graphicsCards[k], nullptr, &extensionCount, graphicsCardExtensions);
            for (u32 i = 0; i < DEVICE_EXTENSION_COUNT; i++)
            {
                for (u32 j = 0; j < extensionCount; j++)
                {
                    if (is_equal(deviceExtensionNames[i], graphicsCardExtensions[j].extensionName))
                    {
                        metExtensions += 1;
                    }
                }
            }
            for (u32 j = 0; j < data.deviceMemProp.memoryProperties.memoryTypeCount; j++)
            {
                if (data.memPropFlags & data.deviceMemProp.memoryProperties.memoryTypes[j].propertyFlags != 0)
                {
                    metMemoryTypes += 1;
                }
            }
            if (metMemoryTypes < 1 || metExtensions < DEVICE_EXTENSION_COUNT || data.deviceProp.properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                continue;
            }
            data.graphicsCard = graphicsCards[k];
            break;
        }
        if (data.graphicsCard == 0)
        {
            ExitProcess(1);
        }
    }

    void create_device_and_queues(VulkanBase &data)
    {
        u32 queueFamilyCount = 0;
        VkQueueFamilyProperties2 queueFamilyProperties[16]{};
        f32 queuePriorities[QUEUE_FAMILY_INDEX_COUNT]{};
        VkDeviceQueueCreateInfo queueInfos[QUEUE_FAMILY_INDEX_COUNT]{};
        const char *deviceExtensionNames[DEVICE_EXTENSION_COUNT] = {"VK_KHR_swapchain"};
        VkDeviceCreateInfo deviceInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                      .pNext = &data.deviceFeatures,
                                      .flags = 0,
                                      .queueCreateInfoCount = QUEUE_FAMILY_INDEX_COUNT,
                                      .pQueueCreateInfos = queueInfos,
                                      .enabledLayerCount = 0,
                                      .ppEnabledLayerNames = nullptr,
                                      .enabledExtensionCount = DEVICE_EXTENSION_COUNT,
                                      .ppEnabledExtensionNames = deviceExtensionNames,
                                      .pEnabledFeatures = nullptr};
        for (u32 i = 0; i < 16; i++)
        {
            queueFamilyProperties[i].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        }
        for (u32 i = 0; i < QUEUE_FAMILY_INDEX_COUNT; i++)
        {
            queueInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfos[i].pNext = nullptr;
            queueInfos[i].flags = 0;
            queueInfos[i].queueFamilyIndex = 0;
            queueInfos[i].queueCount = 1;
            queueInfos[i].pQueuePriorities = queuePriorities;
        }
        vkGetPhysicalDeviceQueueFamilyProperties2(data.graphicsCard, &queueFamilyCount, nullptr);
        vkGetPhysicalDeviceQueueFamilyProperties2(data.graphicsCard, &queueFamilyCount, queueFamilyProperties);
        for (u32 i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilyProperties[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT != 0)
            {
                data.queueFamilyIndices[0] = i;
                break;
            }
        }
        for (u32 i = 0; i < QUEUE_FAMILY_INDEX_COUNT; i++)
        {
            if (data.queueFamilyIndices[i] == UINT_MAX)
            {
                ExitProcess(1);
            }
            queueInfos[i].queueFamilyIndex = data.queueFamilyIndices[i];
        }
        vkCreateDevice(data.graphicsCard, &deviceInfo, nullptr, &data.device);
        if (data.device == 0)
        {
            ExitProcess(1);
        }
        volkLoadDevice(data.device);
        VkDeviceQueueInfo2 queueInfo2{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
                                      .pNext = nullptr,
                                      .flags = 0,
                                      .queueFamilyIndex = data.queueFamilyIndices[0],
                                      .queueIndex = 0};
        vkGetDeviceQueue2(data.device, &queueInfo2, &data.graphicsQueue);
        if (data.graphicsQueue == 0)
        {
            ExitProcess(1);
        }
    }

    bool is_equal(const char *first, const char *second)
    {
        while (*first)
        {
            if (*first != *second)
            {
                return false;
            }
            first++;
            second++;
        }
        return true;
    }
}
