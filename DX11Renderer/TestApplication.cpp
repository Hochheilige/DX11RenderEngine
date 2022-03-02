#include <WndDxIncludes.h>

#include <iostream>
#include <chrono>

#include <Window.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

int main() {

	HINSTANCE hInstance = GetModuleHandle(nullptr);
	Window window;
	window.Initialize(hInstance, WndProc);

	std::chrono::time_point<std::chrono::steady_clock> prev_time = std::chrono::steady_clock::now();
	float total_time = 0;
	unsigned int frame_count = 0;

	MSG message = {};
	bool is_exit_requested = false;
	while (!is_exit_requested) {
		while (PeekMessage(&message, nullptr, 0u, 0u, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_QUIT) {
			is_exit_requested = true;
		}

		auto current_time = std::chrono::steady_clock::now();
		float delta_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - prev_time).count() / 1000000.0f;
		prev_time = current_time;

		total_time += delta_time;
		frame_count++;

		if (total_time > 1.0f) {
			float fps = frame_count / total_time;

			total_time -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(window.GetWindow(), text);

			frame_count = 0;
		}
	}
}