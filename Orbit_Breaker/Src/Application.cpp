#include "Application.hpp"

#include "Utility/Echo.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

bool Application::Init()
{
	if (!glfwInit())
	{
		return false;
	}

	// Configure GLFW
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	const int window_width = 1280;
	const int window_height = 720;

	// Create window
	window = glfwCreateWindow(window_width, window_height, "Orbit Breaker",
		nullptr, nullptr);
	if (window == nullptr)
	{
		Echo::Error("Failed to open GLFW window");
		glfwTerminate();
		return false;
	}

	// Makes the context of the specified window current on the calling thread
	glfwMakeContextCurrent(window);
	// Make window visible
	glfwShowWindow(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Echo::Error("Failed to initialize GLAD");
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, window_width, window_height);

	return true;
}

void Application::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		// Poll events
		glfwPollEvents();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
}

void Application::Destroy()
{
	// Close OpenGL window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	glfwSetErrorCallback(nullptr);
}
