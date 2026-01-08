#include "TitleExit.h"
#include <memory>
#include "StageSelectScene.h"
#include "InputManager.h"
#include "SaveData.h"

TitleExit::TitleExit():
	m_newGameText(nullptr),
	m_continueText(nullptr),
	m_isNewGame(true)
{
}

void TitleExit::Start()
{
	m_newGameText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara.png");
	m_newGameText->SetSize(300.0f);
	m_newGameText->SetOffsetPos(0.0f, 0.5f);
	m_newGameText->SetUI(true);

	m_continueText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara_hide.png");
	m_continueText->SetSize(300.0f);
	m_continueText->SetOffsetPos(0.0f, -0.5f);
	m_continueText->SetUI(true);
}

void TitleExit::Update()
{
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash))
	{
		m_isNewGame = !m_isNewGame;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash))
	{
		m_isNewGame = !m_isNewGame;
	}

	Vector2 currentInput = Input::GetLeftStick(0.5f);
	if (currentInput.y > 0.0f && m_lastInput.y <= 0.0f)
	{
		m_isNewGame = !m_isNewGame;
	}
	if (currentInput.y < 0.0f && m_lastInput.y >= 0.0f)
	{
		m_isNewGame = !m_isNewGame;
	}
	m_lastInput = currentInput;

	if (m_isNewGame)
	{
		m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara.png", false);
		m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara_hide.png", false);
	}
	else
	{
		m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara_hide.png", false);
		m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara.png", false);
	}

	if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
	{
		if (m_isNewGame)
		{
			SaveData::Reset();
			InputManager::ChangeBindType(InputBindType::GAMEPLAY);
			SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
		}
		else
		{
			// ‚Â‚Ã‚«‚©‚ç
			SaveData::Load();
			InputManager::ChangeBindType(InputBindType::GAMEPLAY);
			SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
		}
	}
}
