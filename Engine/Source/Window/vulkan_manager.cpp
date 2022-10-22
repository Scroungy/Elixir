#include "vulkan_manager.h"

namespace VM
{
    DWORD __stdcall updateLoop(LPVOID param)
    {
        VulkanManager *v = static_cast<VulkanManager *>(param);
        while (!WM::WindowManager::should_close())
        {
            v->update();
        }
        return 0;
    }

    VulkanManager::VulkanManager(const VKH::VulkanSettings &settings)
    {
        // Initialize volk
        if (volkInitialize() != VK_SUCCESS)
        {
            ExitProcess(1); // Something went wrong with volk init, exit
        }
        createInstance(settings.instanceSettings);
        selectPhysicalDevice(settings.deviceSettings.physicalDeviceSettings);
        createDeviceAndQueues(settings.deviceSettings);
    }

    VulkanManager::~VulkanManager()
    {
        if (deviceData.device != 0)
        {
            vkDeviceWaitIdle(deviceData.device);
            const u32 winCount = WM::WindowManager::windows.size();
            for (u32 i = 0; i < winCount; i++)
            {
                for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
                {
                    vkDestroySemaphore(deviceData.device, renderData.at(i).syncData.acquireSemaphores[k], nullptr);
                    vkDestroySemaphore(deviceData.device, renderData.at(i).syncData.renderSemaphores[k], nullptr);
                    vkDestroyFence(deviceData.device, renderData.at(i).syncData.renderFences[k], nullptr);
                }
                for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
                {
                    vkDestroyCommandPool(deviceData.device, renderData.at(i).commandData.commandPools[k], nullptr);
                }
                for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
                {
                    vkDestroyImageView(deviceData.device, renderData.at(i).swapchainData.swapchainImageViews[k], nullptr);
                }
                vkDestroySwapchainKHR(deviceData.device, renderData.at(i).swapchainData.swapchain, nullptr);
                vkDestroySurfaceKHR(instanceData.instance, renderData.at(i).surfaceData.surface, nullptr);
            }
            vkDestroyDevice(deviceData.device, nullptr);
            vkDestroyInstance(instanceData.instance, nullptr);
        }
    }

    void VulkanManager::update()
    {
        for (u32 i = 0; i < WM::WindowManager::windows.size(); i++)
        {
            // Render
        }
        if (removeWindowQueue.size() > 0)
        {
            for (u32 i = 0; i < removeWindowQueue.size(); i++)
            {
                removeWindow(removeWindowQueue.at(i));
                removeWindowQueue.pop_back();
            }
            removeWindowQueue.clear();
        }
    }

    void VulkanManager::createInstance(const VKH::VulkanSettings::InstanceSettings &settings)
    {
        // Get available instance layers
        u32 retrievedLayerCount = 0;                                       // Amount of layers queried
        u32 metLayers = 0;                                                 // Amount of required layers that are actually supported
        vkEnumerateInstanceLayerProperties(&retrievedLayerCount, nullptr); // Get amount of layers
        std::Array<VkLayerProperties> layers(retrievedLayerCount);
        vkEnumerateInstanceLayerProperties(&retrievedLayerCount, layers.data()); // Fill array with queried layers
        // Check supported layers by matching names
        for (u32 i = 0; i < settings.instanceCIS.layerCount; i++)
        {
            for (u32 j = 0; j < retrievedLayerCount; j++)
            {
                if (std::is_equal(settings.instanceCIS.layerNames.at(i), layers[j].layerName))
                {
                    metLayers++;
                }
            }
        }
        if (metLayers < settings.instanceCIS.layerCount)
        {
            ExitProcess(1); // Not all required layers are supported, exit
        }
        // Get available instance extensions
        u32 retrievedExtensionCount = 0;                                                    // Amount of extensions queried
        u32 metExtensions = 0;                                                              // Amount of required extensions that are actually supported
        vkEnumerateInstanceExtensionProperties(nullptr, &retrievedExtensionCount, nullptr); // Get amount of extensions
        std::Array<VkExtensionProperties> extensions(retrievedExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &retrievedExtensionCount, extensions.data()); // Fill array with queried extensions
        // Check supported extensions by matching names
        for (u32 i = 0; i < settings.instanceCIS.extensionCount; i++)
        {
            for (u32 j = 0; j < retrievedExtensionCount; j++)
            {
                if (std::is_equal(settings.instanceCIS.extensionNames.at(i), extensions[j].extensionName))
                {
                    metExtensions++;
                }
            }
        }
        if (metExtensions < settings.instanceCIS.extensionCount)
        {
            ExitProcess(1); // Not all required extensions are supported, exit
        }
        // Create vulkan instance and load instance fn pointers
        VkApplicationInfo appInfo{.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                  .pNext = nullptr,
                                  .pApplicationName = settings.appCIS.appName,
                                  .applicationVersion = settings.appCIS.appVersion,
                                  .pEngineName = ASCII_ENGINE_NAME,
                                  .engineVersion = 1,
                                  .apiVersion = VK_API_VERSION_1_3};

        VkInstanceCreateInfo instanceInfo{.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                          .pNext = settings.instanceCIS.pNext,
                                          .flags = settings.instanceCIS.flags,
                                          .pApplicationInfo = &appInfo,
                                          .enabledLayerCount = settings.instanceCIS.layerCount,
                                          .ppEnabledLayerNames = settings.instanceCIS.layerNames.data(),
                                          .enabledExtensionCount = settings.instanceCIS.extensionCount,
                                          .ppEnabledExtensionNames = settings.instanceCIS.extensionNames.data()};
        if (vkCreateInstance(&instanceInfo, nullptr, &instanceData.instance) != VK_SUCCESS)
        {
            ExitProcess(1); // Something went wrong creating instance, exit
        }
        volkLoadInstance(instanceData.instance);
    }

