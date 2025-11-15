#include "Application.hpp"

#include "Render/Shader.hpp"
#include "Utility/Echo.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/fwd.hpp>
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
	auto shader = std::make_unique<Shader>("Shader/default.vs",
		"Shader/default.fs");
	shader->Bind();
	shader->SetMat4("Projection", glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f));
	shader->SetMat4("View", glm::mat4(1.0f));
	shader->Unbind();
	std::vector<float> vertexArr;
	std::vector<unsigned int> elementArr;
	unsigned int vaoId;

	// Setup
	{
		// Generate VAO, VBO, EBO buffer objects and send to GPU
		vertexArr = {
			// position         // color
			0.5f, -0.5f, 0.f,   1.f, 0.f, 0.f, 1.f, // right bottom 0
		   -0.5f,  0.5f, 0.f,   0.f, 1.f, 0.f, 1.f, // left top     1
			0.5f,  0.5f, 0.f,   0.f, 0.f, 1.f, 1.f, // right top    2
		   -0.5f, -0.5f, 0.f,   1.f, 1.f, 0.f, 1.f, // left bottom  3
		};

		// Must be in counter-clockwise order
		elementArr = {
			/*
				1x   x2

				3x   x0
			*/
			0, 2, 1, // top right triangle
			0, 1, 3 // bottom right triangle
		};

		unsigned int vbo;
		unsigned int ebo;

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float),
			vertexArr.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			elementArr.size() * sizeof(unsigned int), elementArr.data(),
			GL_STATIC_DRAW);

		// Add vertex attribute pointers
		int positionSize = 3;
		int colorSize = 4;
		int vertexSizeByte = (positionSize + colorSize) * sizeof(float);

		glVertexAttribPointer(0, positionSize, GL_FLOAT, false, vertexSizeByte,
			nullptr);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, colorSize, GL_FLOAT, false, vertexSizeByte,
			(void *)(positionSize * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	while (!glfwWindowShouldClose(window))
	{
		// Poll events
		glfwPollEvents();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind shader
		shader->Bind();

		// Bind vao
		glBindVertexArray(vaoId);
		// Enable vertex attribute pointers
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, static_cast<int>(elementArr.size()),
			GL_UNSIGNED_INT, nullptr);

		// Unbind everything
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);

		// Unbind shader
		shader->Unbind();

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
