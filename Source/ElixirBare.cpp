#include "ElixirBare.h"

DWORD WINAPI render_thread(LPVOID lParam)
{
    RM::RenderManager renderManager = RM::RenderManager{};
    CH::Channel *receiver = (CH::Channel *)lParam;
    RM::initialize(renderManager);
    while (true)
    {
        if (renderManager.presentData.currentFrame == 0)
        {

            CH::Message msg = CH::recv(*receiver);
            switch (msg.type)
            {
            case CH::CreateWindowMessage:
            {
                RM::create_window(renderManager, msg.content.create);
                // RM::render(renderManager); // Must render each time we create a window or validation layers complain
            }
            break;
            case CH::UpdateWindowMessage:
            {
                RM::resize(renderManager, msg.content.update);
            }
            break;
            case CH::DestroyWindowMessage:
            {
                RM::destroy_window(renderManager, msg.content.destroy);
            }
            break;
            }
        }
        if (!RM::is_empty(renderManager))
        {
            RM::render(renderManager);
        }
        else
        {
            break;
        }
    }
    RM::destroy(renderManager);
    return 0;
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int show)
{
    WM::WindowManager windowManager = WM::WindowManager{};
    CH::Channel channel = CH::Channel{};
    windowManager.transmitter = &channel;
    u32 settingsIndex = WM::register_window_settings(windowManager, WM::CreateWindowInfo{.windowName = L"Elixir", .vsync = true});
    WM::create_window(windowManager, settingsIndex, instance);
    HANDLE renderThread = CreateThread(0, 0, render_thread, &channel, 0, 0);
    WM::update(windowManager);
    WaitForSingleObject(renderThread, UINT_MAX);
    return 0;
}

int __stdcall wWinMainCRTStartup()
{
    wWinMain(GetModuleHandle(0), 0, 0, 0);
    return 0;
}
