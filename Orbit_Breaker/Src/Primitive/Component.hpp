#pragma once

class GameObject;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	virtual void Start();
	virtual void Destroy();
	virtual void Update(float dt);

	void GenerateId();
	unsigned int GetUid() const;

	GameObject *gameObject = nullptr;

protected:
	unsigned int uid = 0;

private:
	inline static unsigned int ID_COUNTER;
};
