#include <Window.h>

Window::Window()
	: instance(nullptr), wndproc(nullptr),
	  width(800), height(600), is_fullscreen(false) {
}

Window::~Window() {
	if (window)
	{
		DestroyWindow(window);
		window = NULL;
	}

	UnregisterClassW(window_class_name, instance);
}

bool Window::Initialize(const HINSTANCE inst, const WNDPROC proc) {
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = inst;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = window_class_name;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc)) {
		printf("[DX11]: RegisterClass Failed");
		MessageBoxA(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	if (is_fullscreen)
	{
		posX = 0;
		posY = 0;
	}

	auto dx_style = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	window = CreateWindowEx(	
		WS_EX_APPWINDOW,
		window_class_name, 
		title,
		dx_style,
		posX,
		posY,
		width,
		height,
		NULL,
		NULL,
		inst,
		NULL);
	
	if (!window) {
		printf("[DX11]: CreateWindow Failed");
		MessageBoxA(0, "CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	instance = inst;
	wndproc = proc;

	printf("[DX11]: Window created\n");

	return true;
}

void Window::SetMode(const int x, const int y, const int w, const int h, const bool fullscreen) {
	width = w;
	height = h;
	is_fullscreen = fullscreen;
	posX = x;
	posY = y;
}

HWND Window::GetWindow() {
	return window;
}

HINSTANCE Window::GetInstance() {
	return instance;
}

uint32_t Window::GetWidth() {
	return width;
}

uint32_t Window::GetHeight() {
	return height;
}

bool Window::IsFullscreen() {
	return is_fullscreen;
}
