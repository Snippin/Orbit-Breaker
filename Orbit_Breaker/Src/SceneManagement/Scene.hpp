#pragma once

#include "../Render/Renderer.hpp"

#include <vector>
#include <memory>

class GameObject;

class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

	virtual void Init();
	void Start();
	void Destroy() const;
	void Update(float dt);
	void Render();

protected:
	void AddGameObject(std::shared_ptr<GameObject> go);

private:
	void FlushDelayedGameObjectActions();

	Renderer renderer;
	bool is_running;

	std::vector<std::shared_ptr<GameObject>> gos;
	std::vector<std::shared_ptr<GameObject>> deffered_gos;
	std::vector<std::shared_ptr<GameObject>> destroyed_gos;
};
