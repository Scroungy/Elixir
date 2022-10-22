#pragma once

#include "vulkan_context.h"

namespace VKM
{
    struct MappedMemoryBlock
    {
        VkDeviceSize size{};
        VkDeviceSize offset{};
        void *mappedPtr{};
        bool free = true;
        bool defrag = false;
    };

    struct UnmappedMemoryBlock
    {
        VkDeviceSize size{};
        VkDeviceSize offset{};
        bool free = true;
        bool defrag = false;
    };

    struct MappedAllocation
    {
        VkDeviceMemory memory{};
        VkDeviceSize memorySize{};
        VkDeviceSize usedSize{};
        u32 memoryTypeIndex = 0;
        MappedMemoryBlock blocks[MAX_BLOCKS]{};
    };

    struct UnmappedAllocation
    {
        VkDeviceMemory memory{};
        VkDeviceSize memorySize{};
        VkDeviceSize usedSize{};
        u32 memoryTypeIndex = 0;
        UnmappedMemoryBlock blocks[MAX_BLOCKS];
    };

    struct AllocationPool
    {
        MappedAllocation mappedMemory{};
        UnmappedAllocation unmappedMemory{};
    };

    void initialize(AllocationPool &data, VkDevice device, VkPhysicalDeviceMemoryProperties &memoryProperties, VkDeviceSize mappedSize, VkDeviceSize unmappedSize);
    void destroy(AllocationPool &data, VkDevice device);
    u32 find_suitable_memory_type(VkPhysicalDeviceMemoryProperties &memoryProperties, u32 memoryTypeBitsRequirement, VkMemoryPropertyFlags requiredProperties);
    u32 find_memory_type(VkPhysicalDeviceMemoryProperties &memoryProperties, VkMemoryPropertyFlags requiredProperties);
    void allocate_mapped_memory(MappedAllocation &data, VkDevice device, VkDeviceSize allocateSize);
    void allocate_unmapped_memory(UnmappedAllocation &data, VkDevice device, VkDeviceSize allocateSize);
    void free_mapped_memory(MappedAllocation &data, VkDevice device);
    void free_unmapped_memory(UnmappedAllocation &data, VkDevice device);
    void free_all_memory(AllocationPool &data, VkDevice device);

    MappedMemoryBlock *allocate_mapped_memory_block(MappedAllocation &data, VkDeviceSize blockSize, VkDeviceSize pageSize);
    UnmappedMemoryBlock *allocate_unmapped_memory_block(UnmappedAllocation &data, VkDeviceSize blockSize, VkDeviceSize pageSize);
    void free_mapped_memory_block(MappedAllocation &data, MappedMemoryBlock &toFree);
    void free_unmapped_memory_block(UnmappedAllocation &data, UnmappedMemoryBlock &toFree);
}
