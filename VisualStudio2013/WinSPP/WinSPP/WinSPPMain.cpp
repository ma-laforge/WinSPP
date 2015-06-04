#include "WinSPPFrm.h"
#include <windows.h>

using namespace System;
using namespace System::Windows::Forms;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	using WinSPP::WinSPPFrm;
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	WinSPPFrm^ myfrm = gcnew WinSPPFrm;
	Application::Run(myfrm);
	return 0;
} //WinMain