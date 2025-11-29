#pragma once

#include "../Primitive/Camera.hpp"
#include "../Render/Renderer.hpp"

#include <memory>
#include <vector>

class GameObject;

class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

	virtual void Init() = 0;
	void Start();
	void Destroy() const;
	void Update(float dt);
	void Render();

	Camera *GetCamera();

protected:
	void AddGameObject(std::shared_ptr<GameObject> go);

private:
	void FlushDelayedGameObjectActions();

	Renderer renderer;
	bool is_running;
	Camera camera;

	std::vector<std::shared_ptr<GameObject>> gos;
	std::vector<std::shared_ptr<GameObject>> deffered_gos;
	std::vector<std::shared_ptr<GameObject>> destroyed_gos;
};
