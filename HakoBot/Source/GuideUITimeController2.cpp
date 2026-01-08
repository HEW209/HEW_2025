//GuideUITimeController2.cpp
#include "GuideUITimeController2.h"
#include "GameState.h"
#include "InputManager.h"
#include "StageSelectScene.h"
#include "GameScene.h"
#include "Fade.h"
#include "SaveData.h"

GuideUITimeController2::GuideUITimeController2():
	m_sceneChangeFlag(false)
{
}

void GuideUITimeController2::Start()
{
	m_b3 = false;

	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->SetUI(true);
	renderer1->LoadTexture("Assets/Textures/restart.png");
	renderer1->SetOffsetPos(0.0f, -3.3f);
	m_restart = renderer1;

	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->SetUI(true);
	renderer2->LoadTexture("Assets/Textures/stageselectback.png");
	renderer2->SetOffsetPos(0.0f, -5.0f);
	m_stageSelect = renderer2;

	m_curcirPos = 0;
	isActive = false;
}

void GuideUITimeController2::Update()
{
	if (m_sceneChangeFlag)
	{
		if (Fade::IsActive())
			return;

		int stageNo = GameState::GetCurrentStegaNo();
		if (stageNo > SaveData::GetClearLevel())
		{
			SaveData::SetClearLevel(stageNo);
			SaveData::Save();
		}

		if (m_curcirPos == 0)
		{
			std::string levelName = GameState::GetInstance()->GetLevelName();
			SceneManager::ChangeScene(std::make_unique<GameScene>(levelName));
		}
		if (m_curcirPos == 1)
		{
			SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
		}
	}
	else
	{
		if (!isActive)
		{
			GridField* gridfield = GameState::GetInstance()->GetGridField();

			//if(Input::GetKeyDown(KeyCode::ENTER))
			if (gridfield->IsClear() && InputManager::CurrentInputSystem().GetButtonDown("Clear"_hash))
			{
				m_b3 = true;

			}

			if (m_b3)
			{
				m_time2++;
			}

			if (m_time2 / 60 > 4)
			{
				isActive = true;
				GetTransform()->SetScale(0.2f, 0.2f, 0.0f);
				InputManager::ChangeBindType(InputBindType::UI);
			}
			return;
		}

		Vector2 input = Input::GetLeftStick();

		if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash) || input.y > 0.0f)
		{
			if (m_curcirPos != 0)
			{
				m_curcirPos--;
			}
		}
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash) || input.y < 0.0f)
		{
			if (m_curcirPos != 1)
			{
				m_curcirPos++;
			}
		}

		if (m_curcirPos == 0)
		{
			m_restart->SetColor(255.0f, 165.0f, 0.0f, 1.0f);
			m_stageSelect->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
		}
		if (m_curcirPos == 1)
		{
			m_restart->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
			m_stageSelect->SetColor(255.0f, 165.0f, 0.0f, 1.0f);
		}

		if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
		{
			m_sceneChangeFlag = true;
			Fade::StartIrisOut();
		}
	}
}
