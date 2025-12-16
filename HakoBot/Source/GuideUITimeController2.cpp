//GuideUITimeController2.cpp
#include "GuideUITimeController2.h"
#include "GameState.h"
#include "InputManager.h"
#include "TitleScene.h"
#include "GameScene.h"

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
	if (!isActive)
	{
		GridField* gridfield = GameState::GetInstance()->GetGridField();

		//if(Input::GetKeyDown(KeyCode::ENTER))
		if (gridfield->IsClear()&& InputManager::CurrentInputSystem().GetButtonDown("Clear"_hash))
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

	if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash))
	{
		if (m_curcirPos != 0)
		{
			m_curcirPos--;
		}
	}

	if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash))
	{
		if (m_curcirPos != 1)
		{
			m_curcirPos++;
		}
	}

	if (m_curcirPos == 0)
	{
		if(InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
			SceneManager::ChangeScene(std::make_unique<GameScene>("TestLevel01"));

		m_restart->SetColor(255.0f, 165.0f, 0.0f, 1.0f);
		m_stageSelect->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
	}
	if (m_curcirPos == 1)
	{
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
			SceneManager::ChangeScene(std::make_unique<TitleScene>());
		m_restart->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
		m_stageSelect->SetColor(255.0f, 165.0f, 0.0f, 1.0f);
	}
}
