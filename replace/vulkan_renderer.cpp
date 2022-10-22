#include "vulkan_renderer.h"

namespace VKR
{
    RenderManager::RenderData::RenderData(VKD::VulkanData &vulkan, const VKH::RenderSettings &renderSettings, HWND hwnd) : vulkan(vulkanSettings), windowIndex(renderSettings.windowIndex), vsync(renderSettings.vsync)
    {
        create_surface(vulkan, hwnd);
        select_surface_format(vulkan, renderSettings.surfaceSettings.surfaceFormatSettings);
        select_present_mode(vulkan, renderSettings.surfaceSettings.presentModeSettings);
        create_swapchain(vulkan, renderSettings.swapchainSettings, hwnd);
        get_swapchain_images(vulkan);
        create_swapchain_image_views(vulkan, renderSettings.swapchainSettings.imageViewUsageSettings, renderSettings.swapchainSettings.imageViewSettings);
        create_semaphores(vulkan, renderSettings.semaphoreSettings);
        create_fences(vulkan, renderSettings.fenceSettings);
        create_command_pools(vulkan, renderSettings.commandPoolSettings);
        create_command_buffers(vulkan, renderSettings.commandBufferSettings);
    }

    RenderManager::RenderData::~RenderData(VKD::VulkanData &vulkan)
    {
        vkDeviceWaitIdle(vulkan.deviceData.device);
        for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
        {
            vkDestroySemaphore(vulkan.deviceData.device, syncData.acquireSemaphores[k], nullptr);
            vkDestroySemaphore(vulkan.deviceData.device, syncData.renderSemaphores[k], nullptr);
            vkDestroyFence(vulkan.deviceData.device, syncData.renderFences[k], nullptr);
        }
        for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
        {
            vkDestroyCommandPool(vulkan.deviceData.device, commandData.pools[k], nullptr);
        }
        empty_deletion_queue(vulkan.deviceData.device);
        for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
        {
            vkDestroyImageView(vulkan.deviceData.device, swapchainData.imageViews[k], nullptr);
        }
        vkDestroySwapchainKHR(vulkan.deviceData.device, swapchainData.swapchain, nullptr);
        vkDestroySurfaceKHR(vulkan.deviceData.device, surfaceData.surface, nullptr);
    }

    void RenderManager::RenderData::acquire()
    {
        VkResult wait = vkWaitForFences(vulkanCtx.device, 1, &renderFences[currentFrame], VK_TRUE, 0);
        if (wait != VK_SUCCESS)
        {
            return;
        }
        VkResult acquireRes = vkAcquireNextImageKHR(vulkanCtx.device, swapchain, 0, acquireSemaphores[currentFrame], 0, &swapchainImageIndices[currentFrame]);
        if (acquireRes == VK_SUBOPTIMAL_KHR || acquireRes == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RECT r{};
            GetClientRect(windowHandle, &r);
            resize({.width = (u32)(r.right - r.left), .height = (u32)(r.bottom - r.top)});
            acquireRes = vkAcquireNextImageKHR(vulkanCtx.device, swapchain, 0, acquireSemaphores[currentFrame], 0, &swapchainImageIndices[currentFrame]);
        }
        if (!(acquireRes == VK_SUCCESS || acquireRes == VK_SUBOPTIMAL_KHR))
        {
            return;
        }
        vkResetFences(vulkanCtx.device, 1, &renderFences[currentFrame]);
    }

