#pragma once

#include "Typedefs.h"
#include "volk.h"
#include "channel.h"

namespace RM
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
		u32 currentFrame = 0;			 // Current multiplier used for offsets in non-dyn arrays
		u32 validWindowCount = 0;		 // Count of acquired swapchain images
		u64 currentRenderTick = 0;		 // Iteration of render
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

	// Window specific data
	struct RenderData
	{
		u32 emptyDeletionQueueIndex = 0;
		HWND windowHandle = 0;
		VkRect2D area{};
		VkSurfaceKHR surface{};
		VkPresentModeKHR presentMode{};
		VkSurfaceFormatKHR surfaceFormat{};
		VkPipelineLayout graphicsPipelineLayout{};
		VkPipeline graphicsPipeline{};
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

	// Used to store data not intended for present/submit batches
	struct WindowData
	{
		u32 emptyWindowIndex = 0;
		VkFence renderFences[SWAPCHAIN_IMAGE_COUNT]{};
		RenderData windows[MAX_WINDOWS]{};
	};

	struct RenderManager
	{
		VkInstance instance{};
		VkPhysicalDevice graphicsCard{};
		VkDevice device{};
		VkQueue graphicsQueue{};
		u32 queueFamilyIndices[QUEUE_FAMILY_INDEX_COUNT]{};
		VkMemoryPropertyFlags memPropFlags[MEMORY_TYPE_FLAG_COUNT + 1]{VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
																	   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0};
		VkPhysicalDeviceMemoryProperties2 deviceMemProp{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2};
		VkPhysicalDeviceVulkan13Properties device13Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES};
		VkPhysicalDeviceVulkan12Properties device12Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES,
														.pNext = &device13Prop};
		VkPhysicalDeviceVulkan11Properties device11Prop{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES,
														.pNext = &device12Prop};
		VkPhysicalDeviceProperties2 deviceProp{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
											   .pNext = &device11Prop};
		VkPhysicalDeviceCoherentMemoryFeaturesAMD deviceMemFeaturesAMD{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD};
		VkPhysicalDeviceVulkan13Features device13Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
														  .pNext = &deviceMemFeaturesAMD};
		VkPhysicalDeviceVulkan12Features device12Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
														  .pNext = &device13Features};
		VkPhysicalDeviceVulkan11Features device11Features{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
														  .pNext = &device12Features};
		VkPhysicalDeviceFeatures2 deviceFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
												 .pNext = &device11Features};
		VkSurfaceCapabilities2KHR deviceCapabilities{.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR,
													 .pNext = nullptr};
		LPCWSTR cwd{};
		WindowData windowData{};
		PresentData presentData{};
	};

	void initialize(RenderManager &data);
	void destroy(RenderManager &data);
	void create_instance(RenderManager &data);
	void select_graphics_card(RenderManager &data);
	void create_device_and_queues(RenderManager &data);
	void acquire(RenderManager &data);
	void record(RenderManager &data);
	void map_to_soa(RenderManager &data);
	void render(RenderManager &data);
	bool is_empty(RenderManager &data);
	bool is_equal(const char *first, const char *second);
	void create_window(RenderManager &data, CH::ConstructedWindowInfo message);
	void create_surface(RenderManager &data, RenderData &window, HINSTANCE instance, HWND hwnd);
	void select_surface_format(RenderManager &data, RenderData &window);
	void select_present_mode(RenderManager &data, RenderData &window);
	void create_swapchain(RenderManager &data, RenderData &window);
	void get_swapchain_images(RenderManager &data, RenderData &window);
	void create_swapchain_image_views(RenderManager &data, RenderData &window);
	void create_graphics_pipeline_layout(RenderManager &data, RenderData &window);
	void create_graphics_pipeline(RenderManager &data, RenderData &window);
	void create_command_pools(RenderManager &data, RenderData &window);
	void create_command_buffers(RenderManager &data, RenderData &window);
	void create_semaphores(RenderManager &data, RenderData &window);
	void create_fences(RenderManager &data);
	void load_shader_module(RenderManager &data, PCWSTR fileName, VkShaderModule &shaderModule);

	void destroy_all_windows(RenderManager &data);
	void destroy_window(RenderManager &data, CH::DestroyedWindowInfo message);
	void resize(RenderManager &data, CH::UpdatedWindowInfo message);
	void flush_deletion_queue(RenderManager &data, RenderData &window);
}
