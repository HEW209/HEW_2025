#include "TitleExit.h"
#include <memory>
#include "StageSelectScene.h"
#include "InputManager.h"
#include "SaveData.h"
#include "Fade.h"
#include "SoundManager.h"

static const float g_textSize = 300.0f;
static const float g_startAnimeDuration = 0.3f;

TitleExit::TitleExit():
	m_newGameText(nullptr),
	m_continueText(nullptr),
	m_isNewGame(true),
	m_startFlag(false),
	m_animeTimer(0.0f)
{
}

void TitleExit::Start()
{
	m_newGameText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara.png");
	m_newGameText->SetSize(g_textSize);
	m_newGameText->SetOffsetPos(0.0f, -0.5f);
	m_newGameText->SetUI(true);

	m_continueText = GetGameObject()->AddComponent<SpriteRenderer>();
	m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara_hide.png");
	m_continueText->SetSize(g_textSize);
	m_continueText->SetOffsetPos(0.0f, 0.5f);
	m_continueText->SetUI(true);

	auto button = GetGameObject()->AddComponent<SpriteRenderer>();
	button->LoadTexture("Assets/Textures/Button/Button_B.png");
	button->SetSize(60.0f);
	button->SetOffsetPos(-0.45f, -1.4f);
	button->SetUI(true);

	auto text = GetGameObject()->AddComponent<SpriteRenderer>();
	text->LoadTexture("Assets/Textures/Texts/kettei.png");
	text->SetSize(240.0f);
	text->SetOffsetPos(0.25f, -1.4f);
	text->SetUI(true);

	// セーブデータが存在すれば初期カーソルをつづきからに
	if (SaveData::Find())
	{
		m_isNewGame = false;
	}
	else
	{
		m_isNewGame = true;
		m_newGameText->SetOffsetPos(0.0f, 0.0f);
		m_continueText->SetEnabled(false);
	}
}

void TitleExit::Update()
{
	if (m_startFlag)
	{
		// スケーリングアニメーション
		m_animeTimer += Time::GetDeltaTime();
		if (m_animeTimer > g_startAnimeDuration)
			m_animeTimer = g_startAnimeDuration;

		float easeRatio = m_animeTimer / g_startAnimeDuration;
		float size = g_textSize * (easeRatio * 2.0f + 1.0f);
		if (m_isNewGame)
		{
			m_newGameText->SetColor(1.0f, 1.0f, 1.0f, 1.0f - easeRatio);
			m_newGameText->SetSize(size);
		}
		else
		{
			m_continueText->SetColor(1.0f, 1.0f, 1.0f, 1.0f - easeRatio);
			m_continueText->SetSize(size);
		}


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
		// カーソル移動
		if (SaveData::Find())
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
				SoundManager::PlaySE("Title_Select", 1.0f, false);
				m_isNewGame = !m_isNewGame;
			}
			if (currentInput.y < 0.0f && m_lastInput.y >= 0.0f)
			{
				SoundManager::PlaySE("Title_Select", 1.0f, false);
				m_isNewGame = !m_isNewGame;
			}
			m_lastInput = currentInput;
		}

		m_animeTimer += Time::GetDeltaTime();
		float scale = std::fabsf(std::sinf(m_animeTimer * Math::TAU / 3.0f));
		scale = scale * 0.1f + 1.0f;

		if (m_isNewGame)
		{
			m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara.png", false);
			m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara_hide.png", false);
			m_newGameText->SetSize(g_textSize * scale);
			m_continueText->SetSize(g_textSize);
		}
		else
		{
			m_newGameText->LoadTexture("Assets/Textures/Texts/hajimekara_hide.png", false);
			m_continueText->LoadTexture("Assets/Textures/Texts/tudukikara.png", false);
			m_newGameText->SetSize(g_textSize);
			m_continueText->SetSize(g_textSize * scale);
		}

		if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
		{
			SoundManager::StopBGM();
			SoundManager::PlaySE("Title_Decision", 1.0f, false);
			m_animeTimer = 0.0f;
			m_startFlag = true;
			Fade::StartIconIrisOut();
		}

		if (Input::GetKeyDown(KeyCode::ESC) ||
			Input::GetButtonDown(PadCode::BACK))
		{
			exit(0);
		}
	}
}
