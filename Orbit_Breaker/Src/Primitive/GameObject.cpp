#include "GameObject.hpp"

#include <string>

GameObject::GameObject()
{
	is_active = true;
	is_dead = false;
	tag = "";
	uid = ++ID_COUNTER;
}

void GameObject::Start()
{
	for (const auto &component : components)
	{
		component->gameObject = this;
		component->Start();
	}
}

void GameObject::Destroy() const
{
	for (const auto &component : components)
	{
		component->Destroy();
	}
}

void GameObject::Update(float dt) const
{
	if (is_active)
	{
		for (auto const &component : components)
		{
			component->Update(dt);
		}
	}
}

unsigned int GameObject::GetUid() const
{
	return uid;
}

bool GameObject::IsActive() const
{
	return is_active;
}

const std::string &GameObject::GetTag() const
{
	return tag;
}

bool GameObject::IsDead() const
{
	return is_dead;
}

void GameObject::SetUid(unsigned int new_uid)
{
	uid = new_uid;
}

void GameObject::SetActive(bool active)
{
	if (is_active == active)
	{
		return;
	}

	is_active = active;
}

void GameObject::SetTag(const std::string &new_tag)
{
	tag = new_tag;
}

void GameObject::SetDead()
{
	is_dead = true;
}

void GameObject::GenerateId()
{
	uid = ++ID_COUNTER;
}
