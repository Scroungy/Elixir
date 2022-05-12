#include "ElixirBare.h"

DWORD WINAPI render_thread(LPVOID lParam)
{
    VMNG::VObject vulkanManager = VMNG::VObject{};
    CH::Channel *receiver = (CH::Channel *)lParam;
    VMNG::initialize(vulkanManager);
    while (true)
    {
        CH::Message msg = CH::recv(*receiver);
        switch (msg.type)
        {
        case CH::CreateWindowMessage:
        {
            VMNG::create_window(vulkanManager, msg.content.create);
        }
        break;
        case CH::UpdateWindowMessage:
        {
            VMNG::resize(vulkanManager, msg.content.update);
        }
        break;
        case CH::DestroyWindowMessage:
        {
            VMNG::destroy_window(vulkanManager, msg.content.destroy);
        }
        break;
        }
        if (!VMNG::is_empty(vulkanManager))
        {
            VMNG::render(vulkanManager);
        }
        else
        {
            break;
        }
    }
    VMNG::destroy(vulkanManager);
    return 0;
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int show)
{
    WM::WindowManager windowManager = WM::WindowManager{};
    CH::Channel channel = CH::Channel{};
    windowManager.transmitter = &channel;
    u32 settingsIndex = WM::register_window_settings(windowManager, WM::CreateWindowInfo{.windowName = L"Elixir", .vsync = true});
    WM::create_window(windowManager, settingsIndex, instance);
    WM::create_window(windowManager, settingsIndex, instance);
    HANDLE renderThread = CreateThread(0, 0, render_thread, &channel, 0, 0);
    WM::create_window(windowManager, settingsIndex, instance);
    WM::create_window(windowManager, settingsIndex, instance);
    WM::update(windowManager);
    WaitForSingleObject(renderThread, UINT_MAX);
    return 0;
}

int __stdcall wWinMainCRTStartup()
{
    wWinMain(GetModuleHandle(0), 0, 0, 0);
    return 0;
}
