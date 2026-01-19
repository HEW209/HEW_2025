#include "InputManager.h"

std::array<InputSystem, static_cast<size_t>(InputBindType::MAX_COUNT)> InputManager::m_inputSystems;
InputBindType InputManager::m_currentBindType = InputBindType::GAMEPLAY;

void InputManager::Init()
{
	// ゲームプレイ用入力システム初期化
	{
		InputSystem& system = m_inputSystems[static_cast<size_t>(InputBindType::GAMEPLAY)];

		system.CreateButtonAction("RotateBlockUp"_hash);
		system.CreateButtonAction("RotateBlockDown"_hash);
		system.CreateButtonAction("RotateBlockLeft"_hash);
		system.CreateButtonAction("RotateBlockRight"_hash);
		system.CreateButtonAction("PlaceAndRemove"_hash);
		system.CreateButtonAction("Up"_hash);
		system.CreateButtonAction("Down"_hash);
		system.CreateButtonAction("Clear"_hash);
		system.CreateButtonAction("Menu"_hash);
		system.CreateButtonAction("LockRotation"_hash);
		system.CreateButtonAction("ChangeBlockTransparency"_hash);
		system.CreateButtonAction("Result"_hash);
		system.CreateButtonAction("ResultSelect"_hash);
		system.CreateAxisAction("Move"_hash);
		system.CreateAxisAction("CameraMove"_hash);

		system.BindKey("RotateBlockUp"_hash, KeyCode::UP);
		system.BindKey("RotateBlockDown"_hash, KeyCode::DOWN);
		system.BindKey("RotateBlockLeft"_hash, KeyCode::LEFT);
		system.BindKey("RotateBlockRight"_hash, KeyCode::RIGHT);
		system.BindKey("PlaceAndRemove"_hash, KeyCode::SPACE);
		system.BindKey("Up"_hash, KeyCode::E);
		system.BindKey("Down"_hash, KeyCode::Q);
		system.BindKey("Clear"_hash, KeyCode::X);
		system.BindKey("Menu"_hash, KeyCode::ESC);
		system.BindKey("LockRotation"_hash, KeyCode::SHIFT);
		system.BindKey("ChangeBlockTransparency"_hash, KeyCode::R);
		system.BindKey("Result"_hash, KeyCode::P);
		system.BindKey("ResultSelect"_hash, KeyCode::ENTER);

		system.BindVectorKeys("Move"_hash, KeyCode::W, KeyCode::S, KeyCode::A, KeyCode::D);
		system.BindVectorKeys("CameraMove"_hash, KeyCode::I, KeyCode::K, KeyCode::MOUSE_LEFT, KeyCode::MOUSE_RIGHT);

		system.BindPadButton("RotateBlockUp"_hash, PadCode::UP);
		system.BindPadButton("RotateBlockDown"_hash, PadCode::DOWN);
		system.BindPadButton("RotateBlockLeft"_hash, PadCode::LEFT_SHOULDER);
		system.BindPadButton("RotateBlockRight"_hash, PadCode::RIGHT_SHOULDER);
		system.BindPadButton("PlaceAndRemove"_hash, PadCode::B);
		system.BindPadButton("Up"_hash, PadCode::RIGHT_TRIGGER);
		system.BindPadButton("Down"_hash, PadCode::LEFT_TRIGGER);
		system.BindPadButton("Clear"_hash, PadCode::X);
		system.BindPadButton("Menu"_hash, PadCode::START);
		system.BindPadButton("LockRotation"_hash, PadCode::Y);
		system.BindPadButton("ChangeBlockTransparency"_hash, PadCode::A);
		system.BindPadButton("ResultSelect"_hash, PadCode::B);
		system.BindPadStick("Move"_hash, StickCode::LEFT);
		system.BindPadStick("CameraMove"_hash, StickCode::RIGHT);
	}

	// UI操作用入力システム初期化
	{
		InputSystem& system = m_inputSystems[static_cast<size_t>(InputBindType::UI)];

		system.CreateButtonAction("MenuBack"_hash);
		system.CreateButtonAction("MenuUp"_hash);
		system.CreateButtonAction("MenuDown"_hash);
		system.CreateButtonAction("MenuLeft"_hash);
		system.CreateButtonAction("MenuRight"_hash);
		system.CreateButtonAction("MenuInteract"_hash);

		system.BindKey("MenuBack"_hash, KeyCode::Z);
		system.BindKey("MenuUp"_hash, KeyCode::UP);
		system.BindKey("MenuDown"_hash, KeyCode::DOWN);
		system.BindKey("MenuLeft"_hash, KeyCode::LEFT);
		system.BindKey("MenuRight"_hash, KeyCode::RIGHT);
		system.BindKey("MenuInteract"_hash, KeyCode::ENTER);

		system.BindPadButton("MenuBack"_hash, PadCode::A);
		system.BindPadButton("MenuUp"_hash, PadCode::UP);
		system.BindPadButton("MenuDown"_hash, PadCode::DOWN);
		system.BindPadButton("MenuLeft"_hash, PadCode::LEFT);
		system.BindPadButton("MenuRight"_hash, PadCode::RIGHT);
		system.BindPadButton("MenuInteract"_hash, PadCode::B);
	}
}