    void RenderManager::RenderData::record()
    {
        for (u32 i = 0; i < data.presentData.validWindowCount; i++)
        {
            present.viewport.width = (f32)currentWindow.area.extent.width;
            present.viewport.height = (f32)currentWindow.area.extent.height;
            present.scissor.offset = currentWindow.area.offset;
            present.scissor.extent = currentWindow.area.extent;
            present.renderInfo.renderArea = currentWindow.area;
            present.renderAttachmentInfo.imageView = swapchainImageViews[currentFrame];
            present.beginImageBarrier.image = swapchainImages[currentFrame];
            present.endImageBarrier.image = swapchainImages[currentFrame];
            VkCommandBuffer &activeCommandBuffer = commandBuffers[currentFrame];
            vkResetCommandBuffer(activeCommandBuffer, 0);
            vkBeginCommandBuffer(activeCommandBuffer, &present.commandBufferBeginInfo);
            vkCmdSetViewport(activeCommandBuffer, 0, 1, &present.viewport);
            vkCmdSetScissor(activeCommandBuffer, 0, 1, &present.scissor);
            vkCmdPipelineBarrier(activeCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &present.beginImageBarrier);
            vkCmdBeginRendering(activeCommandBuffer, &present.renderInfo);
            vkCmdBindPipeline(activeCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
            vkCmdDraw(activeCommandBuffer, 3, 1, 0, 0);
            vkCmdEndRendering(activeCommandBuffer);
            vkCmdPipelineBarrier(activeCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &present.endImageBarrier);
            vkEndCommandBuffer(activeCommandBuffer);
        }
    }

    void RenderManager::RenderData::render()
    {
        acquire();
        record();
        vkQueueSubmit(vulkanCtx.graphicsQueue, 1, &submitBatches[currentFrame], renderFences[currentFrame]);
        VkResult presentRes = vkQueuePresentKHR(vulkanCtx.graphicsQueue, &presentBatches[currentFrame]);
        if (presentRes == VK_SUBOPTIMAL_KHR || presentRes == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RECT r{};
            GetClientRect(windowHandle, &r);
            resize({.width = (u32)(r.right - r.left), .height = (u32)(r.bottom - r.top)});
        }
        currentFrame = (currentFrame + 1) % SWAPCHAIN_IMAGE_COUNT;
        currentRenderTick += 1;
    }

    void RenderManager::RenderData::create_surface(VKD::VulkanData &vulkan, HWND hwnd)
    {
        // Create surface
        VkWin32SurfaceCreateInfoKHR surfaceInfo{.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                                                .pNext = nullptr,
                                                .flags = 0,
                                                .hinstance = GetModuleHandleW(0),
                                                .hwnd = hwnd};
        vkCreateWin32SurfaceKHR(vulkan.instanceData.instance, &surfaceInfo, nullptr, &surfaceData.surface);
        if (surfaceData.surface == 0)
        {
            ExitProcess(1);
        }
        // Check if surface supports display
        VkBool32 surfaceSupported = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(vulkan.deviceData.physicalDeviceData.physicalDevice, vulkan.deviceData.graphicsQueueFamilyIndex, surfaceData.surface, &surfaceSupported);
        if (surfaceSupported == 0)
        {
            ExitProcess(1);
        }
        // Acquire surface capabilities
        VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo2{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
                                                     .pNext = nullptr,
                                                     .surface = surfaceData.surface};
        vkGetPhysicalDeviceSurfaceCapabilities2KHR(vulkan.deviceData.physicalDeviceData.physicalDevice, &surfaceInfo2, &surfaceData.surfaceCapabilities);
    }

    void RenderManager::RenderData::select_surface_format(VKD::VulkanData &vulkan, RenderSettings::SurfaceSettings::SurfaceFormatSettings &surfaceFormatSettings)
    {
        // Acquire available surface formats
        u32 formatCount = 0;
        VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
                                                    .pNext = nullptr,
                                                    .surface = surfaceData.surface};
        vkGetPhysicalDeviceSurfaceFormats2KHR(vulkan.deviceData.physicalDeviceData.physicalDevice, &surfaceInfo, &formatCount, nullptr);
        cstd::LinAllocator<VkSurfaceFormat2KHR> currentFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormats2KHR(vulkan.deviceData.physicalDeviceData.physicalDevice, &surfaceInfo, &formatCount, currentFormats.data());
        // Select surface format
        u32 formatIndex = UINT_MAX;
        for (u32 i = 0; i < formatCount; i++)
        {
            if (currentFormats[i].surfaceFormat.format == surfaceFormatSettings.format && currentFormats[i].surfaceFormat.colorSpace == surfaceFormatSettings.colorSpace)
            {
                formatIndex = i;
                break;
            }
        }
        if (formatIndex == UINT_MAX)
        {
            ExitProcess(1);
        }
        surfaceData.surfaceFormat = currentFormats[formatIndex].surfaceFormat;
    }