    void VulkanManager::selectPhysicalDevice(const VKH::VulkanSettings::PhysicalDeviceSettings &settings)
    {
        // Get available physical devices
        u32 retrievedPhysicalDeviceCount = 0;                                                      // Amount of physical devices queried
        vkEnumeratePhysicalDevices(instanceData.instance, &retrievedPhysicalDeviceCount, nullptr); // Get amount of physical devices
        if (retrievedPhysicalDeviceCount == 0)
        {
            ExitProcess(1); // No devices found, exit
        }
        std::Array<VkPhysicalDevice> physicalDevices(retrievedPhysicalDeviceCount);
        vkEnumeratePhysicalDevices(instanceData.instance, &retrievedPhysicalDeviceCount, physicalDevices.data()); // Fill array with queried physical devices
        // Setup features and properties
        deviceData.physicalDeviceData.properties.deviceMemoryProperties.pNext = settings.memPropertiesPNext;
        deviceData.physicalDeviceData.properties.deviceMaintenance4Properties.pNext = settings.propertiesPNext;
        deviceData.physicalDeviceData.features.deviceVulkan13Features.pNext = settings.featuresPNext;
        // Find suitable physical device
        for (u32 i = 0; i < retrievedPhysicalDeviceCount; i++)
        {
            VkPhysicalDevice &currentPhysicalDevice = physicalDevices.at(i);
            // Acquire physical device properties and features
            vkGetPhysicalDeviceProperties2(currentPhysicalDevice, &deviceData.physicalDeviceData.properties.deviceProperties);
            vkGetPhysicalDeviceMemoryProperties2(currentPhysicalDevice, &deviceData.physicalDeviceData.properties.deviceMemoryProperties);
            vkGetPhysicalDeviceFeatures2(currentPhysicalDevice, &deviceData.physicalDeviceData.features.deviceFeatures);
            // Get available physical device extension properties
            u32 retrievedExtensionCount = 0;                                                                         // Amount of extensions queried
            vkEnumerateDeviceExtensionProperties(currentPhysicalDevice, nullptr, &retrievedExtensionCount, nullptr); // Get amount of physical device extension properties
            std::Array<VkExtensionProperties> physicalDeviceExtensionProperties(retrievedExtensionCount);
            vkEnumerateDeviceExtensionProperties(currentPhysicalDevice, nullptr, &retrievedExtensionCount, physicalDeviceExtensionProperties.data()); // Fill array with queried physical device extension properties
            // Check supported extensions by matching names
            u32 metExtensions = 0; // Amount of required extensions that are actually supported
            for (u32 k = 0; k < settings.extensionCount; k++)
            {
                for (u32 j = 0; j < retrievedExtensionCount; j++)
                {
                    if (std::is_equal(settings.extensionNames.at(k), physicalDeviceExtensionProperties[j].extensionName))
                    {
                        metExtensions++;
                    }
                }
            }
            // Check supported memory types
            u32 metMemoryTypes = 0; // Amount of required memory types that are actually supported
            for (u32 k = 0; k < settings.memPropFlagCount; k++)
            {
                for (u32 j = 0; j < deviceData.physicalDeviceData.properties.deviceMemoryProperties.memoryProperties.memoryTypeCount; j++)
                {
                    if (settings.memPropFlags.at(k) & deviceData.physicalDeviceData.properties.deviceMemoryProperties.memoryProperties.memoryTypes[j].propertyFlags == settings.memPropFlags.at(k))
                    {
                        metMemoryTypes++;
                        break;
                    }
                }
            }
            // Check physical device meets requirements
            if (metMemoryTypes < settings.memPropFlagCount || metExtensions < settings.extensionCount || deviceData.physicalDeviceData.properties.deviceProperties.properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                continue;
            }
            deviceData.physicalDeviceData.physicalDevice = currentPhysicalDevice; // Select physical device
            break;
        }
        if (deviceData.physicalDeviceData.physicalDevice == 0)
        {
            ExitProcess(1); // No suitable physical device, exit
        }
    }

