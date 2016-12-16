#include "Application.h"

int main()
{
	Application::getInstance().init();
	Application::getInstance().run();
	Application::getInstance().close();

	return 0;
}
