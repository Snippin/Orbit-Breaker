#include "KeyInput.hpp"

void KeyInput::Update(const int key, const int action)
{
	if (key >= 0 && key < MAX_KEYS && action != 2)
	{
		// 1 = press, 0 = release
		current_key_status[key] = (action != 0);
	}
}

void KeyInput::PostUpdate()
{
	previous_key_status = current_key_status;
}

bool KeyInput::IsKeyDown(const int key)
{
	return current_key_status[key];
}

bool KeyInput::IsKeyUp(const int key)
{
	return !current_key_status[key];
}

bool KeyInput::IsKeyPressed(const int key)
{
	return current_key_status[key] && !previous_key_status[key];
}

bool KeyInput::IsKeyReleased(const int key)
{
	return !current_key_status[key] && previous_key_status[key];
}
