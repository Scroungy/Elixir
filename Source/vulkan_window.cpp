#include "vulkan_window.h"

namespace VW
{
    void initialize(VkDevice device, VulkanWindows &data)
    {
        create_fences(device, data);
        PresentData &present = data.presentData;
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            PresentDataSOA &soa = present.soa[i];
            // Initialize soa data
            for (u32 k = 0; k < MAX_WINDOWS; k++)
            {
                soa.waitStageMasksDyn[k] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            }
            present.submitBatches[i].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            present.submitBatches[i].pWaitDstStageMask = soa.waitStageMasksDyn;
            present.submitBatches[i].pWaitSemaphores = soa.acquireNextImageSemaphoresDyn;
            present.submitBatches[i].pCommandBuffers = soa.commandBuffersDyn;
            present.submitBatches[i].pSignalSemaphores = soa.renderSemaphoresDyn;
            present.presentBatches[i].sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present.presentBatches[i].pWaitSemaphores = soa.renderSemaphoresDyn;
            present.presentBatches[i].pSwapchains = soa.swapchainsDyn;
            present.presentBatches[i].pImageIndices = soa.swapchainImageIndicesDyn;
        }
        // Initialize present data of max windows size
        for (u32 i = 0; i < MAX_WINDOWS; i++)
        {
            present.commandBufferBeginInfo[i] = {.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
            present.viewport[i] = {.maxDepth = 1.0f};
            present.renderAttachmentInfo[i] = {.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO, .imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR, .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, .storeOp = VK_ATTACHMENT_STORE_OP_STORE, .clearValue = present.clearValue[i]};
            present.renderInfo[i] = {.sType = VK_STRUCTURE_TYPE_RENDERING_INFO, .layerCount = 1, .colorAttachmentCount = 1, .pColorAttachments = &present.renderAttachmentInfo[i]};
            present.beginImageBarrier[i] = {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED, .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}};
            present.endImageBarrier[i] = {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}};
        }
    }

    void destroy(VkDevice device, VulkanWindows &data)
    {
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkDestroyFence(device, data.renderFences[i], nullptr);
        }
    }

    void acquire(VkDevice device, VulkanWindows &data)
    {
        VkResult wait = vkWaitForFences(device, 1, &data.renderFences[data.presentData.currentFrame], VK_TRUE, 0);
        if (wait != VK_SUCCESS)
        {
            return;
        }
        u32 currentFrame = data.presentData.currentFrame;
        for (u32 i = 0; i < data.emptyWindowIndex; i++)
        {
            WindowData &currentWindow = data.windows[i];
            VkResult acquireRes = vkAcquireNextImageKHR(device, currentWindow.swapchain, 0, currentWindow.acquireNextImageSemaphores[currentWindow.currentFrame], 0, &currentWindow.swapchainImageIndices[currentWindow.currentFrame]);
            if (acquireRes == VK_SUBOPTIMAL_KHR || acquireRes == VK_ERROR_OUT_OF_DATE_KHR)
            {
                RECT r{};
                GetClientRect(currentWindow.windowHandle, &r);
                resize(device, data, {.width = (u32)(r.right - r.left), .height = (u32)(r.bottom - r.top), .windowIndex = i});
                acquireRes = vkAcquireNextImageKHR(device, currentWindow.swapchain, 0, currentWindow.acquireNextImageSemaphores[currentWindow.currentFrame], 0, &currentWindow.swapchainImageIndices[currentWindow.currentFrame]);
            }
            if (acquireRes == VK_SUCCESS || acquireRes == VK_SUBOPTIMAL_KHR)
            {
                data.presentData.validWindows[data.presentData.validWindowCount] = i;
                data.presentData.validWindowCount += 1;
            }
        }
        if (data.presentData.validWindowCount == 0)
        {
            return;
        }
        u32 validWindowCount = data.presentData.validWindowCount;
        data.presentData.submitBatches[currentFrame].waitSemaphoreCount = validWindowCount;
        data.presentData.submitBatches[currentFrame].commandBufferCount = validWindowCount;
        data.presentData.submitBatches[currentFrame].signalSemaphoreCount = validWindowCount;
        data.presentData.presentBatches[currentFrame].waitSemaphoreCount = validWindowCount;
        data.presentData.presentBatches[currentFrame].swapchainCount = validWindowCount;
        vkResetFences(device, 1, &data.renderFences[currentFrame]);
    }

    void record(VulkanWindows &data)
    {
        for (u32 i = 0; i < data.presentData.validWindowCount; i++)
        {
            u32 currentWindowIndex = data.presentData.validWindows[i];
            u32 currentFrame = data.presentData.currentFrame;
            WindowData &currentWindow = data.windows[currentWindowIndex];
            PresentData &present = data.presentData;
            present.viewport[currentWindowIndex].width = (f32)currentWindow.area.extent.width;
            present.viewport[currentWindowIndex].height = (f32)currentWindow.area.extent.height;
            present.scissor[currentWindowIndex].offset = currentWindow.area.offset;
            present.scissor[currentWindowIndex].extent = currentWindow.area.extent;
            present.renderInfo[currentWindowIndex].renderArea = currentWindow.area;
            present.renderAttachmentInfo[currentWindowIndex].imageView = currentWindow.swapchainImageViews[currentWindow.currentFrame];
            present.beginImageBarrier[currentWindowIndex].image = currentWindow.swapchainImages[currentWindow.currentFrame];
            present.endImageBarrier[currentWindowIndex].image = currentWindow.swapchainImages[currentWindow.currentFrame];
            VkCommandBuffer &activeCommandBuffer = currentWindow.commandBuffers[currentWindow.currentFrame];
            vkResetCommandBuffer(activeCommandBuffer, 0);
            vkBeginCommandBuffer(activeCommandBuffer, &present.commandBufferBeginInfo[currentWindowIndex]);
            vkCmdSetViewport(activeCommandBuffer, 0, 1, &present.viewport[currentWindowIndex]);
            vkCmdSetScissor(activeCommandBuffer, 0, 1, &present.scissor[currentWindowIndex]);
            vkCmdPipelineBarrier(activeCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &present.beginImageBarrier[currentWindowIndex]);
            vkCmdBeginRendering(activeCommandBuffer, &present.renderInfo[currentWindowIndex]);
            vkCmdBindPipeline(activeCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, currentWindow.graphicsPipeline);
            vkCmdDraw(activeCommandBuffer, 3, 1, 0, 0);
            vkCmdEndRendering(activeCommandBuffer);
            vkCmdPipelineBarrier(activeCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &present.endImageBarrier[currentWindowIndex]);
            vkEndCommandBuffer(activeCommandBuffer);
        }
    }

    void map_to_soa(VulkanWindows &data)
    {
        for (u32 i = 0; i < data.presentData.validWindowCount; i++)
        {
            WindowData &currentWindow = data.windows[data.presentData.validWindows[i]];
            u32 currentFrame = data.presentData.currentFrame;
            PresentDataSOA &soa = data.presentData.soa[currentFrame];
            soa.swapchainsDyn[i] = currentWindow.swapchain;
            soa.swapchainImageIndicesDyn[i] = currentWindow.swapchainImageIndices[currentWindow.currentFrame];
            soa.acquireNextImageSemaphoresDyn[i] = currentWindow.acquireNextImageSemaphores[currentWindow.currentFrame];
            soa.renderSemaphoresDyn[i] = currentWindow.renderSemaphores[currentWindow.currentFrame];
            soa.commandBuffersDyn[i] = currentWindow.commandBuffers[currentWindow.currentFrame];
        }
    }

    void render(VkDevice device, VulkanWindows &data, VkQueue graphicsQueue)
    {
        data.presentData.validWindowCount = 0;
        acquire(device, data);
        if (data.presentData.validWindowCount == 0)
        {
            return;
        }
        record(data);
        map_to_soa(data);
        vkQueueSubmit(graphicsQueue, 1, &data.presentData.submitBatches[data.presentData.currentFrame], data.renderFences[data.presentData.currentFrame]);
        VkResult presentRes = vkQueuePresentKHR(graphicsQueue, &data.presentData.presentBatches[data.presentData.currentFrame]);
        // if (presentRes == VK_SUBOPTIMAL_KHR || presentRes == VK_ERROR_OUT_OF_DATE_KHR)
        // {
        //     for (u32 i = 0; i < data.emptyWindowIndex; i++)
        //     {
        //         RECT r{};
        //         GetClientRect(data.windows[i].windowHandle, &r);
        //         resize(data, {.width = (u32)(r.right - r.left), .height = (u32)(r.bottom - r.top), .windowIndex = i});
        //     }
        // }
        data.presentData.currentFrame = (data.presentData.currentFrame + 1) % SWAPCHAIN_IMAGE_COUNT;
        for (u32 i = 0; i < data.emptyWindowIndex; i++)
        {
            data.windows[i].currentFrame = (data.windows[i].currentFrame + 1) % SWAPCHAIN_IMAGE_COUNT;
        }
        data.presentData.currentRenderTick += 1;
    }

    bool is_empty(VulkanWindows &data)
    {
        if (data.emptyWindowIndex == 0)
        {
            return true;
        }
        return false;
    }

    void create_window(VB::VulkanBase &base, VulkanWindows &data, CH::ConstructedWindowInfo message)
    {
        WindowData &currentWindow = data.windows[message.windowIndex];
        currentWindow.vsync = message.vsync;
        currentWindow.area.extent.width = message.width;
        currentWindow.area.extent.height = message.height;
        currentWindow.windowHandle = message.hwnd;
        create_surface(base, currentWindow, message.hinstance, message.hwnd);
        select_surface_format(base.graphicsCard, currentWindow);
        select_present_mode(base.graphicsCard, currentWindow);
        create_swapchain(base.device, currentWindow);
        get_swapchain_images(base.device, currentWindow);
        create_swapchain_image_views(base.device, currentWindow);
        create_graphics_pipeline_layout(base.device, currentWindow);
        create_graphics_pipeline(base.device, currentWindow);
        create_command_pools(base.device, currentWindow, base.queueFamilyIndices[0]);
        create_command_buffers(base.device, currentWindow);
        create_semaphores(base.device, currentWindow);
        data.emptyWindowIndex += 1;
    }

    void create_surface(VB::VulkanBase &data, WindowData &window, HINSTANCE instance, HWND hwnd)
    {
        VkWin32SurfaceCreateInfoKHR surfaceInfo{.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                                                .pNext = nullptr,
                                                .flags = 0,
                                                .hinstance = instance,
                                                .hwnd = hwnd};
        VkBool32 surfaceSupported = 0;
        vkCreateWin32SurfaceKHR(data.instance, &surfaceInfo, nullptr, &window.surface);
        vkGetPhysicalDeviceSurfaceSupportKHR(data.graphicsCard, data.queueFamilyIndices[0], window.surface, &surfaceSupported);
        if (surfaceSupported == 0 || window.surface == 0)
        {
            ExitProcess(1);
        }
        VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo2{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
                                                     .pNext = nullptr,
                                                     .surface = window.surface};
        vkGetPhysicalDeviceSurfaceCapabilities2KHR(data.graphicsCard, &surfaceInfo2, &data.deviceCapabilities);
    }

    void select_surface_format(VkPhysicalDevice graphicsCard, WindowData &window)
    {
        u32 formatCount = 0;
        VkSurfaceFormat2KHR currentFormats[256]{};
        VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR,
                                                    .pNext = nullptr,
                                                    .surface = window.surface};
        for (u32 i = 0; i < 256; i++)
        {
            currentFormats[i].sType = VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR;
        }
        vkGetPhysicalDeviceSurfaceFormats2KHR(graphicsCard, &surfaceInfo, &formatCount, nullptr);
        vkGetPhysicalDeviceSurfaceFormats2KHR(graphicsCard, &surfaceInfo, &formatCount, currentFormats);
        u32 formatIndex = UINT_MAX;
        for (u32 i = 0; i < formatCount; i++)
        {
            if (currentFormats[i].surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && currentFormats[i].surfaceFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            {
                formatIndex = i;
                break;
            }
        }
        if (formatIndex == UINT_MAX)
        {
            ExitProcess(1);
        }
        window.surfaceFormat = currentFormats[formatIndex].surfaceFormat;
    }

    void select_present_mode(VkPhysicalDevice graphicsCard, WindowData &window)
    {
        u32 presentModeCount = 0;
        VkPresentModeKHR currentPresentModes[256]{};
        vkGetPhysicalDeviceSurfacePresentModesKHR(graphicsCard, window.surface, &presentModeCount, nullptr);
        vkGetPhysicalDeviceSurfacePresentModesKHR(graphicsCard, window.surface, &presentModeCount, currentPresentModes);
        u32 presentModeIndex = UINT_MAX;
        for (u32 i = 0; i < presentModeCount; i++)
        {
            if (window.vsync)
            {
                if (currentPresentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
                {
                    presentModeIndex = i;
                    break;
                }
            }
            else
            {
                if (currentPresentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
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
        window.presentMode = currentPresentModes[presentModeIndex];
    }

    void create_swapchain(VkDevice device, WindowData &window)
    {
        VkSwapchainCreateInfoKHR swapchainInfo{.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                               .pNext = nullptr,
                                               .flags = 0,
                                               .surface = window.surface,
                                               .minImageCount = SWAPCHAIN_IMAGE_COUNT,
                                               .imageFormat = window.surfaceFormat.format,
                                               .imageColorSpace = window.surfaceFormat.colorSpace,
                                               .imageExtent = window.area.extent,
                                               .imageArrayLayers = 1,
                                               .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                               .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                               .queueFamilyIndexCount = 0,
                                               .pQueueFamilyIndices = nullptr,
                                               .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                               .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                               .presentMode = window.presentMode,
                                               .clipped = VK_TRUE,
                                               .oldSwapchain = 0};
        vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &window.swapchain);
        if (window.swapchain == 0)
        {
            ExitProcess(1);
        }
    }

    void get_swapchain_images(VkDevice device, WindowData &window)
    {
        u32 swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(device, window.swapchain, &swapchainImageCount, nullptr);
        vkGetSwapchainImagesKHR(device, window.swapchain, &swapchainImageCount, window.swapchainImages);
    }

    void create_swapchain_image_views(VkDevice device, WindowData &window)
    {
        VkImageViewUsageCreateInfo imageViewUsage{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO,
                                                  .pNext = nullptr,
                                                  .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT};
        VkImageViewCreateInfo imageViewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                            .pNext = &imageViewUsage,
                                            .flags = 0,
                                            .image = 0,
                                            .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                            .format = window.surfaceFormat.format,
                                            .components = {
                                                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                .a = VK_COMPONENT_SWIZZLE_IDENTITY},
                                            .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            imageViewInfo.image = window.swapchainImages[i];
            vkCreateImageView(device, &imageViewInfo, nullptr, &window.swapchainImageViews[i]);
        }
    }

    void create_graphics_pipeline_layout(VkDevice device, WindowData &window)
    {
        VkPipelineLayoutCreateInfo layoutInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                              .pNext = nullptr,
                                              .flags = 0,
                                              .setLayoutCount = 0,
                                              .pSetLayouts = nullptr,
                                              .pushConstantRangeCount = 0,
                                              .pPushConstantRanges = nullptr};
        vkCreatePipelineLayout(device, &layoutInfo, nullptr, &window.graphicsPipelineLayout);
        if (window.graphicsPipelineLayout == 0)
        {
            ExitProcess(1);
        }
    }

    void create_graphics_pipeline(VkDevice device, WindowData &window)
    {
        VkShaderModule vertexShaderModule{};
        VkShaderModule fragmentShaderModule{};
        load_shader_module(device, L"vert.spv", vertexShaderModule);
        load_shader_module(device, L"frag.spv", fragmentShaderModule);

        VkPipelineRenderingCreateInfo renderInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                                                 .colorAttachmentCount = 1,
                                                 .pColorAttachmentFormats = &window.surfaceFormat.format};

        VkPipelineShaderStageCreateInfo shaderStageInfo[2] = {{.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                               .pNext = nullptr,
                                                               .flags = 0,
                                                               .stage = VK_SHADER_STAGE_VERTEX_BIT,
                                                               .module = vertexShaderModule,
                                                               .pName = "main",
                                                               .pSpecializationInfo = nullptr},
                                                              {.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                               .pNext = nullptr,
                                                               .flags = 0,
                                                               .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                               .module = fragmentShaderModule,
                                                               .pName = "main",
                                                               .pSpecializationInfo = nullptr}};
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                                                             .pNext = nullptr,
                                                             .flags = 0,
                                                             .vertexBindingDescriptionCount = 0,
                                                             .pVertexBindingDescriptions = nullptr,
                                                             .vertexAttributeDescriptionCount = 0,
                                                             .pVertexAttributeDescriptions = nullptr};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                                                                 .pNext = nullptr,
                                                                 .flags = 0,
                                                                 .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                                                                 .primitiveRestartEnable = VK_FALSE};
        VkPipelineRasterizationStateCreateInfo rasterizationInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                                                                 .pNext = nullptr,
                                                                 .flags = 0,
                                                                 .depthClampEnable = VK_FALSE,
                                                                 .rasterizerDiscardEnable = VK_FALSE,
                                                                 .polygonMode = VK_POLYGON_MODE_FILL,
                                                                 .cullMode = VK_CULL_MODE_BACK_BIT,
                                                                 .frontFace = VK_FRONT_FACE_CLOCKWISE,
                                                                 .depthBiasEnable = VK_FALSE,
                                                                 .depthBiasConstantFactor = 0.0f,
                                                                 .depthBiasClamp = 0.0f,
                                                                 .depthBiasSlopeFactor = 0.0f,
                                                                 .lineWidth = 1.0f};
        VkPipelineMultisampleStateCreateInfo multisampleInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                                                             .pNext = nullptr,
                                                             .flags = 0,
                                                             .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                                                             .sampleShadingEnable = VK_FALSE,
                                                             .minSampleShading = 0.0f,
                                                             .pSampleMask = nullptr,
                                                             .alphaToCoverageEnable = VK_FALSE,
                                                             .alphaToOneEnable = VK_FALSE};
        VkPipelineColorBlendAttachmentState colorAttachment{.blendEnable = VK_FALSE,
                                                            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                                                            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE,
                                                            .colorBlendOp = VK_BLEND_OP_ADD,
                                                            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                                                            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                                                            .alphaBlendOp = VK_BLEND_OP_ADD,
                                                            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
        VkPipelineColorBlendStateCreateInfo colorBlendInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                                                           .pNext = nullptr,
                                                           .flags = 0,
                                                           .logicOpEnable = VK_FALSE,
                                                           .logicOp = VK_LOGIC_OP_COPY,
                                                           .attachmentCount = 1,
                                                           .pAttachments = &colorAttachment,
                                                           .blendConstants = {1.0f, 1.0f, 1.0f, 1.0f}};
        VkPipelineViewportStateCreateInfo viewportInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                                                       .pNext = nullptr,
                                                       .flags = 0,
                                                       .viewportCount = 1,
                                                       .pViewports = nullptr,
                                                       .scissorCount = 1,
                                                       .pScissors = nullptr};
        VkDynamicState dynamicStates[2]{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicStateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                                                          .pNext = nullptr,
                                                          .flags = 0,
                                                          .dynamicStateCount = 2,
                                                          .pDynamicStates = dynamicStates};
        VkGraphicsPipelineCreateInfo graphicsPipelineInfo{.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                                                          .pNext = &renderInfo,
                                                          .flags = 0,
                                                          .stageCount = 2,
                                                          .pStages = shaderStageInfo,
                                                          .pVertexInputState = &vertexInputInfo,
                                                          .pInputAssemblyState = &inputAssemblyInfo,
                                                          .pTessellationState = nullptr,
                                                          .pViewportState = &viewportInfo,
                                                          .pRasterizationState = &rasterizationInfo,
                                                          .pMultisampleState = &multisampleInfo,
                                                          .pDepthStencilState = nullptr,
                                                          .pColorBlendState = &colorBlendInfo,
                                                          .pDynamicState = &dynamicStateInfo,
                                                          .layout = window.graphicsPipelineLayout,
                                                          .renderPass = nullptr,
                                                          .subpass = 0,
                                                          .basePipelineHandle = 0,
                                                          .basePipelineIndex = 0};
        vkCreateGraphicsPipelines(device, 0, 1, &graphicsPipelineInfo, nullptr, &window.graphicsPipeline);
        vkDestroyShaderModule(device, vertexShaderModule, nullptr);
        vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
        if (window.graphicsPipeline == 0)
        {
            ExitProcess(1);
        }
    }

    void create_command_pools(VkDevice device, WindowData &window, u32 queueFamilyIndex)
    {
        VkCommandPoolCreateInfo commandPoolInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                                .pNext = nullptr,
                                                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                                .queueFamilyIndex = queueFamilyIndex};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkCreateCommandPool(device, &commandPoolInfo, nullptr, &window.commandPools[i]);
        }
    }

    void create_command_buffers(VkDevice device, WindowData &window)
    {
        VkCommandBufferAllocateInfo commandBufferInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                                                      .pNext = nullptr,
                                                      .commandPool = 0,
                                                      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                                      .commandBufferCount = 1};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            commandBufferInfo.commandPool = window.commandPools[i];
            vkAllocateCommandBuffers(device, &commandBufferInfo, &window.commandBuffers[i]);
        }
    }

    void create_semaphores(VkDevice device, WindowData &window)
    {
        VkSemaphoreCreateInfo semaphoreInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                                            .pNext = nullptr,
                                            .flags = 0};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &window.acquireNextImageSemaphores[i]);
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &window.renderSemaphores[i]);
        }
    }

    void create_fences(VkDevice device, VulkanWindows &data)
    {
        VkFenceCreateInfo fenceInfo{.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                                    .pNext = nullptr,
                                    .flags = VK_FENCE_CREATE_SIGNALED_BIT};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            vkCreateFence(device, &fenceInfo, nullptr, &data.renderFences[i]);
        }
    }

    void load_shader_module(VkDevice device, PCWSTR fileName, VkShaderModule &shaderModule)
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
                                            .pNext = nullptr,
                                            .flags = 0,
                                            .codeSize = fileSize,
                                            .pCode = outputBuffer};
        vkCreateShaderModule(device, &shaderInfo, nullptr, &shaderModule);
        if (shaderModule == 0)
        {
            ExitProcess(1);
        }
    }

    void destroy_all_windows(VkInstance instance, VkDevice device, VulkanWindows &data)
    {
        for (u32 i = 0; i < data.emptyWindowIndex; i++)
        {
            WindowData &currentWindow = data.windows[i];
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroySemaphore(device, currentWindow.acquireNextImageSemaphores[k], nullptr);
                vkDestroySemaphore(device, currentWindow.renderSemaphores[k], nullptr);
            }
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyCommandPool(device, currentWindow.commandPools[k], nullptr);
            }
            flush_deletion_queue(device, currentWindow);
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyImageView(device, currentWindow.swapchainImageViews[k], nullptr);
                vkDestroyImageView(device, currentWindow.swapchainDeletionQueue[currentWindow.emptyDeletionQueueIndex].swapchainImageViews[k], nullptr);
            }
            vkDestroySwapchainKHR(device, currentWindow.swapchainDeletionQueue[currentWindow.emptyDeletionQueueIndex].swapchain, nullptr);
            vkDestroySwapchainKHR(device, currentWindow.swapchain, nullptr);
            vkDestroyPipeline(device, currentWindow.graphicsPipeline, nullptr);
            vkDestroyPipelineLayout(device, currentWindow.graphicsPipelineLayout, nullptr);
            vkDestroySurfaceKHR(instance, currentWindow.surface, nullptr);
        }
        data.emptyWindowIndex = 0;
    }

    void destroy_window(VkInstance instance, VkDevice device, VulkanWindows &data, CH::DestroyedWindowInfo message)
    {
        vkDeviceWaitIdle(device);
        if (message.windowIndex == UINT_MAX)
        {
            destroy_all_windows(instance, device, data);
        }
        else
        {
            WindowData &currentWindow = data.windows[message.windowIndex];
            u32 lastFrame = (data.presentData.currentFrame + SWAPCHAIN_IMAGE_COUNT - 1) % SWAPCHAIN_IMAGE_COUNT;
            PresentDataSOA &present = data.presentData.soa[lastFrame];
            u32 validWindowIndex = UINT_MAX;
            for (u32 i = 0; i < data.emptyWindowIndex; i++)
            {
                if (message.windowIndex == data.presentData.validWindows[i])
                {
                    validWindowIndex = i;
                    break;
                }
            }
            if (validWindowIndex != UINT_MAX)
            {
                u32 lastElement = data.presentData.presentBatches[lastFrame].swapchainCount - 1;
                if (validWindowIndex >= lastElement)
                {
                    present.swapchainsDyn[validWindowIndex] = 0;
                    present.swapchainImageIndicesDyn[validWindowIndex] = 0;
                    present.acquireNextImageSemaphoresDyn[validWindowIndex] = 0;
                    present.renderSemaphoresDyn[validWindowIndex] = 0;
                    present.commandBuffersDyn[validWindowIndex] = 0;
                    data.presentData.validWindows[validWindowIndex] = 0;
                    if (validWindowIndex == lastElement)
                    {
                        data.presentData.presentBatches[lastFrame].waitSemaphoreCount -= 1;
                        data.presentData.presentBatches[lastFrame].swapchainCount -= 1;
                    }
                }
                else
                {
                    present.swapchainsDyn[validWindowIndex] = present.swapchainsDyn[lastElement];
                    present.swapchainImageIndicesDyn[validWindowIndex] = present.swapchainImageIndicesDyn[lastElement];
                    present.acquireNextImageSemaphoresDyn[validWindowIndex] = present.acquireNextImageSemaphoresDyn[lastElement];
                    present.renderSemaphoresDyn[validWindowIndex] = present.renderSemaphoresDyn[lastElement];
                    present.commandBuffersDyn[validWindowIndex] = present.commandBuffersDyn[lastElement];
                    data.presentData.validWindows[validWindowIndex] = data.presentData.validWindows[lastElement];
                    present.swapchainsDyn[lastElement] = 0;
                    present.swapchainImageIndicesDyn[lastElement] = 0;
                    present.acquireNextImageSemaphoresDyn[lastElement] = 0;
                    present.renderSemaphoresDyn[lastElement] = 0;
                    present.commandBuffersDyn[lastElement] = 0;
                    data.presentData.validWindows[lastElement] = 0;
                    data.presentData.presentBatches[lastFrame].waitSemaphoreCount -= 1;
                    data.presentData.presentBatches[lastFrame].swapchainCount -= 1;
                }
            }
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroySemaphore(device, currentWindow.acquireNextImageSemaphores[k], nullptr);
                vkDestroySemaphore(device, currentWindow.renderSemaphores[k], nullptr);
            }
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyCommandPool(device, currentWindow.commandPools[k], nullptr);
            }
            flush_deletion_queue(device, currentWindow);
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyImageView(device, currentWindow.swapchainImageViews[k], nullptr);
                vkDestroyImageView(device, currentWindow.swapchainDeletionQueue[currentWindow.emptyDeletionQueueIndex].swapchainImageViews[k], nullptr);
            }
            vkDestroySwapchainKHR(device, currentWindow.swapchainDeletionQueue[currentWindow.emptyDeletionQueueIndex].swapchain, nullptr);
            vkDestroySwapchainKHR(device, currentWindow.swapchain, nullptr);
            vkDestroyPipeline(device, currentWindow.graphicsPipeline, nullptr);
            vkDestroyPipelineLayout(device, currentWindow.graphicsPipelineLayout, nullptr);
            vkDestroySurfaceKHR(instance, currentWindow.surface, nullptr);
            if (message.windowIndex != data.emptyWindowIndex)
            {
                data.windows[message.windowIndex] = data.windows[data.emptyWindowIndex - 1];
            }
            data.emptyWindowIndex -= 1;
        }
    }

    void resize(VkDevice device, VulkanWindows &data, CH::UpdatedWindowInfo message)
    {
        WindowData &updatedWindow = data.windows[message.windowIndex];
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            updatedWindow.swapchainDeletionQueue[updatedWindow.emptyDeletionQueueIndex].swapchainImageViews[i] = updatedWindow.swapchainImageViews[i];
        }
        updatedWindow.swapchainDeletionQueue[updatedWindow.emptyDeletionQueueIndex].swapchain = updatedWindow.swapchain;
        updatedWindow.area.extent.width = message.width;
        updatedWindow.area.extent.height = message.height;
        VkSwapchainCreateInfoKHR swapchainInfo{.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                               .pNext = nullptr,
                                               .flags = 0,
                                               .surface = updatedWindow.surface,
                                               .minImageCount = SWAPCHAIN_IMAGE_COUNT,
                                               .imageFormat = updatedWindow.surfaceFormat.format,
                                               .imageColorSpace = updatedWindow.surfaceFormat.colorSpace,
                                               .imageExtent = updatedWindow.area.extent,
                                               .imageArrayLayers = 1,
                                               .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                               .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                               .queueFamilyIndexCount = 0,
                                               .pQueueFamilyIndices = nullptr,
                                               .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                               .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                               .presentMode = updatedWindow.presentMode,
                                               .clipped = VK_TRUE,
                                               .oldSwapchain = updatedWindow.swapchainDeletionQueue[updatedWindow.emptyDeletionQueueIndex].swapchain};
        vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &updatedWindow.swapchain);
        u32 swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(device, updatedWindow.swapchain, &swapchainImageCount, nullptr);
        vkGetSwapchainImagesKHR(device, updatedWindow.swapchain, &swapchainImageCount, updatedWindow.swapchainImages);
        VkImageViewUsageCreateInfo imageViewUsage{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO,
                                                  .pNext = nullptr,
                                                  .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT};
        VkImageViewCreateInfo imageViewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                            .pNext = &imageViewUsage,
                                            .flags = 0,
                                            .image = 0,
                                            .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                            .format = updatedWindow.surfaceFormat.format,
                                            .components = {
                                                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                .a = VK_COMPONENT_SWIZZLE_IDENTITY},
                                            .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}};
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
        {
            imageViewInfo.image = updatedWindow.swapchainImages[i];
            vkCreateImageView(device, &imageViewInfo, nullptr, &updatedWindow.swapchainImageViews[i]);
        }
        flush_deletion_queue(device, updatedWindow);
        updatedWindow.emptyDeletionQueueIndex = (updatedWindow.emptyDeletionQueueIndex + 1) % (SWAPCHAIN_IMAGE_COUNT * 2);
        updatedWindow.currentFrame = 0;
    }

    void flush_deletion_queue(VkDevice device, WindowData &window)
    {
        vkDeviceWaitIdle(device);
        for (u32 i = 0; i < SWAPCHAIN_IMAGE_COUNT * 2; i++)
        {
            if (i == window.emptyDeletionQueueIndex)
            {
                continue;
            }
            for (u32 k = 0; k < SWAPCHAIN_IMAGE_COUNT; k++)
            {
                vkDestroyImageView(device, window.swapchainDeletionQueue[i].swapchainImageViews[k], nullptr);
                window.swapchainDeletionQueue[i].swapchainImageViews[k] = 0;
            }
            vkDestroySwapchainKHR(device, window.swapchainDeletionQueue[i].swapchain, nullptr);
            window.swapchainDeletionQueue[i].swapchain = 0;
        }
    }
}