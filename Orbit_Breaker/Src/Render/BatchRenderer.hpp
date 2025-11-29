#pragma once

#include <compare>
#include <memory>
#include <vector>

class Renderer;
class SpriteRenderer;
class Texture;
class GameObject;

class BatchRenderer
{
public:
	BatchRenderer(int max_batch_size, int layer, Renderer *renderer);
	~BatchRenderer();

	void Start();
	void Render();

	void AddSprite(std::shared_ptr<SpriteRenderer> sprite_renderer);
	bool RemoveIfExists(GameObject *go);
	bool RemoveIfExists(std::shared_ptr<GameObject> go);

	bool HasRoom() const;
	bool HasTextureRoom() const;
	bool HasTexture(const std::shared_ptr<Texture> &texture) const;
	int GetLayer() const;

	std::strong_ordering operator<=>(const BatchRenderer &other) const;

private:
	void GenerateIndices(std::vector<unsigned int> &elements) const;
	void LoadElementIndices(std::vector<unsigned int> &elements, int index)
		const;
	void LoadVertexProperties(int index);

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

	std::vector<std::shared_ptr<SpriteRenderer>> sprite_renderers;
	int num_sprite;
	bool is_room_available;
	std::vector<float> vertices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	int max_batch_size;
	std::vector<std::shared_ptr<Texture>> textures;

	int layer;
	Renderer *renderer;
};
