#include "Renderer.hpp"

#include "../Component/Core/SpriteRenderer.hpp"
#include "../Primitive/GameObject.hpp"
#include "BatchRenderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <memory>

void Renderer::SetShader(std::shared_ptr<Shader> new_shader)
{
	shader = new_shader;
}

std::shared_ptr<Shader> Renderer::GetShader()
{
	return shader;
}

void Renderer::Render()
{
	shader->Bind();
	for (const auto &renderer : batch_renderers)
	{
		renderer->Render();
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
	if (!go->GetComponent<SpriteRenderer>())
	{
		return;
	}

	for (const auto &renderer : batch_renderers)
	{
		if (renderer->RemoveIfExists(go))
		{
			return;
		}
	}
}

void Renderer::Remove(std::shared_ptr<GameObject> go)
{
	Remove(go.get());
}

void Renderer::AddSpriteRenderer(
	std::shared_ptr<SpriteRenderer> sprite_renderer)
{
	bool added = false;

	for (const auto &renderer : batch_renderers)
	{
		if (renderer->HasRoom() &&
			renderer->GetLayer() ==
			sprite_renderer->gameObject->transform->layer)
		{
			const auto &texture = sprite_renderer->GetTexture();
			if (!texture || renderer->HasTexture(texture) ||
				renderer->HasTextureRoom())
			{
				renderer->AddSprite(sprite_renderer);
				added = true;
				break;
			}
		}
	}

	if (!added)
	{
		auto new_renderer = std::make_shared<BatchRenderer>(MAX_BATCH_SIZE,
			sprite_renderer->gameObject->transform->layer, this);
		new_renderer->Start();
		new_renderer->AddSprite(sprite_renderer);
		batch_renderers.push_back(new_renderer);

		std::sort(batch_renderers.begin(), batch_renderers.end(),
			[](const auto &a, const auto &b)
			{
				return a < b;
			}
		);
	}
}