    void RenderManager::RenderData::select_present_mode(VKD::VulkanData &vulkan, RenderSettings::SurfaceSettings::PresentModeSettings &presentModeSettings)
    {
        // Acquire available present modes
        u32 presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan.deviceData.physicalDeviceData.physicalDevice, surfaceData.surface, &presentModeCount, nullptr);
        cstd::LinAllocator<VkPresentModeKHR> currentPresentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan.deviceData.physicalDeviceData.physicalDevice, surfaceData.surface, &presentModeCount, currentPresentModes.data());
        // Select present mode
        u32 presentModeIndex = UINT_MAX;
        if (vsync)
        {
            for (u32 i = 0; i < presentModeCount; i++)
            {
                if (curentPresentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
                {
                    presentModeIndex = i;
                    break;
                }
            }
        }
        else
        {
            for (u32 i = 0; i < presentModeCount; i++)
            {
                if (currentPresentModes[i] = presentModeSettings.presentMode)
                {
                    presentModeIndex = i;
                    break;
                }
            }
        }
        if (presentModeIndex == UINT_MAX)
        {
            ExitProcess(1);
        }
        surfaceData.presentMode = currentPresentModes[presentModeIndex];
    }

    void RenderManager::RenderData::create_swapchain(VKD::VulkanData &vulkan, RenderSettings::SwapchainSettings &swapchainSettings, HWND hwnd)
    {
        // Acquire current window size
        RECT currentRect{};
        GetClientRect(hwnd, &currentRect);
        VkExtent2D currentExtent{};
        currentExtent.height = currentRect.bottom - currentRect.top;
        currentExtent.width = currentRect.right - currentRect.left;
        // Create swapchain
        VkSwapchainCreateInfoKHR swapchainInfo{.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                               .pNext = nullptr,
                                               .flags = swapchainSettings.flags,
                                               .surface = surfaceData.surface,
                                               .minImageCount = SWAPCHAIN_IMAGE_COUNT,
                                               .imageFormat = surfaceData.surfaceFormat.format,
                                               .imageColorSpace = surfaceData.surfaceFormat.colorSpace,
                                               .imageExtent = currentExtent,
                                               .imageArrayLayers = swapchainSettings.imageArrayLayers,
                                               .imageUsage = swapchainSettings.imageUsage,
                                               .imageSharingMode = swapchainSettings.imageSharingMode,
                                               .queueFamilyIndexCount = swapchainSettings.queueFamilyIndexCount,
                                               .pQueueFamilyIndices = swapchainSettings.queueFamilyIndices,
                                               .preTransform = swapchainSettings.preTransform,
                                               .compositeAlpha = swapchainSettings.compositeAlpha,
                                               .presentMode = surfaceData.presentMode,
                                               .clipped = swapchainSettings.clipped,
                                               .oldSwapchain = swapchainSettings.oldSwapchain};
        vkCreateSwapchainKHR(vulkan.deviceData.device, &swapchainInfo, nullptr, &swapchainData.swapchain);
        if (swapchainData.swapchain == 0)
        {
            ExitProcess(1);
        }
    }

    void RenderManager::RenderData::get_swapchain_images(VKD::VulkanData &vulkan)
    {
        // Acquire swapchain images
        u32 swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(vulkan.deviceData.device, swapchainData.swapchain, &swapchainImageCount, nullptr);
        vkGetSwapchainImagesKHR(vulkan.deviceData.device, swapchainData.swapchain, &swapchainImageCount, swapchainData.images);
    }

    void RenderManager::RenderData::create_swapchain_image_views(VKD::VulkanData &vulkan, RenderSettings::ImageViewUsageSettings &imageViewUsageSettings, RenderSettings::ImageViewSettings &imageViewSettings)
    {
        // Create swapchain image views
        VkImageViewUsageCreateInfo imageViewUsage{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO,
                                                  .pNext = nullptr,
                                                  .usage = imageViewUsageSettings.usage};
        VkImageViewCreateInfo imageViewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                            .pNext = &imageViewUsage,
                                            .flags = imageViewSettings.flags,
                                            .image = 0,
                                            .viewType = imageViewSettings.viewType,
                                            .format = surfaceFormat.format,
                                            .components = imageViewSettings.components,
                                            .subresourceRange = imageViewSettings.subresourceRange};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            imageViewInfo.image = swapchainData.images[i];
            vkCreateImageView(vulkan.deviceData.device, &imageViewInfo, nullptr, &swapchainData.imageViews[i]);
        }
    }

    void RenderManager::RenderData::flush_deletion_queue(VKD::VulkanData &vulkan)
    {
        vkDeviceWaitIdle(vulkan.deviceData.device);
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT * 2; i++)
        {
            if (i == emptyDeletionQueueIndex)
            {
                continue;
            }
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyImageView(vulkanCtx.device, swapchainDeletionQueue[i].swapchainImageViews[k], nullptr);
                swapchainDeletionQueue[i].swapchainImageViews[k] = 0;
            }
            vkDestroySwapchainKHR(vulkanCtx.device, swapchainDeletionQueue[i].swapchain, nullptr);
            swapchainDeletionQueue[i].swapchain = 0;
        }
    }

    void RenderManager::RenderData::destroy_deletion_queue(VKD::VulkanData &vulkan)
    {
        vkDeviceWaitIdle(vulkanCtx.device);
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT * 2; i++)
        {
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyImageView(vulkanCtx.device, swapchainDeletionQueue[i].swapchainImageViews[k], nullptr);
                swapchainDeletionQueue[i].swapchainImageViews[k] = 0;
            }
            vkDestroySwapchainKHR(vulkanCtx.device, swapchainDeletionQueue[i].swapchain, nullptr);
            swapchainDeletionQueue[i].swapchain = 0;
        }
    }

    void RenderManager::RenderData::resize(CH::UpdatedWindowInfo message)
    {
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            swapchainDeletionQueue[emptyDeletionQueueIndex].swapchainImageViews[i] = swapchainImageViews[i];
        }
        swapchainDeletionQueue[updatedWindow.emptyDeletionQueueIndex].swapchain = swapchain;
        VkExtent2D currentExtent{};
        currentExtent.width = message.width;
        currentExtent.height = message.height;
        SwapchainSettings &swapchainSettings = resizeSettings.swapchainSettings;
        VkSwapchainCreateInfoKHR swapchainInfo{.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                               .pNext = swapchainSettings.pNext,
                                               .flags = swapchainSettings.flags,
                                               .surface = surface,
                                               .minImageCount = SWAPCHAIN_IMAGE_COUNT,
                                               .imageFormat = surfaceFormat.format,
                                               .imageColorSpace = surfaceFormat.colorSpace,
                                               .imageExtent = currentExtent,
                                               .imageArrayLayers = swapchainSettings.imageArrayLayers,
                                               .imageUsage = swapchainSettings.imageUsage,
                                               .imageSharingMode = swapchainSettings.imageSharingMode,
                                               .queueFamilyIndexCount = swapchainSettings.queueFamilyIndexCount,
                                               .pQueueFamilyIndices = swapchainSettings.queueFamilyIndices,
                                               .preTransform = swapchainSettings.preTransform,
                                               .compositeAlpha = swapchainSettings.compositeAlpha,
                                               .presentMode = presentMode,
                                               .clipped = swapchainSettings.clipped,
                                               .oldSwapchain = swapchainDeletionQueue[emptyDeletionQueueIndex].swapchain};
        vkCreateSwapchainKHR(vulkanCtx.device, &swapchainInfo, nullptr, &swapchain);
        u32 swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(vulkanCtx.device, swapchain, &swapchainImageCount, nullptr);
        vkGetSwapchainImagesKHR(vulkanCtx.device, swapchain, &swapchainImageCount, swapchainImages);

        ImageViewUsageSettings &imageViewUsageSettings = resizeSettings.imageViewUsageSettings;
        VkImageViewUsageCreateInfo imageViewUsage{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO,
                                                  .pNext = imageViewUsageSettings.pNext,
                                                  .usage = imageViewUsageSettings.usage};

        ImageViewSettings &imageViewSettings = resizeSettings.imageViewSettings;
        VkImageViewCreateInfo imageViewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                            .pNext = &imageViewUsage,
                                            .flags = imageViewSettings.flags,
                                            .image = 0,
                                            .viewType = imageViewSettings.viewType,
                                            .format = surfaceFormat.format,
                                            .components = imageViewSettings.components,
                                            .subresourceRange = imageViewSettings.subresourceRange};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            imageViewInfo.image = swapchainImages[i];
            vkCreateImageView(vulkanCtx.device, &imageViewInfo, nullptr, &swapchainImageViews[i]);
        }
        flush_deletion_queue();
        emptyDeletionQueueIndex = (emptyDeletionQueueIndex + 1) % (SWAPCHAIN_IMAGE_COUNT * 2);
    }

    void RenderManager::RenderData::create_semaphores(VKD::VulkanData &vulkan, RenderSettings::SemaphoreSettings *semaphoreSettings)
    {
        // Create acquire and render semaphores
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkSemaphoreCreateInfo semaphoreInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                                                .pNext = semaphoreSettings[i].pNext,
                                                .flags = 0};
            vkCreateSemaphore(vulkan.deviceData.device, &semaphoreInfo, nullptr, &syncData.acquireSemaphores[i]);
            vkCreateSemaphore(vulkan.deviceData.device, &semaphoreInfo, nullptr, &syncData.renderSemaphores[i]);
        }
    }

    void RenderManager::RenderData::create_fences(VKD::VulkanData &vulkan, RenderSettings::FenceSettings *fenceSettings)
    {
        // Create render fences
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkFenceCreateInfo fenceInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                        .pNext = nullptr,
                                        .flags = fenceSettings[i].flags};
            vkCreateFence(vulkan.deviceData.device, &fenceInfo, nullptr, &syncData.renderFences[i]);
        }
    }

    void RenderManager::RenderData::create_command_pools(VKD::VulkanData &vulkan, RenderSettings::CommandPoolSettings *commandPoolSettings)
    {
        // Create per thread command pools
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkCommandPoolCreateInfo commandPoolInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                                    .pNext = nullptr,
                                                    .flags = commandPoolSettings[i].flags,
                                                    .queueFamilyIndex = commandPoolSettings[i].queueFamilyIndex};
            vkCreateCommandPool(vulkan.deviceData.device, &commandPoolInfo, nullptr, &commandData.pools[i]);
        }
    }

    void RenderManager::RenderData::create_command_buffers(VKD::VulkanData &vulkan, RenderSettings::CommandBufferSettings *commandBufferSettings)
    {
        // Create per pool command buffers
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            VkCommandBufferAllocateInfo commandBufferInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                                          .pNext = nullptr,
                                                          .commandPool = commandData.pools[i],
                                                          .level = commandBufferSettings[i].level,
                                                          .commandBufferCount = commandBufferSettings[i].count};
            vkAllocateCommandBuffers(vulkan.deviceData.device, &commandBufferInfo, &commandData.buffers[i]);
        }
    }

    RenderManager::RenderManager(VKH::VulkanSettings &vulkanSettings, WM::WindowManager::WindowSettings &windowSettings)
    {
        vulkan = VKD::VulkanData(vulkanSettings);
        windowManager = WM::WindowManager(windowSettings);
    }

    RenderManager::create_window(u32 settingsIndex)
    {
        HWND created = windowManager.create_window(settingsIndex);
        renderSurfaces.emplace_back(); // Push new render surface onto array
    }

    RenderManager::destroy_window(u32 index)
    {
        // Destroy window
        windowManager.remove_window(index);
    }

    RenderManager::destroy_extra()
    {
        // Destroy all but main window
        for (u32 i = 1; i < windowManager.windows.size(); i++)
        {
            windowManager.remove_window(i);
        }
    }

    void RenderManager::create_graphics_pipeline_layout(GraphicsPipelineLayoutSettings &pipelineLayoutSettings)
    {
        VkPipelineLayoutCreateInfo layoutInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                              .pNext = nullptr,
                                              .flags = pipelineLayoutSettings.flags,
                                              .setLayoutCount = pipelineLayoutSettings.setLayoutCount,
                                              .pSetLayouts = pipelineLayoutSettings.setLayouts,
                                              .pushConstantRangeCount = pipelineLayoutSettings.pushConstantRangeCount,
                                              .pPushConstantRanges = pipelineLayoutSettings.pushConstantRanges};
        vkCreatePipelineLayout(vulkanCtx.device, &layoutInfo, nullptr, &graphicsPipelineLayout);
        if (window.graphicsPipelineLayout == 0)
        {
            ExitProcess(1);
        }
    }

    void RenderManager::create_graphics_pipeline(TotalGraphicsPipelineSettings &graphicsPipelineSettings)
    {
        PipelineRenderingSettings &renderingSettings = graphicsPipelineSettings.renderingSettings;
        VkPipelineRenderingCreateInfo renderInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                                                 .pNext = renderingSettings.pNext,
                                                 .viewMask = renderingSettings.viewMask,
                                                 .colorAttachmentCount = renderingSettings.colorAttachmentCount,
                                                 .pColorAttachmentFormats = renderingSettings.colorAttachmentFormats,
                                                 .depthAttachmentFormat = renderingSettings.deptchAttachmentFormat,
                                                 .stencilAttachmentFormat = renderingSettings.stencilAttachmentFormat};

        PipelineShaderStageSettings *shaderStageSettings = graphicsPipelineSettings.shaderStageSettings;
        VkPipelineShaderStageCreateInfo shaderStageInfo[14]{};
        for (u32 i = 0; i < graphicsPipelineSettings.graphicsPipelineSettings.stageCount; i++)
        {
            shaderStageInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo[i].pNext = shaderStageSettings[i].pNext;
            shaderStageInfo[i].flags = shaderStageSettings[i].flags;
            shaderStageInfo[i].stage = shaderStageSettings[i].stage;
            shaderStageInfo[i].module = shaderStageSettings[i].shaderModule;
            shaderStageInfo[i].pName = shaderStageSettings[i].name;
            shaderStageInfo[i].pSpecializationInfo = shaderStageSettings[i].specializationInfo;
        };

        PipelineVertexInputSettings &vertexInputSettings = graphicsPipelineSettings.vertexInputSettings;
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                                                             .pNext = vertexInputSettings.pNext,
                                                             .flags = vertexInputSettings.flags,
                                                             .vertexBindingDescriptionCount = vertexInputSettings.vertexBindingDescriptionCount,
                                                             .pVertexBindingDescriptions = vertexInputSettings.vertexBindingDescriptions,
                                                             .vertexAttributeDescriptionCount = vertexInputSettings.vertexAttributeDescriptionCount,
                                                             .pVertexAttributeDescriptions = vertexInputSettings.vertexAttributeDescriptions};

        PipelineInputAssemblySettings &inputAssemblySettings = graphicsPipelineSettings.inputAssemblySettings;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                                                                 .pNext = nullptr,
                                                                 .flags = 0,
                                                                 .topology = inputAssemblySettings.topology,
                                                                 .primitiveRestartEnable = inputAssemblySettings.primitiveRestartEnable};

        PipelineTessellationSettings &tessellationSettings = graphicsPipelineSettings.tessellationSettings;
        VkPipelineTessellationStateCreateInfo tessellationInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
                                                               .pNext = tessellationSettings.pNext,
                                                               .flags = 0,
                                                               .patchControlPoints = tessellationSettings.patchControlPoints};

        PipelineRasterizationSettings &rasterizationSettings = graphicsPipelineSettings.rasterizationSettings;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                                                                 .pNext = rasterizationSettings.pNext,
                                                                 .flags = 0,
                                                                 .depthClampEnable = rasterizationSettings.depthClampEnable,
                                                                 .rasterizerDiscardEnable = rasterizationSettings.rasterizerDiscardEnable,
                                                                 .polygonMode = rasterizationSettings.polygonMode,
                                                                 .cullMode = rasterizationSettings.cullMode,
                                                                 .frontFace = rasterizationSettings.frontFace,
                                                                 .depthBiasEnable = rasterizationSettings.depthBiasEnable,
                                                                 .depthBiasConstantFactor = rasterizationSettings.depthBiasConstantFactor,
                                                                 .depthBiasClamp = rasterizationSettings.depthBiasClamp,
                                                                 .depthBiasSlopeFactor = rasterizationSettings.depthBiasSlopeFactor,
                                                                 .lineWidth = rasterizationSettings.lineWidth};

        PipelineMultisampleSettings &multisampleSettings = graphicsPipelineSettings.multisampleSettings;
        VkPipelineMultisampleStateCreateInfo multisampleInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                                                             .pNext = multisampleSettings.pNext,
                                                             .flags = 0,
                                                             .rasterizationSamples = multisampleSettings.rasterizationSamples,
                                                             .sampleShadingEnable = multisampleSettings.sampleShadingEnable,
                                                             .minSampleShading = multisampleSettings.minSampleShading,
                                                             .pSampleMask = multisampleSettings.sampleMask,
                                                             .alphaToCoverageEnable = multisampleSettings.alphaToCoverageEnable,
                                                             .alphaToOneEnable = multisampleSettings.alphaToOneEnable};

        PipelineDepthStencilSettings &depthStencilSettings = graphicsPipelineSettings.depthStencilSettings;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                                                               .pNext = nullptr,
                                                               .flags = depthStencilSettings.flags,
                                                               .depthTestEnable = depthStencilSettings.depthTestEnable,
                                                               .depthWriteEnable = depthStencilSettings.depthWriteEnable,
                                                               .depthCompareOp = depthStencilSettings.depthCompareOp,
                                                               .depthBoundsTestEnable = depthStencilSettings.depthBoundsTestEnable,
                                                               .stencilTestEnable = depthStencilSettings.stencilTestEnable,
                                                               .front = depthStencilSettings.front,
                                                               .back = depthStencilSettings.back,
                                                               .minDepthBounds = depthStencilSettings.minDepthBounds,
                                                               .maxDepthBounds = depthStencilSettings.maxDepthBounds};

        PipelineColorBlendSettings &colorBlendSettings = graphicsPipelineSettings.colorBlendSettings;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                                                           .pNext = colorBlendSettings.pNext,
                                                           .flags = colorBlendSettings.flags,
                                                           .logicOpEnable = colorBlendSettings.logicOpEnable,
                                                           .logicOp = colorBlendSettings.logicOp,
                                                           .attachmentCount = colorBlendSettings.attachmentCount,
                                                           .pAttachments = colorBlendSettings.attachments,
                                                           .blendConstants = colorBlendSettings.blendConstants};

        PipelineViewportSettings &viewportSettings = graphicsPipelineSettings.viewportSettings;
        VkPipelineViewportStateCreateInfo viewportInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                                                       .pNext = viewportSettings.pNext,
                                                       .flags = 0,
                                                       .viewportCount = viewportSettings.viewportCount,
                                                       .pViewports = viewportSettings.viewports,
                                                       .scissorCount = viewportSettings.scissorCount,
                                                       .pScissors = viewportSettings.scissors};

        PipelineDynamicStateSettings &dynamicStateSettings = graphicsPipelineSettings.dynamicStateSettings;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                                                          .pNext = nullptr,
                                                          .flags = 0,
                                                          .dynamicStateCount = dynamicStateSettings.dynamicSateCount,
                                                          .pDynamicStates = dynamicStateSettings.dynamicStates};

        GraphicsPipelineSettings &gps = graphicsPipelineSettings.graphicsPipelineSettings;
        VkGraphicsPipelineCreateInfo graphicsPipelineInfo{.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                                                          .pNext = &renderInfo,
                                                          .flags = gps.flags,
                                                          .stageCount = gps.stageCount,
                                                          .pStages = shaderStageInfo,
                                                          .pVertexInputState = &vertexInputInfo,
                                                          .pInputAssemblyState = &inputAssemblyInfo,
                                                          .pTessellationState = &tessellationInfo,
                                                          .pViewportState = &viewportInfo,
                                                          .pRasterizationState = &rasterizationInfo,
                                                          .pMultisampleState = &multisampleInfo,
                                                          .pDepthStencilState = &depthStencilInfo,
                                                          .pColorBlendState = &colorBlendInfo,
                                                          .pDynamicState = &dynamicStateInfo,
                                                          .layout = graphicsPipelineLayout,
                                                          .renderPass = nullptr,
                                                          .subpass = 0,
                                                          .basePipelineHandle = gps.basePipelineHandle,
                                                          .basePipelineIndex = gps.basePipelineIndex};
        vkCreateGraphicsPipelines(vulkanCtx.device, 0, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline);
        if (graphicsPipeline == 0)
        {
            ExitProcess(1);
        }
    }

    void RenderManager::load_shader_module(ShaderModuleSettings &shaderModuleSettings, PCWSTR fileName, VkShaderModule &shaderModule)
    {
        HANDLE file = CreateFileW(fileName, FILE_GENERIC_READ, 1, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (file == INVALID_HANDLE_VALUE)
        {
            ExitProcess(1);
        }
        u32 fileSize = GetFileSize(file, nullptr);
        if (fileSize == 0)
        {
            ExitProcess(1);
        }
        u32 outputBuffer[50000]{};
        DWORD bytesRead = 0;
        ReadFile(file, outputBuffer, fileSize, &bytesRead, nullptr);
        VkShaderModuleCreateInfo shaderInfo{.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                                            .pNext = shaderModuleSettings.pNext,
                                            .flags = 0,
                                            .codeSize = fileSize,
                                            .pCode = outputBuffer};
        vkCreateShaderModule(vulkanCtx.device, &shaderInfo, nullptr, &shaderModule);
        if (shaderModule == 0)
        {
            ExitProcess(1);
        }
    }
}