#pragma once
#include <array>

#include "InputSystem.h"


enum class InputBindType
{
	GAMEPLAY,
	UI,
	MAX_COUNT
};

class InputManager
{
public:
	static const InputSystem& CurrentInputSystem() {
		return m_inputSystems[static_cast<size_t>(m_currentBindType)];
	}

	static void ChangeBindType(InputBindType type) {
		if (type == InputBindType::MAX_COUNT) {
			return;
		}
		m_currentBindType = type;
	}

	static InputBindType GetCurrentBindType() {
		return m_currentBindType;
	}

	static void Init();

	static void Update() {
		for (auto& system : m_inputSystems) {
			system.Update();
		}
	}

private:
	static std::array<InputSystem, static_cast<size_t>(InputBindType::MAX_COUNT)> m_inputSystems;
	static InputBindType m_currentBindType;
};