// Input.cpp
#include <Utility/Input.h>
#include <algorithm>

static const BYTE g_triggerDeadzone = 1000;		//コントローラーのトリガーデッドゾーン

HWND Input::s_hWnd;
uint8_t Input::s_oldKeyTable[MAX_KEY_TYPE];
uint8_t Input::s_keyTable[MAX_KEY_TYPE];
XINPUT_STATE Input::s_oldPadState;
XINPUT_STATE Input::s_padState;
Vector2 Input::s_oldMousePos;
Vector2 Input::s_mousePos;
Vector2 Input::s_mouseDelta;
bool Input::s_lockMousePos = false;
float Input::s_mouseWheelBuffer;
float Input::s_mouseWheelDelta;

void Input::Init()
{
	// キー入力初期化
	memset(s_oldKeyTable, 0, sizeof(s_oldKeyTable));
	bool dummy = GetKeyboardState(s_keyTable);

	// コントローラー入力初期化	
	memset(&s_oldPadState, 0, sizeof(XINPUT_STATE));
	memset(&s_padState, 0, sizeof(XINPUT_STATE));
	XInputGetState(0, &s_padState);

	s_hWnd = GetActiveWindow();
}

void Input::Update()
{
	// キー入力更新
	memcpy(s_oldKeyTable, s_keyTable, sizeof(s_oldKeyTable));
	bool dummy = GetKeyboardState(s_keyTable);

	// コントローラー入力更新
	memcpy(&s_oldPadState, &s_padState, sizeof(XINPUT_STATE));
	XInputGetState(0, &s_padState);

	// マウス座標更新
	POINT mousePos;
	if (GetCursorPos(&mousePos))
	{
		s_oldMousePos = s_mousePos;
		s_mousePos.SetVector((float)mousePos.x, (float)mousePos.y);
		s_mouseDelta = s_mousePos - s_oldMousePos;

		if (s_lockMousePos)
		{
			if (GetForegroundWindow() == s_hWnd)
			{
				const Vector2 lockPos(640.0f, 360.0f);
				SetCursorPos((int)lockPos.x, (int)lockPos.y);
				s_mousePos = lockPos;
			}
		}
	}

	// マウスホイールの回転量更新
	s_mouseWheelDelta = s_mouseWheelBuffer;
	s_mouseWheelBuffer = 0.0f;
}

bool Input::IsControllerActive()
{
	XINPUT_STATE state;
	DWORD result = XInputGetState(0, &state);
	return result == ERROR_SUCCESS;
}

bool Input::GetKeyHold(KeyCode keyType)
{
	return s_keyTable[(uint8_t)keyType] & 0x80;
}

bool Input::GetKeyDown(KeyCode keyType)
{
	uint8_t key = (uint8_t)keyType;
	return (s_keyTable[key] ^ s_oldKeyTable[key]) & s_keyTable[key] & 0x80;
}

bool Input::GetKeyUp(KeyCode keyType)
{
	uint8_t key = (uint8_t)keyType;
	return (s_keyTable[key] ^ s_oldKeyTable[key]) & s_oldKeyTable[key] & 0x80;
}

