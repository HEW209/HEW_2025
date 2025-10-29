//InputManager.cpp
#include <Utility/InputManager.h>

uint8_t InputManager::s_keyTable[MAX_KEY_TYPE];
uint8_t InputManager::s_oldKeyTable[MAX_KEY_TYPE];

void InputManager::Init()
{
	memset(s_oldKeyTable, 0, sizeof(s_oldKeyTable));
	memset(s_keyTable, 0, sizeof(s_keyTable));
}

void InputManager::Update()
{
	memcpy(s_oldKeyTable, s_keyTable, sizeof(s_oldKeyTable));
	bool dummy = GetKeyboardState(s_keyTable);
}

Vector2 InputManager::GetMousePos()
{
	POINT mousePos;
	Vector2 result = { 0.0f, 0.0f };

	if (GetCursorPos(&mousePos))
	{
		result.x = (float)mousePos.x;
		result.y = (float)-mousePos.y;
	}

	return result;
}