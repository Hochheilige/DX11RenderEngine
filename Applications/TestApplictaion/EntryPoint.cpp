#include <stdio.h>
#include <TestApp.h>

int main() {
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	TestApp testApp;
	if (!testApp.Initialize(hInstance)) {
		printf("[TestApplication]: Failed to initialize the application correctly");
		return -1;
	}

	testApp.Run();
	
	return 0;
}