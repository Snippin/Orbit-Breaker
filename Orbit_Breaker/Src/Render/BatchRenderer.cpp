#include "BatchRenderer.hpp"

#include "../Component/Core/SpriteRenderer.hpp"
#include "../Primitive/GameObject.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <array>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

BatchRenderer::BatchRenderer(int max_batch_size, int layer, Renderer *renderer)
{
	this->max_batch_size = max_batch_size;
	this->layer = layer;
	this->renderer = renderer;

	num_sprite = 0;
	is_room_available = true;
	VAO = 0;
	VBO = 0;
	EBO = 0;

	sprite_renderers.resize(max_batch_size);
	// 4 vertices per quad
	vertices.resize(max_batch_size * 4 * VERTEX_SIZE);
}

BatchRenderer::~BatchRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void BatchRenderer::Start()
{
	// Generate and bind Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Allocate space for vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), nullptr,
		GL_DYNAMIC_DRAW);

	// Create and upload indices buffer
	glGenBuffers(1, &EBO);
	std::vector<unsigned int> indices;
	GenerateIndices(indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		indices.data(), GL_STATIC_DRAW);

	// Enable buffer attribute pointers
	glVertexAttribPointer(0, POSITION_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES,
		nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, COLOR_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES,
		(void *)(COLOR_OFFSET));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, TEX_COORDS_SIZE, GL_FLOAT, false,
		VERTEX_SIZE_BYTES, (void *)(TEX_COORDS_OFFSET));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, TEX_ID_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES,
		(void *)(TEX_ID_OFFSET));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, ENTITY_ID_SIZE, GL_FLOAT, false, VERTEX_SIZE_BYTES,
		(void *)(ENTITY_ID_OFFSET));
	glEnableVertexAttribArray(4);
}

void BatchRenderer::Render()
{
	bool rebufferData = false;
	for (int i = 0; i < num_sprite; i++)
	{
		const auto &sr = sprite_renderers[i];

		if (sr->IsDirty())
		{
			LoadVertexProperties(i);
			sr->SetClean();
			rebufferData = true;
		}

		if (sr->gameObject->transform->layer != layer)
		{
			RemoveIfExists(sr->gameObject);
			renderer->Add(sr->gameObject);
			i--;
		}
	}

	if (rebufferData)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
			vertices.data());
	}

	const auto &shader = Renderer::GetShader();
	shader->Bind();

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i + 1);
		textures[i]->Bind();
	}
	shader->SetIntArray("Textures", {0, 1, 2, 3, 4, 5, 6, 7});

	// 6 indices per quad
	glDrawElements(GL_TRIANGLES, num_sprite * 6, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	for (const auto &texture : textures)
	{
		texture->Unbind();
	}

	shader->Unbind();
}

void BatchRenderer::AddSprite(std::shared_ptr<SpriteRenderer> sprite_renderer)
{
	sprite_renderers[num_sprite] = std::move(sprite_renderer);

	auto texture = sprite_renderers[num_sprite]->GetTexture();
	if (texture &&
		std::find(textures.begin(), textures.end(), texture) == textures.end())
	{
		textures.push_back(texture);
	}

	LoadVertexProperties(num_sprite);

	num_sprite++;
	if (num_sprite >= max_batch_size)
	{
		is_room_available = false;
	}
}

bool BatchRenderer::RemoveIfExists(GameObject *go)
{
	const auto &sr = go->GetComponent<SpriteRenderer>();

	for (int i = 0; i < num_sprite; i++)
	{
		if (sprite_renderers[i] == sr)
		{
			for (int j = i; j < num_sprite - 1; j++)
			{
				sprite_renderers[j] = sprite_renderers[j + 1];
				sprite_renderers[j]->SetDirty();
			}

			num_sprite--;
			return true;
		}
	}

	return false;
}

bool BatchRenderer::RemoveIfExists(std::shared_ptr<GameObject> go)
{
	return RemoveIfExists(go.get());
}

bool BatchRenderer::HasRoom() const
{
	return is_room_available;
}

bool BatchRenderer::HasTextureRoom() const
{
	// - 1 due to index 0 being saved for rendering without a texture
	return textures.size() < MAX_TEXTURE_ROOM - 1;
}

