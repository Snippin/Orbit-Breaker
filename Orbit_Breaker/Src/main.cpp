#include "Application.hpp"

int main()
{
	Application &app = Application::Get();

	if (app.Init())
	{
		app.Run();
		app.Destroy();

		return 0;
	}

	return 1;
}
