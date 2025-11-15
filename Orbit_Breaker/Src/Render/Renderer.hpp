#pragma once

#include <vector>
#include <memory>

class GameObject;
class SpriteRenderer;
class Shader;
class Texture;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void SetShader(std::shared_ptr<Shader> new_shader);
	void Start();
	void Render();

	void Add(GameObject *go);
	void Add(std::shared_ptr<GameObject> go);

	void Remove(GameObject *go);
	void Remove(std::shared_ptr<GameObject> go);

private:
	void AddSpriteRenderer(std::shared_ptr<SpriteRenderer> sprite_renderer);
	void LoadVertexProperties(int index);
	void GenerateIndices(std::vector<unsigned int> &elements) const;
	void LoadElementIndices(std::vector<unsigned int> &elements, int index)
		const;

	static constexpr int MAX_TEXTURE_ROOM = 8;

	static constexpr int POSITION_SIZE = 2;
	static constexpr int COLOR_SIZE = 4;
	static constexpr int TEX_COORDS_SIZE = 2;
	static constexpr int TEX_ID_SIZE = 1;
	static constexpr int ENTITY_ID_SIZE = 1;

	static constexpr int COLOR_OFFSET = POSITION_SIZE * sizeof(float);
	static constexpr int TEX_COORDS_OFFSET = COLOR_OFFSET + COLOR_SIZE *
		sizeof(float);
	static constexpr int TEX_ID_OFFSET = TEX_COORDS_OFFSET + TEX_COORDS_SIZE *
		sizeof(float);
	static constexpr int ENTITY_ID_OFFSET = TEX_ID_OFFSET + ENTITY_ID_SIZE *
		sizeof(float);

	static constexpr int VERTEX_SIZE = POSITION_SIZE + COLOR_SIZE +
		TEX_COORDS_SIZE + TEX_ID_SIZE + ENTITY_ID_SIZE;
	static constexpr int VERTEX_SIZE_BYTES = VERTEX_SIZE * sizeof(float);

	std::vector<float> vertices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<SpriteRenderer>> sprite_renderers;
	std::vector<std::shared_ptr<Texture>> textures;
};
