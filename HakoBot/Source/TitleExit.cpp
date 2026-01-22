#include "TitleExit.h"
#include <memory>
#include "StageSelectScene.h"
#include "InputManager.h"
#include "SaveData.h"
#include "Fade.h"
#include "SoundManager.h"

TitleExit::TitleExit():
	m_newGameText(nullptr),
	m_continueText(nullptr),
	m_isNewGame(true),
	m_startFlag(false)
{
}

void TitleExit::Start()
{
	m_newGameText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara.png");
	m_newGameText->SetSize(300.0f);
	m_newGameText->SetOffsetPos(0.0f, -0.5f);
	m_newGameText->SetUI(true);

	m_continueText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara_hide.png");
	m_continueText->SetSize(300.0f);
	m_continueText->SetOffsetPos(0.0f, 0.5f);
	m_continueText->SetUI(true);

	// セーブデータが存在すれば初期カーソルをつづきからに
	if (SaveData::Find())
	{
		m_isNewGame = false;
	}
	else
	{
		m_isNewGame = true;
	}
}

void TitleExit::Update()
{
	if (m_startFlag)
	{
		if (Fade::IsActive())
			return;

		if (m_isNewGame)
		{
			SaveData::Reset();
			InputManager::ChangeBindType(InputBindType::GAMEPLAY);
			SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
		}
		else
		{
			// つづきから
			SaveData::Load();
			InputManager::ChangeBindType(InputBindType::GAMEPLAY);
			SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
		}
	}
	else
	{
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash))
		{
			SoundManager::PlaySE("Title_Select", 1.0f, false);
			m_isNewGame = !m_isNewGame;
		}
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash))
		{
			SoundManager::PlaySE("Title_Select", 1.0f, false);
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
			SoundManager::StopBGM();
			SoundManager::PlaySE("Title_Decision", 1.0f, false);
			m_startFlag = true;
			Fade::StartIconIrisOut();
		}
	}
}
