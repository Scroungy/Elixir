#pragma once

#include "defines.h"
#include <array.h>

namespace WM
{
	class WindowManager
	{
	public:
		struct WindowSettings
		{
			DWORD windowStyle = WS_VISIBLE | WS_SIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
			DWORD extendedStyle = 0;
			u32 width = DEFAULT_WINDOW_WIDTH;
			u32 height = DEFAULT_WINDOW_HEIGHT;
			i32 xPos = CW_USEDEFAULT;
			i32 yPos = CW_USEDEFAULT;
			HWND windowParent = 0;
			HMENU titleMenu = 0;
			LPCWSTR windowName = nullptr;
			WNDCLASSEXW windowClass = {
				sizeof(WNDCLASSEXW),
				CS_HREDRAW | CS_VREDRAW,
				nullptr,
				0,
				sizeof(i32 *),
				0,
				0,
				0,
				0,
				0,
				nullptr,
				0};
		};

		class Window
		{
		public:
			HWND handle = 0;
			u32 settingsIndex = UINT_MAX; // Index into settings array

			Window() {}
			Window(u32 index) : settingsIndex(index) {}
			~Window() {}
		};

		global_variable MSG message;
		global_variable std::Array<WindowSettings> settings;
		global_variable std::Array<Window> windows;

		WindowManager() {}
		~WindowManager() {}

		static void add_settings(WindowSettings &settingsIn); // Add new window settings into array
		static void remove_settings(u32 index);				  // Remove window settings from array
		static HWND create_window(u32 settingsIndex);		  // Create new window with settings index and add to array
		static void remove_window(u32 index);				  // Remove window by index
		static u32 remove_window(HWND hwnd);				  // Remove window by hwnd
		static void update();								  // Process MSG
		static bool should_close();							  // Check if main window exists
	};
}