    void VulkanManager::createDeviceAndQueues(const VKH::VulkanSettings::DeviceSettings &settings)
    {
        // Get queue family properties
        u32 retrievedQueueFamilyPropertyCount = 0;                                                                                            // Amount of queried queue family properties
        vkGetPhysicalDeviceQueueFamilyProperties2(deviceData.physicalDeviceData.physicalDevice, &retrievedQueueFamilyPropertyCount, nullptr); // Get amount of queue family properties
        std::Array<VkQueueFamilyProperties2> queueFamilyProperties(retrievedQueueFamilyPropertyCount);
        for (u32 i = 0; i < queueFamilyProperties.size(); i++)
        {
            queueFamilyProperties.at(i).sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
            queueFamilyProperties.at(i).pNext = nullptr;
            queueFamilyProperties.at(i).queueFamilyProperties = {};
        }
        vkGetPhysicalDeviceQueueFamilyProperties2(deviceData.physicalDeviceData.physicalDevice, &retrievedQueueFamilyPropertyCount, queueFamilyProperties.data()); // Fill array with queried queue family properties
        // Create device queue create infos
        std::Array<VkDeviceQueueCreateInfo> deviceQueueCreateInfos{retrievedQueueFamilyPropertyCount};
        for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
        {
            VkDeviceQueueCreateInfo &currentDeviceQueueCreateInfo = deviceQueueCreateInfos.at(i);
            currentDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            currentDeviceQueueCreateInfo.pNext = nullptr;
            currentDeviceQueueCreateInfo.flags = 0;
            currentDeviceQueueCreateInfo.queueFamilyIndex = i;
            currentDeviceQueueCreateInfo.queueCount = queueFamilyProperties.at(i).queueFamilyProperties.queueCount;
            currentDeviceQueueCreateInfo.pQueuePriorities = nullptr;
        }
        // Create device and load device fn pointers
        VkDeviceCreateInfo deviceInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                      .pNext = settings.deviceCIS.pNext,
                                      .flags = settings.deviceCIS.flags,
                                      .queueCreateInfoCount = retrievedQueueFamilyPropertyCount,
                                      .pQueueCreateInfos = deviceQueueCreateInfos.data(),
                                      .enabledLayerCount = settings.deviceCIS.layerCount,
                                      .ppEnabledLayerNames = settings.deviceCIS.layerNames.data(),
                                      .enabledExtensionCount = settings.deviceCIS.extensionCount,
                                      .ppEnabledExtensionNames = settings.deviceCIS.extensionNames.data(),
                                      .pEnabledFeatures = nullptr};
        vkCreateDevice(deviceData.physicalDeviceData.physicalDevice, &deviceInfo, nullptr, &deviceData.device);
        if (deviceData.device == 0)
        {
            ExitProcess(1); // Unable to create device, exit
        }
        volkLoadDevice(deviceData.device);
        // Create queue infos
        VkDeviceQueueInfo2 deviceQueueInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
                                           .pNext = nullptr,
                                           .flags = 0,
                                           .queueFamilyIndex = 0,
                                           .queueIndex = 0};
        // Get graphics queue
        u32 idealGraphicsQueueIndex = 0;
        for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
        {
            if (queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                deviceData.graphicsQueueFamilyIndex = i;
                deviceQueueInfo.flags = deviceQueueCreateInfos.at(i).flags;
                deviceQueueInfo.queueFamilyIndex = i;
                vkGetDeviceQueue2(deviceData.device, &deviceQueueInfo, &deviceData.graphicsQueue);
                break;
            }
        }
        // Get compute queue family indices
        for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
        {
            if (queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT && !(queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                deviceData.computeQueueFamilyIndex = i; // Ideal compute
                break;
            }
        }
        if (deviceData.computeQueueFamilyIndex == UINT_MAX)
        {
            for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
            {
                if (queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    deviceData.computeQueueFamilyIndex = i; // Fallback compute
                    break;
                }
            }
        }
        // Get transfer queue family indices
        for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
        {
            if (queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT && !(queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) && !(queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                deviceData.transferQueueFamilyIndex = i; // Ideal transfer
                break;
            }
        }
        if (deviceData.transferQueueFamilyIndex == UINT_MAX)
        {
            for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
            {
                if (queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT && !(queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT))
                {
                    deviceData.transferQueueFamilyIndex = i; // Secondary ideal transfer
                    break;
                }
            }
            if (deviceData.transferQueueFamilyIndex == UINT_MAX)
            {
                for (u32 i = 0; i < retrievedQueueFamilyPropertyCount; i++)
                {
                    if (queueFamilyProperties.at(i).queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
                    {
                        deviceData.transferQueueFamilyIndex = i; // Fallback transfer
                        break;
                    }
                }
            }
        }
        // Get compute and transfer queues
        const u32 maxGraphicsQueueCount = queueFamilyProperties.at(deviceData.graphicsQueueFamilyIndex).queueFamilyProperties.queueCount;
        const u32 maxComputeQueueCount = queueFamilyProperties.at(deviceData.computeQueueFamilyIndex).queueFamilyProperties.queueCount;
        const u32 maxTransferQueueCount = queueFamilyProperties.at(deviceData.transferQueueFamilyIndex).queueFamilyProperties.queueCount;
        deviceData.computeQueues.reset(maxComputeQueueCount);
        deviceData.transferQueues.reset(maxTransferQueueCount);
        u32 graphicsQueueIndexCounter = 1;
        u32 computeQueueIndexCounter = 0;
        u32 transferQueueIndexCounter = 0;
        VkQueue tempQueue = 0;
        if (deviceData.computeQueueFamilyIndex == deviceData.graphicsQueueFamilyIndex)
        {
            // Compute and graphics are same
            for (u32 i = 1; i < maxGraphicsQueueCount - 1; i++)
            {
                deviceQueueInfo.flags = deviceQueueCreateInfos.at(deviceData.computeQueueFamilyIndex).flags;
                deviceQueueInfo.queueFamilyIndex = deviceData.computeQueueFamilyIndex;
                deviceQueueInfo.queueIndex = i;
                vkGetDeviceQueue2(deviceData.device, &deviceQueueInfo, &tempQueue);
                deviceData.computeQueues.push_back(tempQueue);
                graphicsQueueIndexCounter++;
            }
            computeQueueIndexCounter = graphicsQueueIndexCounter;
        }
        else
        {
            // Compute is independant
            for (u32 i = 0; i < maxComputeQueueCount; i++)
            {
                deviceQueueInfo.flags = deviceQueueCreateInfos.at(deviceData.computeQueueFamilyIndex).flags;
                deviceQueueInfo.queueFamilyIndex = deviceData.computeQueueFamilyIndex;
                deviceQueueInfo.queueIndex = i;
                vkGetDeviceQueue2(deviceData.device, &deviceQueueInfo, &tempQueue);
                deviceData.computeQueues.push_back(tempQueue);
                computeQueueIndexCounter++;
            }
        }
        if (deviceData.transferQueueFamilyIndex == deviceData.graphicsQueueFamilyIndex)
        {
            // Transfer and graphics and compute are same
            for (u32 i = graphicsQueueIndexCounter; i < maxGraphicsQueueCount; i++)
            {
                deviceQueueInfo.flags = deviceQueueCreateInfos.at(deviceData.transferQueueFamilyIndex).flags;
                deviceQueueInfo.queueFamilyIndex = deviceData.transferQueueFamilyIndex;
                deviceQueueInfo.queueIndex = i;
                vkGetDeviceQueue2(deviceData.device, &deviceQueueInfo, &tempQueue);
                deviceData.transferQueues.push_back(tempQueue);
                graphicsQueueIndexCounter++;
            }
            transferQueueIndexCounter = graphicsQueueIndexCounter;
            computeQueueIndexCounter = graphicsQueueIndexCounter;
        }
        else if (deviceData.transferQueueFamilyIndex == deviceData.computeQueueFamilyIndex)
        {
            // Transfer and compute are same
            for (u32 i = computeQueueIndexCounter; i < maxComputeQueueCount; i++)
            {
                deviceQueueInfo.flags = deviceQueueCreateInfos.at(deviceData.transferQueueFamilyIndex).flags;
                deviceQueueInfo.queueFamilyIndex = deviceData.transferQueueFamilyIndex;
                deviceQueueInfo.queueIndex = i;
                vkGetDeviceQueue2(deviceData.device, &deviceQueueInfo, &tempQueue);
                deviceData.transferQueues.push_back(tempQueue);
                computeQueueIndexCounter++;
            }
            transferQueueIndexCounter = computeQueueIndexCounter;
        }
        else
        {
            // Transfer is independant
            for (u32 i = 0; i < maxTransferQueueCount; i++)
            {
                deviceQueueInfo.flags = deviceQueueCreateInfos.at(deviceData.transferQueueFamilyIndex).flags;
                deviceQueueInfo.queueFamilyIndex = deviceData.transferQueueFamilyIndex;
                deviceQueueInfo.queueIndex = i;
                vkGetDeviceQueue2(deviceData.device, &deviceQueueInfo, &tempQueue);
                deviceData.transferQueues.push_back(tempQueue);
                transferQueueIndexCounter++;
            }
        }
        if (deviceData.graphicsQueue == 0 || deviceData.computeQueues.size() == 0 || deviceData.transferQueues.size() == 0)
        {
            ExitProcess(1); // Unable to get required queues, exit
        }
    }

    void VulkanManager::createSurfaceData(const VKH::RenderSettings::SurfaceSettings &settings, u32 index)
    {
        RenderData::SurfaceData &data = renderData.back().surfaceData;
        // Create surface
        VkWin32SurfaceCreateInfoKHR winSurfaceInfo{.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                                                   .pNext = nullptr,
                                                   .flags = 0,
                                                   .hinstance = GetModuleHandle(0),
                                                   .hwnd = WM::WindowManager::windows.at(index).handle};
        vkCreateWin32SurfaceKHR(instanceData.instance, &winSurfaceInfo, nullptr, &data.surface);
        if (data.surface == 0)
        {
            ExitProcess(1);
        }
        VkBool32 surfaceSupported = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(deviceData.physicalDeviceData.physicalDevice, deviceData.graphicsQueueFamilyIndex, data.surface, &surfaceSupported);
        if (surfaceSupported == 0)
        {
            ExitProcess(1);
        }
        // Select surface format
        u32 retrievedSurfaceFormatCount = 0;
        VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
                                                    .pNext = settings.surfaceInfoSettings.pNext,
                                                    .surface = data.surface};
        vkGetPhysicalDeviceSurfaceFormats2KHR(deviceData.physicalDeviceData.physicalDevice, &surfaceInfo, &retrievedSurfaceFormatCount, nullptr);
        std::Array<VkSurfaceFormat2KHR> surfaceFormats(retrievedSurfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormats2KHR(deviceData.physicalDeviceData.physicalDevice, &surfaceInfo, &retrievedSurfaceFormatCount, surfaceFormats.data());
        b8 chosenFormat = false;
        for (u32 i = 0; i < surfaceFormats.size(); i++)
        {
            if (surfaceFormats.at(i).surfaceFormat.format == settings.surfaceFormatSettings.surfaceFormat.format && surfaceFormats.at(i).surfaceFormat.colorSpace == settings.surfaceFormatSettings.surfaceFormat.colorSpace)
            {
                data.surfaceFormat = surfaceFormats.at(i);
                chosenFormat = true;
                break;
            }
        }
        if (!chosenFormat)
        {
            ExitProcess(1);
        }
        // Select present mode
        u32 retrievedPresentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(deviceData.physicalDeviceData.physicalDevice, data.surface, &retrievedPresentModeCount, nullptr);
        std::Array<VkPresentModeKHR> presentModes(retrievedPresentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(deviceData.physicalDeviceData.physicalDevice, data.surface, &retrievedPresentModeCount, presentModes.data());
        b8 chosenMode = false;
        for (u32 i = 0; i < presentModes.size(); i++)
        {
            if (presentModes.at(i) == settings.presentModeSettings.presentMode)
            {
                data.presentMode = presentModes.at(i);
                chosenMode = true;
                break;
            }
        }
        if (!chosenMode)
        {
            ExitProcess(1);
        }
    }

    void VulkanManager::createSwapchainData(const VKH::RenderSettings::SwapchainSettings &settings, u32 index)
    {
        RenderData::SwapchainData &data = renderData.back().swapchainData;
        // Get current surface size
        RECT clientSize{};
        GetClientRect(WM::WindowManager::windows.at(index).handle, &clientSize);
        VkExtent2D extent{};
        extent.height = clientSize.bottom - clientSize.top;
        extent.width = clientSize.right - clientSize.left;
        // Create Swapchain
        VkSwapchainCreateInfoKHR swapchainInfo{.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                               .pNext = settings.swapchainCreateInfoSettings.pNext,
                                               .flags = settings.swapchainCreateInfoSettings.flags,
                                               .surface = renderData.at(index).surfaceData.surface,
                                               .minImageCount = SWAPCHAIN_IMAGE_COUNT,
                                               .imageFormat = renderData.at(index).surfaceData.surfaceFormat.surfaceFormat.format,
                                               .imageColorSpace = renderData.at(index).surfaceData.surfaceFormat.surfaceFormat.colorSpace,
                                               .imageExtent = extent,
                                               .imageArrayLayers = settings.swapchainCreateInfoSettings.imageArrayLayers,
                                               .imageUsage = settings.swapchainCreateInfoSettings.imageUsage,
                                               .imageSharingMode = settings.swapchainCreateInfoSettings.imageSharingMode,
                                               .queueFamilyIndexCount = settings.swapchainCreateInfoSettings.queueFamilyIndexCount,
                                               .pQueueFamilyIndices = settings.swapchainCreateInfoSettings.pQueueFamilyIndices.data(),
                                               .preTransform = settings.swapchainCreateInfoSettings.preTransform,
                                               .compositeAlpha = settings.swapchainCreateInfoSettings.compositeAlpha,
                                               .presentMode = renderData.at(index).surfaceData.presentMode,
                                               .clipped = settings.swapchainCreateInfoSettings.clipped,
                                               .oldSwapchain = settings.swapchainCreateInfoSettings.oldSwapchain};
        vkCreateSwapchainKHR(deviceData.device, &swapchainInfo, nullptr, &data.swapchain);
        if (data.swapchain == 0)
        {
            ExitProcess(1);
        }
        // Get swapchain images
        u32 swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(deviceData.device, data.swapchain, &swapchainImageCount, nullptr);
        vkGetSwapchainImagesKHR(deviceData.device, data.swapchain, &swapchainImageCount, data.swapchainImages);
        // Create swapchain image views
        VkImageViewUsageCreateInfo imageViewUsage{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                                  .pNext = nullptr,
                                                  .usage = settings.swapchainCreateInfoSettings.imageUsage};
        VkImageViewCreateInfo imageViewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                            .pNext = &imageViewUsage,
                                            .flags = settings.imageViewCreateSettings.flags,
                                            .image = 0,
                                            .viewType = settings.imageViewCreateSettings.viewType,
                                            .format = renderData.at(index).surfaceData.surfaceFormat.surfaceFormat.format,
                                            .components = settings.imageViewCreateSettings.components,
                                            .subresourceRange = settings.imageViewCreateSettings.subresourceRange};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            imageViewInfo.image = data.swapchainImages[i];
            vkCreateImageView(deviceData.device, &imageViewInfo, nullptr, &data.swapchainImageViews[i]);
        }
    }

    void VulkanManager::createSyncData(const VKH::RenderSettings::SyncSettings &settings, u32 index)
    {
        RenderData::SyncData &data = renderData.back().syncData;
        // Create acquire and render semaphores
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkSemaphoreCreateInfo semaphoreInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                                                .pNext = settings.semaphoreCreateSettings.pNext,
                                                .flags = 0};
            vkCreateSemaphore(deviceData.device, &semaphoreInfo, nullptr, &data.acquireSemaphores[i]);
            vkCreateSemaphore(deviceData.device, &semaphoreInfo, nullptr, &data.renderSemaphores[i]);
        }
        // Create render fences
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkFenceCreateInfo fenceInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                        .pNext = settings.fenceCreateSettings.pNext,
                                        .flags = settings.fenceCreateSettings.flags};
            vkCreateFence(deviceData.device, &fenceInfo, nullptr, &data.renderFences[i]);
        }
    }

    void VulkanManager::createCommandData(const VKH::RenderSettings::CommandSettings &settings, u32 index)
    {
        RenderData::CommandData &data = renderData.back().commandData;
        // Create per thread command pools
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkCommandPoolCreateInfo commandPoolInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                                    .pNext = nullptr,
                                                    .flags = settings.commandPoolCreateSettings.flags,
                                                    .queueFamilyIndex = settings.commandPoolCreateSettings.queueFamilyIndex};
            vkCreateCommandPool(deviceData.device, &commandPoolInfo, nullptr, &data.commandPools[i]);
        }
        // Create per pool command buffers
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkCommandBufferAllocateInfo commandBufferInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                                          .pNext = nullptr,
                                                          .commandPool = data.commandPools[i],
                                                          .level = settings.commandBufferAllocateSettings.level,
                                                          .commandBufferCount = 1};
            vkAllocateCommandBuffers(deviceData.device, &commandBufferInfo, &data.commandBuffer[i]);
        }
    }

    void VulkanManager::createWindow(const VKH::RenderSettings &settings, u32 windowSettingsIndex)
    {
        WM::WindowManager::create_window(windowSettingsIndex);
        const u32 winIndex = WM::WindowManager::windows.size() - 1;
        renderData.emplace_back();
        createSurfaceData(settings.surfaceSettings, winIndex);
        createSwapchainData(settings.swapchainSettings, winIndex);
        createSyncData(settings.syncSettings, winIndex);
        createCommandData(settings.commandSettings, winIndex);
    }

    void VulkanManager::destroySurfaceData(u32 index)
    {
        vkDestroySurfaceKHR(instanceData.instance, renderData.at(index).surfaceData.surface, nullptr);
    }

    void VulkanManager::destroySwapchainData(u32 index)
    {
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkDestroyImageView(deviceData.device, renderData.at(index).swapchainData.swapchainImageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(deviceData.device, renderData.at(index).swapchainData.swapchain, nullptr);
    }

    void VulkanManager::destroySyncData(u32 index)
    {
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkDestroySemaphore(deviceData.device, renderData.at(index).syncData.acquireSemaphores[i], nullptr);
            vkDestroySemaphore(deviceData.device, renderData.at(index).syncData.renderSemaphores[i], nullptr);
            vkDestroyFence(deviceData.device, renderData.at(index).syncData.renderFences[i], nullptr);
        }
    }

    void VulkanManager::destroyCommandData(u32 index)
    {
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkDestroyCommandPool(deviceData.device, renderData.at(index).commandData.commandPools[i], nullptr);
        }
    }

    void VulkanManager::removeWindow(HWND hwnd)
    {
        vkDeviceWaitIdle(deviceData.device);
        const u32 winIndex = WM::WindowManager::remove_window(hwnd);
        destroySyncData(winIndex);
        destroyCommandData(winIndex);
        destroySwapchainData(winIndex);
        destroySurfaceData(winIndex);
        if (winIndex != renderData.size() - 1)
        {
            renderData.replace_with_back(winIndex);
        }
        else
        {
            renderData.pop_back();
        }
    }

    void VulkanManager::queueRemoveWindow(HWND hwnd)
    {
        removeWindowQueue.push_back(hwnd);
    }
}