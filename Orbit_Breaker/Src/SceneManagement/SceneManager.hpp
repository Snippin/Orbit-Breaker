#pragma once

#include "../DesignPattern/SingletonTemplate.hpp"
#include "SceneType.hpp"

#include <map>
#include <memory>

class Scene;
class Camera;

class SceneManager : public SingletonTemplate<SceneManager>
{
public:
	void Update(float dt);
	void Render();
	void Destroy();

	bool AddScene(SceneType type, std::shared_ptr<Scene> scene);
	bool RemoveScene(SceneType type);
	bool SetActiveScene(SceneType type);

	SceneType GetCurrentSceneType() const;
	Camera *GetActiveCamera();

private:
	bool DoesSceneExist(SceneType type) const;
	void ChangeScene(SceneType type);

	std::map<SceneType, std::shared_ptr<Scene>, std::less<>> scene_map;
	std::shared_ptr<Scene> current_scene;
	SceneType current_scene_type;
};
