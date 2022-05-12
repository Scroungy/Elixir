#pragma once

#include "vulkan_memory.h"
#include "vulkan_window.h"

namespace VMNG
{
    struct VObject
    {
        VB::VulkanBase base{};
        VM::AllocationPool allocationPool{};
        VW::VulkanWindows windows{};
    };

    void initialize(VObject &data);
    void destroy(VObject &data);
    void create_window(VObject &data, CH::ConstructedWindowInfo message);
    void resize(VObject &data, CH::UpdatedWindowInfo message);
    void destroy_window(VObject &data, CH::DestroyedWindowInfo message);
    void render(VObject &data);
    bool is_empty(VObject &data);
}
