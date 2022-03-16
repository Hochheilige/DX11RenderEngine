#include "TestApp.h"

LRESULT TestApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return Engine::GetEnginge().MsgProc(hwnd, msg, wParam, lParam);
}
