#pragma once

#include <bitset>

class KeyInput
{
public:
	const static int MAX_KEYS = 348;

	static void Update(const int key, const int action);
	static void PostUpdate();

	static bool IsKeyDown(const int key);
	static bool IsKeyUp(const int key);
	static bool IsKeyPressed(const int key);
	static bool IsKeyReleased(const int key);

private:
	inline static std::bitset<MAX_KEYS> current_key_status;
	inline static std::bitset<MAX_KEYS> previous_key_status;
};
