#include "Application.hpp"

#include "Input/KeyInput.hpp"
#include "SceneManagement/Scene/MainMenu.hpp"
#include "SceneManagement/SceneManager.hpp"
#include "SceneManagement/SceneType.hpp"
#include "System/Assets.hpp"
#include "System/Time.hpp"
#include "Utility/Echo.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

static void SetGLFWCallbacks(GLFWwindow *window)
{
	// Key callbacks
	glfwSetKeyCallback(window,
		[]([[maybe_unused]] GLFWwindow *glfw_window, int key,
		[[maybe_unused]] int scan_code, int action, [[maybe_unused]] int mods)
		{
			KeyInput::Update(key, action);
		}
	);
}

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
	glEnable(GL_BLEND);

	glViewport(0, 0, window_width, window_height);

	SetGLFWCallbacks(window);

	return true;
}

void Application::Run()
{
	auto shader = Assets::GetShader("Shader/texture");
	Renderer::SetShader(shader);

	auto scene_manager = &SceneManager::Get();
	scene_manager->AddScene(SceneType::MAIN_MENU,
		std::make_shared<MainMenu>());
	scene_manager->SetActiveScene(SceneType::MAIN_MENU);
	scene_manager->GetActiveCamera()->position = {-1.f, -1.f};

	auto &time = Time::Get();
	time.Init();
	time.StartTimer();

	while (!glfwWindowShouldClose(window))
	{
		// Poll events
		glfwPollEvents();

		if (KeyInput::IsKeyPressed(GLFW_KEY_ESCAPE) &&
			scene_manager->GetCurrentSceneType() == SceneType::MAIN_MENU)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		scene_manager->Update(time.GetDeltaTime());
		scene_manager->Render();

		glfwSwapBuffers(window);

		KeyInput::PostUpdate();
		time.Update();
	}
}

void Application::Destroy()
{
	// Close OpenGL window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	glfwSetErrorCallback(nullptr);
}
