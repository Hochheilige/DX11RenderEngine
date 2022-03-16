#include <Engine.h>

Engine* Engine::engine = nullptr;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return Engine::GetEnginge().MsgProc(hwnd, msg, wParam, lParam);
}

Engine::Engine() : renderer(Renderer::GetInstance()) {
}

bool Engine::Initialize(HINSTANCE instance) {
	return renderer->Initialize(instance, MainWndProc);
}

int Engine::Run() {

	MSG msg = { nullptr };
	std::chrono::time_point<std::chrono::steady_clock> prev_time = std::chrono::steady_clock::now();
	float total_time = 0;
	unsigned int frame_count = 0;

	while (msg.message != WM_QUIT) {
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		renderer->Clear();
		renderer->Swap();

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
			SetWindowText(renderer->GetWindow(), text);

			frame_count = 0;
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN: {
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wParam) << std::endl;

		if (static_cast<unsigned int>(wParam) == 27) PostQuitMessage(0);
		return 0;
	}
	default: {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
}

Engine& Engine::GetEnginge() {
	if (engine == nullptr) {
		engine = new Engine();
	}

	return *engine;
}

