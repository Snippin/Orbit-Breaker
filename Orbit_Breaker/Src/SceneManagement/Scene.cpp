#include "Scene.hpp"

#include "../Primitive/GameObject.hpp"
#include "../Render/Shader.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() :
	is_running{}
{
}

void Scene::Init()
{
	auto shader = std::make_shared<Shader>("Shader/default.vs",
		"Shader/default.fs");
	shader->Bind();
	shader->SetMat4("Projection", glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f));
	shader->SetMat4("View", glm::mat4(1.0f));
	shader->Unbind();
	Renderer::SetShader(shader);
}

void Scene::Start()
{
	for (int i = 0; i < gos.size(); i++)
	{
		gos[i]->Start();
		renderer.Add(gos[i]);
	}

	is_running = true;
}

void Scene::Destroy() const
{
	for (const auto &go : gos)
	{
		go->Destroy();
	}
}

void Scene::Update(float dt)
{
	for (const auto &go : gos)
	{
		go->Update(dt);

		if (go->IsDead())
		{
			destroyed_gos.push_back(go);
		}
	}

	FlushDelayedGameObjectActions();
}

void Scene::Render()
{
	renderer.Render();
}

void Scene::AddGameObject(std::shared_ptr<GameObject> go)
{
	if (is_running)
	{
		deffered_gos.push_back(go);
	}
	else
	{
		gos.push_back(go);
	}
}

void Scene::FlushDelayedGameObjectActions()
{
	if (!deffered_gos.empty())
	{
		for (const auto &go : deffered_gos)
		{
			gos.push_back(go);
			go->Start();
			renderer.Add(go);
		}

		deffered_gos.clear();
	}

	if (!destroyed_gos.empty())
	{
		for (const auto &go : destroyed_gos)
		{
			renderer.Remove(go);

			auto it = std::find(gos.begin(), gos.end(), go);
			if (it != gos.end())
			{
				gos.erase(it);
			}
		}

		destroyed_gos.clear();
	}
}
