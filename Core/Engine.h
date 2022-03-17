#pragma once

#include <iostream>
#include <chrono>
#include <memory>


#include <Renderer.h>

class Engine {
public:
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool Initialize(HINSTANCE hInstance);
	int virtual Run();

	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static Engine& GetEnginge();

protected:
	Engine();

private:
	Renderer* renderer;
	static Engine* engine;
};

