#include "ExampleApplication.h"

VM::VulkanManager *pV = nullptr;

LRESULT mainWndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
    {
        DestroyWindow(windowHandle);
        pV->queueRemoveWindow(windowHandle);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        // Queue resize in vulkan
    }
    break;
    default:
        return DefWindowProcW(windowHandle, msg, wParam, lParam);
    }
    return 0;
}

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int show)
{
    WM::WindowManager::WindowSettings windowSettings = {.windowName = L"ExampleApplication",
                                                        .windowClass = {.cbSize = sizeof(WNDCLASSEXW),
                                                                        .style = CS_HREDRAW | CS_VREDRAW,
                                                                        .lpfnWndProc = mainWndProc,
                                                                        .cbWndExtra = sizeof(i32 *),
                                                                        .hInstance = GetModuleHandle(0),
                                                                        .lpszClassName = L"ExampleApplication"}};
    WM::WindowManager::add_settings(windowSettings);
    VKH::VulkanSettings vulkanSettings = {.instanceSettings = {.appCIS = {.appName = "ExampleApplication", .appVersion = 1}}};
    VM::VulkanManager vulkanManager(vulkanSettings);
    pV = &vulkanManager;
    VKH::RenderSettings renderSettings = {.commandSettings = {.commandPoolCreateSettings = {.queueFamilyIndex = vulkanManager.deviceData.graphicsQueueFamilyIndex}}};
    vulkanManager.createWindow(renderSettings, 0);
    vulkanManager.createWindow(renderSettings, 0);
    vulkanManager.createWindow(renderSettings, 0);
    std::ThreadPool<1> threads{};
    threads.createThread(0, &VM::updateLoop, &vulkanManager, 0);
    while (!WM::WindowManager::should_close())
    {
        WM::WindowManager::update();
    }
    return 0;
}