#include "framework.h"
#include "PvDemo.h"
#include "Process.h"
#include <memory>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	std::shared_ptr<Process> process = std::make_shared<Process>();
	process->Initialize(hInstance);
	process->Loop();
	process->Finalize();

}

