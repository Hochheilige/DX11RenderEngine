#pragma once

#include <Engine.h>

class TestApp : public Engine {
public:
	LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
};