#include "vulkan_memory.h"

namespace VM
{
    void initialize(AllocationPool &data, VkDevice device, VkPhysicalDeviceMemoryProperties &memoryProperties, VkDeviceSize mappedSize, VkDeviceSize unmappedSize)
    {
        data.mappedMemory.memoryTypeIndex = find_memory_type(memoryProperties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        data.unmappedMemory.memoryTypeIndex = find_memory_type(memoryProperties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        allocate_mapped_memory(data.mappedMemory, device, mappedSize);
        allocate_unmapped_memory(data.unmappedMemory, device, mappedSize);
    }

    void destroy(AllocationPool &data, VkDevice device)
    {
        free_all_memory(data, device);
    }

    u32 find_suitable_memory_type(VkPhysicalDeviceMemoryProperties &memoryProperties, u32 memoryTypeBitsRequirement, VkMemoryPropertyFlags requiredProperties)
    {
        const u32 memoryCount = memoryProperties.memoryTypeCount;
        for (u32 memoryIndex = 0; memoryIndex < memoryCount; ++memoryIndex)
        {
            const u32 memoryTypeBits = (1 << memoryIndex);
            const bool isRequiredMemoryType = memoryTypeBitsRequirement & memoryTypeBits;
            const VkMemoryPropertyFlags properties = memoryProperties.memoryTypes[memoryIndex].propertyFlags;
            const bool hasRequiredProperties = (properties & requiredProperties) == requiredProperties;
            if (isRequiredMemoryType && hasRequiredProperties)
            {
                return memoryIndex;
            }
        }
        return UINT_MAX;
    }

    u32 find_memory_type(VkPhysicalDeviceMemoryProperties &memoryProperties, VkMemoryPropertyFlags requiredProperties)
    {
        const u32 memoryCount = memoryProperties.memoryTypeCount;
        for (u32 memoryIndex = 0; memoryIndex < memoryCount; ++memoryIndex)
        {
            const VkMemoryPropertyFlags properties = memoryProperties.memoryTypes[memoryIndex].propertyFlags;
            const bool hasRequiredProperties = (properties & requiredProperties) == requiredProperties;
            if (hasRequiredProperties)
            {
                return memoryIndex;
            }
        }
        return UINT_MAX;
    }

    void allocate_mapped_memory(MappedAllocation &data, VkDevice device, VkDeviceSize allocateSize)
    {
        VkMemoryAllocateInfo memInfo{.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                     .pNext = nullptr,
                                     .allocationSize = allocateSize,
                                     .memoryTypeIndex = data.memoryTypeIndex};
        VkResult allocateRes = vkAllocateMemory(device, &memInfo, nullptr, &data.memory);
        if (allocateRes != VK_SUCCESS)
        {
            ExitProcess(1);
        }
        data.memorySize += allocateSize;
    }

    void allocate_unmapped_memory(UnmappedAllocation &data, VkDevice device, VkDeviceSize allocateSize)
    {
        VkMemoryAllocateInfo memInfo{.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                     .pNext = nullptr,
                                     .allocationSize = allocateSize,
                                     .memoryTypeIndex = data.memoryTypeIndex};
        VkResult allocateRes = vkAllocateMemory(device, &memInfo, nullptr, &data.memory);
        if (allocateRes != VK_SUCCESS)
        {
            ExitProcess(1);
        }
        data.memorySize += allocateSize;
    }

    void free_mapped_memory(MappedAllocation &data, VkDevice device)
    {
        vkFreeMemory(device, data.memory, nullptr);
    }

    void free_unmapped_memory(UnmappedAllocation &data, VkDevice device)
    {
        vkFreeMemory(device, data.memory, nullptr);
    }

    void free_all_memory(AllocationPool &data, VkDevice device)
    {
        free_mapped_memory(data.mappedMemory, device);
        free_unmapped_memory(data.unmappedMemory, device);
    }

    MappedMemoryBlock *allocate_mapped_memory_block(MappedAllocation &data, VkDeviceSize blockSize, VkDeviceSize pageSize)
    {
        // Align allocation size to page
        VkDeviceSize allocationSize = ((blockSize / pageSize) + 1) * pageSize;
        // Don't allocate if allocation is full
        if (allocationSize > data.memorySize - data.usedSize)
        {
            return nullptr;
        }
        VkDeviceSize availableOffset = 0;
        VkDeviceSize availableSize = data.memorySize - availableOffset; // Set initial size to rest of chunk
        u32 lastFreeBlock = 0;
        for (u32 i = 0; i < MAX_BLOCKS; i++)
        {
            // Check if we can allocate this space
            if (data.blocks[i].free)
            {
                lastFreeBlock = i;
                // Check blocks ahead to ensure we don't overwrite them
                u32 j = (i + 1); // Next block
                while (j < MAX_BLOCKS)
                {
                    if (!data.blocks[j].free)
                    {
                        availableSize = data.blocks[j].offset - availableOffset;
                        break;
                    }
                    j = (j + 1) % MAX_BLOCKS;
                }
                // Check if we can allocate here
                if (availableSize < allocationSize)
                {
                    continue; // Keep searching for empty blocks
                }
                // Set memory as allocated
                data.blocks[i].offset = availableOffset;
                data.blocks[i].size = allocationSize;
                data.blocks[i].free = false;
                return &data.blocks[i];
            }
            availableOffset += data.blocks[i].size; // Block isn't free, add its size to our offset
            availableSize -= availableOffset;
        }
        if (data.memorySize - data.usedSize > allocationSize)
        {
            data.blocks[lastFreeBlock].size = 0;
            data.blocks[lastFreeBlock].defrag = true;
            return &data.blocks[lastFreeBlock];
        }
        return nullptr;
    }

    UnmappedMemoryBlock *allocate_unmapped_memory_block(UnmappedAllocation &data, VkDeviceSize blockSize, VkDeviceSize pageSize)
    {
        // Align allocation size to page
        VkDeviceSize allocationSize = ((blockSize / pageSize) + 1) * pageSize;
        // Don't allocate if allocation is full
        if (allocationSize > data.memorySize - data.usedSize)
        {
            return nullptr;
        }
        VkDeviceSize availableOffset = 0;
        VkDeviceSize availableSize = data.memorySize - availableOffset; // Set initial size to rest of chunk
        u32 lastFreeBlock = 0;
        for (u32 i = 0; i < MAX_BLOCKS; i++)
        {
            // Check if we can allocate this space
            if (data.blocks[i].free)
            {
                lastFreeBlock = i;
                // Check blocks ahead to ensure we don't overwrite them
                u32 j = (i + 1); // Next block
                while (j < MAX_BLOCKS)
                {
                    if (!data.blocks[j].free)
                    {
                        availableSize = data.blocks[j].offset - availableOffset;
                        break;
                    }
                    j = (j + 1) % MAX_BLOCKS;
                }
                // Check if we can allocate here
                if (availableSize < allocationSize)
                {
                    continue; // Keep searching for empty blocks
                }
                // Set memory as allocated
                data.blocks[i].offset = availableOffset;
                data.blocks[i].size = allocationSize;
                data.blocks[i].free = false;
                return &data.blocks[i];
            }
            availableOffset += data.blocks[i].size; // Block isn't free, add its size to our offset
            availableSize -= availableOffset;
        }
        if (data.memorySize - data.usedSize > allocationSize)
        {
            data.blocks[lastFreeBlock].size = 0;
            data.blocks[lastFreeBlock].defrag = true;
            return &data.blocks[lastFreeBlock];
        }
        return nullptr;
    }

    void defragment_mapped_memory(MappedAllocation &data)
    {
    }

    void defragment_unmapped_memory(UnmappedAllocation &data)
    {
    }

    void free_mapped_memory_block(MappedAllocation &data, MappedMemoryBlock &toFree)
    {
        data.usedSize -= toFree.size;
        toFree.size = 0;
        toFree.free = true;
    }

    void free_unmapped_memory_block(UnmappedAllocation &data, UnmappedMemoryBlock &toFree)
    {
        data.usedSize -= toFree.size;
        toFree.size = 0;
        toFree.free = true;
    }
}