bool BatchRenderer::HasTexture(const std::shared_ptr<Texture> &texture) const
{
	return
		std::find(textures.begin(), textures.end(), texture) != textures.end();
}

int BatchRenderer::GetLayer() const
{
	return layer;
}

std::strong_ordering BatchRenderer::operator<=>(
	const BatchRenderer &other) const
{
	return layer <=> other.GetLayer();
}

void BatchRenderer::GenerateIndices(std::vector<unsigned int> &elements) const
{
	// 6 indices per quad (3 per triangle)
	elements.resize(6 * max_batch_size);
	for (int i = 0; i < max_batch_size; i++)
	{
		LoadElementIndices(elements, i);
	}
}

void BatchRenderer::LoadElementIndices(std::vector<unsigned int> &elements,
	int index) const
{
	int offset_array_index = 6 * index;
	int offset = 4 * index;

	// (3, 2, 0, 0, 2, 1)
	// Triangle 1
	elements[offset_array_index] = offset + 3;
	elements[offset_array_index + 1] = offset + 2;
	elements[offset_array_index + 2] = offset;
	// Triangle 2
	elements[offset_array_index + 3] = offset;
	elements[offset_array_index + 4] = offset + 2;
	elements[offset_array_index + 5] = offset + 1;
}

void BatchRenderer::LoadVertexProperties(int index)
{
	const auto &sr = sprite_renderers[index];

	// Find offset within array (4 vertices per sprite)
	int offset = index * 4 * VERTEX_SIZE;

	glm::vec4 color = sr->GetColor();
	std::array<glm::vec2, 4> tex_coords = sr->GetTexCoords();
	int tex_id = 0;
	if (sr->GetTexture())
	{
		for (int i = 0; i < textures.size(); i++)
		{
			if (*textures[i] == *sr->GetTexture())
			{
				tex_id = i + 1;
				break;
			}
		}
	}

	std::array<std::array<float, 2>, 4> offsets = {
		std::array{  0.5f,  0.5f }, // Top Right
		std::array{  0.5f, -0.5f }, // Bottom Right
		std::array{ -0.5f, -0.5f }, // Bottom Left
		std::array{ -0.5f,  0.5f }  // Top Left
	};

	bool is_rotated = sr->gameObject->transform->rotation != 0.f;
	const auto &transform = sr->gameObject->transform;
	glm::mat4 transform_matrix(1.f);
	if (is_rotated)
	{
		transform_matrix = glm::translate(transform_matrix,
			glm::vec3(transform->position, 0.f));
		transform_matrix = glm::rotate(transform_matrix,
			glm::radians(transform->rotation), glm::vec3(0, 0, 1));
		transform_matrix = glm::scale(transform_matrix,
			glm::vec3(transform->scale, 1.f));
	}

	for (int i = 0; i < 4; i++)
	{
		float xAdd = offsets[i][0];
		float yAdd = offsets[i][1];

		const glm::vec2 &position = sr->gameObject->transform->position;
		const glm::vec2 &scale = sr->gameObject->transform->scale;

		glm::vec4 currentPosition(
			position.x + (xAdd * scale.x),
			position.y + (yAdd * scale.y),
			0.f, 1.f
		);

		if (is_rotated)
		{
			currentPosition = transform_matrix * glm::vec4(xAdd, yAdd, 0.f, 1.f);
		}

		// Load position
		vertices[offset] = currentPosition.x;
		vertices[offset + 1] = currentPosition.y;

		// Load color
		vertices[offset + 2] = color.r;
		vertices[offset + 3] = color.g;
		vertices[offset + 4] = color.b;
		vertices[offset + 5] = color.a;

		// Load texture coordinates
		vertices[offset + 6] = tex_coords[i].x;
		vertices[offset + 7] = tex_coords[i].y;

		// Load texture id
		vertices[offset + 8] = static_cast<float>(tex_id);

		// Load entity id
		vertices[offset + 9] =
			static_cast<float>(sr->gameObject->GetUid() + 1);

		offset += VERTEX_SIZE;
	}
}
