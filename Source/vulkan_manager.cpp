#include "vulkan_manager.h"

namespace VMNG
{
    void initialize(VObject &data)
    {
        VB::initialize(data.base);
        VM::initialize(data.allocationPool, data.base.device, data.base.deviceMemProp.memoryProperties, DEFAULT_ALLOCATION_SIZE, DEFAULT_ALLOCATION_SIZE);
        VW::initialize(data.base.device, data.windows);
    }

    void destroy(VObject &data)
    {
        VW::destroy(data.base.device, data.windows);
        VM::destroy(data.allocationPool, data.base.device);
        VB::destroy(data.base);
    }

    void create_window(VObject &data, CH::ConstructedWindowInfo message)
    {
        VW::create_window(data.base, data.windows, message);
    }

    void resize(VObject &data, CH::UpdatedWindowInfo message)
    {
        VW::resize(data.base.device, data.windows, message);
    }

    void destroy_window(VObject &data, CH::DestroyedWindowInfo message)
    {
        VW::destroy_window(data.base.instance, data.base.device, data.windows, message);
    }

    void render(VObject &data)
    {
        VW::render(data.base.device, data.windows, data.base.graphicsQueue);
    }

    bool is_empty(VObject &data)
    {
        return VW::is_empty(data.windows);
    }
}