bool Input::GetButtonHold(PadCode padCode)
{
	// トリガーデッドゾーン処理
	if (padCode == PadCode::LEFT_TRIGGER)
	{
		if (g_triggerDeadzone < s_padState.Gamepad.bLeftTrigger)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (padCode == PadCode::RIGHT_TRIGGER)
	{
		if (g_triggerDeadzone < s_padState.Gamepad.bRightTrigger)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return ((s_padState.Gamepad.wButtons) & (WORD)padCode) & (WORD)padCode;
}

bool Input::GetButtonDown(PadCode padCode)
{
	if (padCode == PadCode::LEFT_TRIGGER)
	{
		if (g_triggerDeadzone < s_padState.Gamepad.bLeftTrigger && g_triggerDeadzone >= s_oldPadState.Gamepad.bLeftTrigger)
		{
			return true;
		}
		else {
			return false;
		}
	}
	if (padCode == PadCode::RIGHT_TRIGGER)
	{
		if (g_triggerDeadzone < s_padState.Gamepad.bRightTrigger && g_triggerDeadzone >= s_oldPadState.Gamepad.bRightTrigger)
		{
			return true;
		}
		else {
			return false;
		}
	}
	return ((s_padState.Gamepad.wButtons) ^ (s_oldPadState.Gamepad.wButtons)) & (s_padState.Gamepad.wButtons) & (WORD)padCode;
}

bool Input::GetButtonUp(PadCode padCode)
{
	if (padCode == PadCode::LEFT_TRIGGER)
	{
		if (g_triggerDeadzone >= s_padState.Gamepad.bLeftTrigger && g_triggerDeadzone < s_oldPadState.Gamepad.bLeftTrigger)
		{
			return true;
		}
		else {
			return false;
		}
	}
	if (padCode == PadCode::RIGHT_TRIGGER)
	{
		if (g_triggerDeadzone >= s_padState.Gamepad.bRightTrigger && g_triggerDeadzone < s_oldPadState.Gamepad.bRightTrigger)
		{
			return true;
		}
		else {
			return false;
		}
	}
	return ((s_padState.Gamepad.wButtons) ^ (s_oldPadState.Gamepad.wButtons)) & (s_oldPadState.Gamepad.wButtons) & (WORD)padCode;
}

Vector2 Input::GetRightStick(float deadzone)
{
	if (deadzone >= 1.0f)
	{
		return Vector2::zero;
	}

	Vector2 input(s_padState.Gamepad.sThumbRX, s_padState.Gamepad.sThumbRY);
	input /= XINPUT_STICK_MAX;
	float magnitude = input.Magnitude();

	if (magnitude <= 1e-5f)
	{
		return Vector2::zero;
	}

	if (magnitude < deadzone)
	{
		return Vector2::zero;
	}

	float percent = (magnitude - deadzone) / (1.0f - deadzone);

	percent = std::clamp(percent, 0.0f, 1.0f);

	return input.Normalized() * percent;
}

Vector2 Input::GetLeftStick(float deadzone)
{
	if (deadzone >= 1.0f)
	{
		return Vector2::zero;
	}

	Vector2 input(s_padState.Gamepad.sThumbLX, s_padState.Gamepad.sThumbLY);
	input /= XINPUT_STICK_MAX;
	float magnitude = input.Magnitude();

	if (magnitude <= 1e-5f)
	{
		return Vector2::zero;
	}

	if (magnitude < deadzone)
	{
		return Vector2::zero;
	}

	float percent = (magnitude - deadzone) / (1.0f - deadzone);

	percent = std::clamp(percent, 0.0f, 1.0f);

	return input.Normalized() * percent;
}



Vector2 Input::GetLastRightStick(float deadzone)
{
	if (deadzone >= 1.0f)
	{
		return Vector2::zero;
	}

	Vector2 input(s_oldPadState.Gamepad.sThumbRX, s_oldPadState.Gamepad.sThumbRY);
	input /= XINPUT_STICK_MAX;
	float magnitude = input.Magnitude();

	if (magnitude <= 1e-5f)
	{
		return Vector2::zero;
	}

	if (magnitude < deadzone)
	{
		return Vector2::zero;
	}

	float percent = (magnitude - deadzone) / (1.0f - deadzone);

	percent = std::clamp(percent, 0.0f, 1.0f);

	return input.Normalized() * percent;
}

Vector2 Input::GetLastLeftStick(float deadzone)
{
	if (deadzone >= 1.0f)
	{
		return Vector2::zero;
	}

	Vector2 input(s_oldPadState.Gamepad.sThumbLX, s_oldPadState.Gamepad.sThumbLY);
	input /= XINPUT_STICK_MAX;
	float magnitude = input.Magnitude();

	if (magnitude <= 1e-5f)
	{
		return Vector2::zero;
	}

	if (magnitude < deadzone)
	{
		return Vector2::zero;
	}

	float percent = (magnitude - deadzone) / (1.0f - deadzone);

	percent = std::clamp(percent, 0.0f, 1.0f);

	return input.Normalized() * percent;
}


Vector2 Input::GetStick(StickCode stickCode, std::optional<float> deadzone)
{
	switch (stickCode)
	{
	case StickCode::LEFT:
		if (deadzone.has_value()) {
			return GetLeftStick(deadzone.value());
		}
		return GetLeftStick();
	case StickCode::RIGHT:
		if (deadzone.has_value()) {
			return GetRightStick(deadzone.value());
		}
		return GetRightStick();
	default:
		return Vector2::zero;
	}
}

Vector2 Input::GetMousePos()
{
	return s_mousePos;
}

Vector2 Input::GetMouseDelta()
{
	return s_mouseDelta;
}

void Input::LockMousePos(bool isLock)
{
	s_lockMousePos = isLock;
}

float Input::GetMouseWheelDelta()
{
	return s_mouseWheelDelta;
}

void Input::AddMouseWheelDelta(float delta)
{
	s_mouseWheelBuffer += delta;
}
