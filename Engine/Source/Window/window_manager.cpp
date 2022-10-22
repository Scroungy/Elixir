#include "window_manager.h"

namespace WM
{
	MSG WindowManager::message{};
	std::Array<WindowManager::WindowSettings> WindowManager::settings{};
	std::Array<WindowManager::Window> WindowManager::windows{};

	void WindowManager::add_settings(WindowSettings &settingsIn)
	{
		// Register settings and add if unique
		if (RegisterClassExW(&settingsIn.windowClass) != 0)
		{
			settings.push_back(settingsIn);
		}
	}

	void WindowManager::remove_settings(u32 index)
	{
		assert(index < settings.size());
		if (index != settings.size() - 1)
		{
			settings.replace_with_back(index);
		}
		else
		{
			settings.pop_back();
		}
	}

	HWND WindowManager::create_window(u32 index)
	{
		assert(index < settings.size());
		// Create window
		HWND handle = CreateWindowExW(settings[index].extendedStyle,
									  settings[index].windowClass.lpszClassName,
									  settings[index].windowName,
									  settings[index].windowStyle,
									  settings[index].xPos,
									  settings[index].yPos,
									  settings[index].width,
									  settings[index].height,
									  settings[index].windowParent,
									  settings[index].titleMenu,
									  settings[index].windowClass.hInstance,
									  0);
		if (handle == 0)
		{
			ExitProcess(1);
		}
		windows.emplace_back(index);
		windows.back().handle = handle;
		return handle;
	}

	void WindowManager::remove_window(u32 index)
	{
		assert(index < windows.size());
		if (index != windows.size() - 1)
		{
			windows.replace_with_back(index);
		}
		else
		{
			windows.pop_back();
		}
	}

	u32 WindowManager::remove_window(HWND hwnd)
	{
		assert(hwnd != nullptr);
		for (u32 i = 0; i < windows.size(); i++)
		{
			if (windows[i].handle == hwnd)
			{
				remove_window(i);
				return i;
			}
		}
		return UINT_MAX;
	}

	void WindowManager::update()
	{
		if (PeekMessageW(&message, 0, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}

	bool WindowManager::should_close()
	{
		return ((windows.size() == 0) ? true : false);
	}
}
