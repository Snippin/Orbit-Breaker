#include "SceneManager.hpp"

#include "Scene.hpp"
#include "SceneType.hpp"

#include <memory>

void SceneManager::Update(float dt)
{
	if (current_scene)
	{
		current_scene->Update(dt);
	}
}

void SceneManager::Render()
{
	if (current_scene)
	{
		current_scene->Render();
	}
}

void SceneManager::Destroy()
{
	scene_map.clear();
}

bool SceneManager::AddScene(SceneType type, std::shared_ptr<Scene> scene)
{
	if (DoesSceneExist(type) || !scene)
	{
		return false;
	}

	scene_map.try_emplace(type, scene);
	return true;
}

bool SceneManager::RemoveScene(SceneType type)
{
	if (!DoesSceneExist(type))
	{
		return false;
	}

	if (type == current_scene_type)
	{
		return false;
	}

	scene_map.erase(type);
	return true;
}

bool SceneManager::SetActiveScene(SceneType type)
{
	if (!DoesSceneExist(type))
	{
		return false;
	}

	ChangeScene(type);
	return true;
}

SceneType SceneManager::GetCurrentSceneType() const
{
	return current_scene_type;
}

Camera *SceneManager::GetActiveCamera()
{
	return current_scene->GetCamera();;
}

bool SceneManager::DoesSceneExist(SceneType type) const
{
	return scene_map.contains(type);
}

void SceneManager::ChangeScene(SceneType type)
{
	if (current_scene)
	{
		current_scene->Destroy();
	}

	current_scene.reset();
	current_scene_type = type;
	current_scene = scene_map.at(current_scene_type);

	current_scene->Init();
	current_scene->Start();
}
