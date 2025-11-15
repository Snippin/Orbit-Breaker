#pragma once

#include "Component.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject() = default;

	template <typename T>
	std::shared_ptr<T> GetComponent();
	template <typename T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args);
	template <typename T>
	void RemoveComponent();

	void Start();
	void Destroy() const;
	void Update(float dt) const;

	unsigned int GetUid() const;
	bool IsActive() const;
	const std::string &GetTag() const;

	void SetUid(unsigned int new_uid);
	void SetActive(bool active = true);
	void SetTag(const std::string &new_tag);

	void GenerateId();

private:
	inline static unsigned int ID_COUNTER;

	unsigned int uid = 0;
	std::string tag;
	std::vector<std::shared_ptr<Component>> components;
	std::unordered_map<std::type_index, std::shared_ptr<Component>>
		component_lookup;
	bool is_active;
};

template<typename T>
inline std::shared_ptr<T> GameObject::GetComponent()
{
	auto it = component_lookup.find(std::type_index(typeid(T)));
	return it != component_lookup.end() ?
		std::static_pointer_cast<T>(it->second) : nullptr;
}

template<typename T, typename ...Args>
inline std::shared_ptr<T> GameObject::AddComponent(Args && ...args)
{
	static_assert(std::is_base_of_v<Component, T>,
		"T must inherit from `Component`");

	auto component = std::make_shared<T>(std::forward<Args>(args)...);

	component_lookup[std::type_index(typeid(T))] = component;
	components.push_back(component);
	component->GenerateId();
	component->gameObject = this;

	return component;
}

template<typename T>
inline void GameObject::RemoveComponent()
{
	auto typeIndex = std::type_index(typeid(T));

	components.erase(
		std::remove_if(components.begin(), components.end(),
		[&typeIndex, this](const std::unique_ptr<Component> &component)
		{
			if (std::type_index(typeid(*component)) == typeIndex)
			{
				component_lookup.erase(typeIndex);
				return true;
			}

			return false;
		}), components.end());
}
