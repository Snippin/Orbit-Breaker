#include "Renderer.hpp"

#include "../Component/Core/SpriteRenderer.hpp"
#include "../Primitive/GameObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>

Renderer::Renderer()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Renderer::SetShader(std::shared_ptr<Shader> new_shader)
{
	shader = new_shader;
}

void Renderer::Start()
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

void Renderer::Render()
{
	shader->Bind();

	bool rebufferData = false;
	for (int i = 0; i < sprite_renderers.size(); i++)
	{
		const auto &sr = sprite_renderers[i];

		if (sr->IsDirty())
		{
			LoadVertexProperties(i);
			sr->SetClean();
			rebufferData = true;
		}
	}

	if (rebufferData)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			vertices.size() * sizeof(float), vertices.data());
	}

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
	glDrawElements(GL_TRIANGLES, sprite_renderers.size() * 6, GL_UNSIGNED_INT,
		nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	for (const auto &texture : textures)
	{
		texture->Unbind();
	}

	shader->Unbind();
}

void Renderer::Add(GameObject *go)
{
	auto sr = go->GetComponent<SpriteRenderer>();

	if (sr)
	{
		AddSpriteRenderer(sr);
	}
}

void Renderer::Add(std::shared_ptr<GameObject> go)
{
	Add(go.get());
}

void Renderer::Remove(GameObject *go)
{
	auto sr = go->GetComponent<SpriteRenderer>();
	if (!sr)
	{
		return;
	}

	auto begin = sprite_renderers.begin();
	auto end = sprite_renderers.end();

	// Find the first element equal to sprite renderer to be removed
	auto it = std::find(begin, end, sr);
	if (it == end)
	{
		return;
	}

	for (auto shiftIt = it + 1; shiftIt != end; ++shiftIt)
	{
		(*shiftIt)->SetDirty();
	}

	// Erase the element(s)
	sprite_renderers.erase(
		std::remove(begin, end, sr),
		sprite_renderers.end()
	);
}

void Renderer::Remove(std::shared_ptr<GameObject> go)
{
	Remove(go.get());
}

void Renderer::AddSpriteRenderer(
	std::shared_ptr<SpriteRenderer> sprite_renderer)
{
	bool added = false;

	for (const auto &sr : sprite_renderers)
	{
		if (sr == sprite_renderer)
		{
			added = true;
			break;
		}
	}

	if (!added)
	{
		sprite_renderers.push_back(sprite_renderer);
		vertices.resize(sprite_renderers.size() * 4 * VERTEX_SIZE);
		LoadVertexProperties(sprite_renderers.size() - 1);
	}
}

void Renderer::LoadVertexProperties(int index)
{
	const auto &sr = sprite_renderers[index];

	// Find offset within array (4 vertices per sprite)
	int offset = index * 4 * VERTEX_SIZE;

	glm::vec4 color = sr->GetColor();
	std::array<glm::vec2, 4> tex_coords = sr->GetTexCoords();
	int tex_id = 0;
	if (sr->GetTexture())
	{
		bool has_texture = false;
		for (int i = 0; i < textures.size(); i++)
		{
			if (*textures[i] == *sr->GetTexture())
			{
				has_texture = true;
				tex_id = i + 1;
				break;
			}
		}

		if (!has_texture)
		{
			textures.push_back(sr->GetTexture());
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
	glm::mat4 transform_mat(1.f);

	if (is_rotated)
	{
		transform_mat = glm::translate(transform_mat,
			glm::vec3(transform->position, 0.f));
		transform_mat = glm::rotate(transform_mat,
			glm::radians(transform->rotation), glm::vec3(0, 0, 1));
		transform_mat = glm::scale(transform_mat,
			glm::vec3(transform->scale, 1.f));
	}

	for (int i = 0; i < 4; i++)
	{
		float x_add = offsets[i][0];
		float y_add = offsets[i][1];

		const glm::vec2 &position = sr->gameObject->transform->position;
		const glm::vec2 &scale = sr->gameObject->transform->scale;

		glm::vec4 current_position(
			position.x + (x_add * scale.x),
			position.y + (y_add * scale.y),
			0.f, 1.f
		);

		if (is_rotated)
		{
			current_position = transform_mat *
				glm::vec4(x_add, y_add, 0.f, 1.f);
		}

		// Load position
		vertices[offset] = current_position.x;
		vertices[offset + 1] = current_position.y;

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
		vertices[offset + 9] = static_cast<float>(
			sr->gameObject->GetUid() + 1);

		offset += VERTEX_SIZE;
	}
}

void Renderer::GenerateIndices(std::vector<unsigned int> &elements) const
{
	// 6 indices per quad (3 per triangle)
	elements.resize(6 * sprite_renderers.size());
	for (int i = 0; i < sprite_renderers.size(); i++)
	{
		LoadElementIndices(elements, i);
	}
}

void Renderer::LoadElementIndices(std::vector<unsigned int> &elements,
	int index) const
{
	int offsetArrayIndex = 6 * index;
	int offset = 4 * index;

	// (3, 2, 0, 0, 2, 1)
	// Triangle 1
	elements[offsetArrayIndex] = offset + 3;
	elements[offsetArrayIndex + 1] = offset + 2;
	elements[offsetArrayIndex + 2] = offset;
	// Triangle 2
	elements[offsetArrayIndex + 3] = offset;
	elements[offsetArrayIndex + 4] = offset + 2;
	elements[offsetArrayIndex + 5] = offset + 1;
}
