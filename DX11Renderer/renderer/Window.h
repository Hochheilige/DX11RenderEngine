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

private:
	HWND window;
	HINSTANCE instance;
	WNDPROC wndproc;

private:
	uint32_t width;
	uint32_t height;
	int posX;
	int posY;

	LPCWSTR title = L"DX11Renderer";
	LPCWSTR window_class_name = L"Renderer";
};