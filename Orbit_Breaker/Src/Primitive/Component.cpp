#include "Component.hpp"

void Component::Start()
{
}

void Component::Destroy()
{
}

void Component::Update(float dt)
{
}

void Component::GenerateId()
{
	if (uid == 0)
	{
		uid = ++ID_COUNTER;
	}
}

unsigned int Component::GetUid() const
{
	return uid;
}
