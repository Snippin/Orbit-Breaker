#pragma once

#include "DesignPattern/SingletonTemplate.hpp"

#include <glad/glad.h>
#include <glfw3.h>

class Application : public SingletonTemplate<Application>
{
public:
	bool Init();
	void Run();
	void Destroy();

private:
	GLFWwindow* window;
};
