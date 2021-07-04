#include "MyForm.h"

#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	minesweeper::MyForm form;
	Application::Run(%form);
}