#include "window_manager.h"

namespace WM
{
	LRESULT CALLBACK w_wndproc(WindowManager &data, HWND window, u32 message, WPARAM wParam, LPARAM lParam)
	{
		WindowData *currentWindow = 0;
		u32 currentIndex = 0;
		for (u32 i = 0; i < data.emptyMapIndex; i++)
		{
			if (window == data.indexMap[i])
			{
				currentWindow = &data.windows[i];
				currentIndex = i;
			}
		}
		switch (message)
		{
		case WM_SIZE:
		{
			if (currentWindow)
			{
				// CH::send(*data.transmitter, {CH::MessageType::UpdateWindowMessage,
				// 							 {.update = {(u32)(currentWindow->clientSize.right - currentWindow->clientSize.left),
				// 										 (u32)(currentWindow->clientSize.bottom - currentWindow->clientSize.top),
				// 										 currentIndex}}});
				// while (data.transmitter->queue.messagePosted)
				// {
				// 	continue;
				// }
				// currentWindow->sized = true;
			}
		}
		break;
		case WM_DESTROY:
		{
			currentWindow->close = true;
			return 0;
		}
		break;
		}
		return DefWindowProcW(window, message, wParam, lParam);
	}

	u32 register_window_settings(WindowManager &data, CreateWindowInfo settings)
	{
		if (data.emptyWindowSettingsIndex > MAX_WINDOW_SETTINGS)
		{
			return UINT_MAX;
		}
		settings.windowClass.lpszClassName = settings.windowName;
		u32 containIndex = contains(data, settings);
		if (containIndex != UINT_MAX)
		{
			RegisterClassExW(&settings.windowClass);
			data.windowSettings[data.emptyWindowSettingsIndex] = settings;
			data.emptyWindowSettingsIndex += 1;
			return data.emptyWindowSettingsIndex - 1;
		}
		return containIndex;
	}

	void create_window(WindowManager &data, u32 settingsIndex, HINSTANCE hInstance)
	{
		if (data.emptyMapIndex > MAX_WINDOWS)
		{
			return;
		}
		CreateWindowInfo &currentSettings = data.windowSettings[settingsIndex];
		currentSettings.windowClass.hInstance = hInstance;
		HWND windowHandle = CreateWindowExW(currentSettings.extendedStyle,
											currentSettings.windowClass.lpszClassName,
											currentSettings.windowName,
											currentSettings.windowStyle,
											currentSettings.xPos,
											currentSettings.yPos,
											currentSettings.width,
											currentSettings.height,
											currentSettings.windowParent,
											currentSettings.titleMenu,
											currentSettings.windowClass.hInstance,
											(LPVOID)&data);
		if (windowHandle == 0)
		{
			DWORD code = GetLastError();
			return;
		}
		data.indexMap[data.emptyMapIndex] = windowHandle;
		data.windows[data.emptyMapIndex] = WindowData{
			.settingsIndex = 0,
			.windowSize = {0, 0, (i32)currentSettings.width, (i32)currentSettings.height}};

		GetClientRect(windowHandle, &data.windows[data.emptyMapIndex].clientSize);
		if (data.mainWindow == 0)
		{
			data.mainWindow = windowHandle;
		}
		WindowData &currentWindow = data.windows[data.emptyMapIndex];
		CH::send(*data.transmitter, {CH::MessageType::CreateWindowMessage,
									 {.create = {(u32)(currentWindow.clientSize.right - currentWindow.clientSize.left),
												 (u32)(currentWindow.clientSize.bottom - currentWindow.clientSize.top),
												 windowHandle,
												 hInstance,
												 data.emptyMapIndex,
												 currentSettings.vsync}}});
		data.emptyMapIndex += 1;
	}

	void handle_messages(WindowManager &data)
	{
		for (u32 i = 0; i < data.emptyMapIndex; i++)
		{
			WindowData &currentWindow = data.windows[i];
			if (PeekMessageW(&currentWindow.message, data.indexMap[i], 0, 0, PM_REMOVE) != 0)
			{
				TranslateMessage(&currentWindow.message);
				DispatchMessageW(&currentWindow.message);
			}
			if (currentWindow.close)
			{
				data.destroy[data.emptyDestroyIndex] = i;
				data.emptyDestroyIndex += 1;
				continue;
			}
			// if (currentWindow.sized)
			// {
			// 	CH::send(*data.transmitter, {CH::MessageType::UpdateWindowMessage,
			// 								 {.update = {(u32)(currentWindow.clientSize.right - currentWindow.clientSize.left),
			// 											 (u32)(currentWindow.clientSize.bottom - currentWindow.clientSize.top),
			// 											 i}}});
			// 	currentWindow.sized = false;
			// }
		}
	}

	void destroy_windows(WindowManager &data)
	{
		for (u32 i = 0; i < data.emptyDestroyIndex; i++)
		{
			if (data.indexMap[data.destroy[i]] == data.mainWindow)
			{
				clear(data);
				return;
			}
		}
		for (u32 i = 0; i < data.emptyDestroyIndex; i++)
		{
			WindowData &destroyWindow = data.windows[data.destroy[i]];
			CH::send(*data.transmitter, {CH::MessageType::DestroyWindowMessage,
										 {.destroy = {data.destroy[i]}}});
			data.indexMap[data.destroy[i]] = data.indexMap[data.emptyMapIndex - 1];
			data.windows[data.destroy[i]] = data.windows[data.emptyMapIndex - 1];
			// SetWindowLongPtrW(data.indexMap[data.emptyMapIndex - 1], GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&data.windows[data.destroy[i]]));
			data.windows[data.emptyMapIndex - 1] = {};
			data.indexMap[data.emptyMapIndex - 1] = {};
			data.emptyMapIndex -= 1;
		}
		for (u32 i = 0; i < MAX_WINDOWS; i++)
		{
			data.destroy[i] = {};
		}
		data.emptyDestroyIndex = 0;
	}

	void clear(WindowManager &data)
	{
		CH::send(*data.transmitter, {CH::MessageType::DestroyWindowMessage,
									 {.destroy = {UINT_MAX}}});
		data = {};
	}

	void update(WindowManager &data)
	{
		while (data.mainWindow != 0)
		{
			handle_messages(data);
			destroy_windows(data);
		}
	}

	u32 contains(WindowManager &data, CreateWindowInfo &settings)
	{
		for (u32 i = 0; i < data.emptyWindowSettingsIndex; i++)
		{
			CreateWindowInfo &currentSettings = data.windowSettings[i];
			if (is_equal(currentSettings.windowName, settings.windowName))
			{
				return i;
			}
		}
		return data.emptyWindowSettingsIndex;
	}

	bool is_equal(LPCWSTR first, LPCWSTR second)
	{
		while (*first)
		{
			if (*first != *second)
			{
				return false;
			}
			first++;
			second++;
		}
		return true;
	}

	LRESULT CALLBACK wndproc(HWND window, u32 message, WPARAM wParam, LPARAM lParam)
	{
		WindowManager *dataPtr = nullptr;
		if (message == WM_NCCREATE)
		{
			LPCREATESTRUCTW createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			dataPtr = static_cast<WindowManager *>(createStruct->lpCreateParams);
			SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dataPtr));
		}
		else
		{
			dataPtr = reinterpret_cast<WindowManager *>(GetWindowLongPtrW(window, GWLP_USERDATA));
		}
		if (dataPtr)
		{
			return w_wndproc(*dataPtr, window, message, wParam, lParam);
		}
		return DefWindowProcW(window, message, wParam, lParam);
	}
}
