#include "Application.hpp"

#include "Component/Core/SpriteRenderer.hpp"
#include "Component/Core/Transform.hpp"
#include "Primitive/GameObject.hpp"
#include "Render/Renderer.hpp"
#include "Render/Shader.hpp"
#include "Render/Texture.hpp"
#include "Utility/Echo.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, window_width, window_height);

	return true;
}

void Application::Run()
{
	auto shader = std::make_shared<Shader>("Shader/default.vs",
		"Shader/default.fs");
	shader->Bind();
	shader->SetMat4("Projection", glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f));
	shader->SetMat4("View", glm::mat4(1.0f));
	shader->Unbind();

	auto blank_texture = std::make_shared<Texture>();
	blank_texture->Init("Asset/blank.jpg");
	auto go = std::make_shared<GameObject>();
	go->transform = go->AddComponent<Transform>();
	go->transform->scale = glm::vec2{0.5f};
	go->AddComponent<SpriteRenderer>(blank_texture)->
		SetColor({1.f, 1.f, 0.f, 1.f});
	go->Start();

	Renderer renderer;
	renderer.SetShader(shader);
	renderer.Add(go);
	renderer.Start();

	while (!glfwWindowShouldClose(window))
	{
		// Poll events
		glfwPollEvents();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer.Render();

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
