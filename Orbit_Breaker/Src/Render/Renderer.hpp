#pragma once

#include <vector>
#include <memory>

class GameObject;
class BatchRenderer;
class SpriteRenderer;
class Shader;

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	static void SetShader(std::shared_ptr<Shader> new_shader);
	static std::shared_ptr<Shader> GetShader();

	void Render();

	void Add(GameObject *go);
	void Add(std::shared_ptr<GameObject> go);

	void Remove(GameObject *go);
	void Remove(std::shared_ptr<GameObject> go);

private:
	void AddSpriteRenderer(std::shared_ptr<SpriteRenderer> sprite_renderer);

	static constexpr int MAX_BATCH_SIZE = 1000;
	static inline std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<BatchRenderer>> batch_renderers;
};
