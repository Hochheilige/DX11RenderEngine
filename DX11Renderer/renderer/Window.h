#pragma once

#include <WndDxIncludes.h>

class Window final {
public:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;

	Window();
	~Window();

	bool Initialize(HINSTANCE inst, WNDPROC proc);
	void SetMode(const int x, const int y, const int w, const int h, const bool fullscreen);

	HWND GetWindow();
	HINSTANCE GetInstance();
	uint32_t GetWidth();
	uint32_t GetHeight();

	bool IsFullscreen();

private:
	HWND window;
	HINSTANCE instance;
	WNDPROC wndproc;

private:
	int width;
	int height;
	int posX;
	int posY;

	bool is_fullscreen;

	LPCWSTR title = L"Quake 2 DX11";
	LPCWSTR window_class_name = L"Quake 2";
};