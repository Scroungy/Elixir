#pragma once

#include "Typedefs.h"
#include "channel.h"

namespace WM
{
	LRESULT CALLBACK wndproc(HWND window, u32 message, WPARAM wParam, LPARAM lParam);

	// Used for creating windows
	struct CreateWindowInfo
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
			WM::wndproc,
			0,
			sizeof(i32 *),
			0,
			0,
			0,
			0,
			0,
			nullptr,
			0};
		bool vsync = false;
	};

	// Used for storing window specific data
	struct WindowData
	{
		u32 settingsIndex = 0;
		RECT windowSize = {};
		RECT clientSize = {};
		MSG message = {};
		bool close = false;
		bool sized = false;
	};

	// Used to manage win32 windows
	struct WindowManager
	{
		CreateWindowInfo windowSettings[MAX_WINDOW_SETTINGS] = {};
		HWND indexMap[MAX_WINDOWS] = {}; // Keep this separate from struct: can linear search through 8 values in one cache line
		u32 destroy[MAX_WINDOWS] = {};
		WindowData windows[MAX_WINDOWS] = {};
		u32 emptyDestroyIndex = 0;
		u32 emptyWindowSettingsIndex = 0;
		u32 emptyMapIndex = 0;
		HWND mainWindow = 0;
		CH::Channel *transmitter{};
	};

	LRESULT CALLBACK w_wndproc(WindowManager &data, HWND window, u32 message, WPARAM wParam, LPARAM lParam);
	u32 register_window_settings(WindowManager &data, CreateWindowInfo settings);
	void create_window(WindowManager &data, u32 settingsIndex, HINSTANCE hInstance);
	void handle_messages(WindowManager &data);
	void destroy_windows(WindowManager &data);
	void clear(WindowManager &data);
	void update(WindowManager &data);
	u32 contains(WindowManager &data, CreateWindowInfo &settings);
	bool is_equal(LPCWSTR first, LPCWSTR second);
}
