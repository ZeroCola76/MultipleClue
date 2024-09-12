#include "GameProcessDemo.h"
int main() {
	std::unique_ptr<GameProcessDemo> pGameProcessDemo = std::make_unique<GameProcessDemo>();

	if (!pGameProcessDemo->Initialize()) {
		std::cout << "Initialization failed." << std::endl;
		return -1;
	}

	pGameProcessDemo->Loop();
	pGameProcessDemo->Finalize();
}
