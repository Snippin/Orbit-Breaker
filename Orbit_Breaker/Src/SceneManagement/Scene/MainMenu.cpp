#include "MainMenu.hpp"

#include "../../Component/Core/SpriteRenderer.hpp"
#include "../../Primitive/GameObject.hpp"
#include "../../System/Assets.hpp"

#include <glm/glm.hpp>
#include <memory>

void MainMenu::Init()
{
	auto blank_texture = Assets::GetTexture("Asset/blank.jpg");
	auto ship_texture = Assets::GetTexture("Asset/ship.png");
	auto go = std::make_shared<GameObject>();
	go->transform->scale = glm::vec2{10};
	go->AddComponent<SpriteRenderer>(blank_texture)->
		SetColor({1.f, 1.f, 0.f, 1.f});

	auto go2 = std::make_shared<GameObject>();
	go2->transform->scale = glm::vec2{1.f};
	go2->AddComponent<SpriteRenderer>(ship_texture)->
		SetColor({0.f, 1.f, 1.f, 1.f});

	AddGameObject(go);
	AddGameObject(go2);
}
