//GuideUIResultController.cpp
#include "GuideUIResultController.h"
#include "GuideUIController.h"
#include "Easing.h"
#include "InputSystem.h"
#include "GridField.h"
#include "GameState.h"

void GuideUIResultController::Start()
{
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/menukiban.png");
	renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	GetTransform()->SetScale(0.0f, 0.0f, 0.0f);

	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->SetUI(true);
	renderer1->LoadTexture("Assets/Textures/restart.png");
	renderer1->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	renderer1->SetOffsetPos(0.0f,2.0f);
	m_rend = renderer1;

	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->SetUI(true);
	renderer2->LoadTexture("Assets/Textures/stageselectback.png");
	renderer2->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	renderer2->SetOffsetPos(0.0f, -1.0f);
	m_rend2 = renderer2;

	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	m_menu = false;
	m_menu2 = false;
}

void GuideUIResultController::Update()
{
	//メニューをひらく
	if (InputSystem::GetButtonDown("Menu"_hash))
	{
		GridField* gridfield = GameState::GetInstance()->GetGridField();
		//クリアしてたらメニュー表示できない
		if (!gridfield->IsClear())
		{
			m_menu = true;
		}
		m_rend->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
		m_rend2->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
	}

	//メニュー開いてるとき
	if (m_menu)
	{
		//メニュー出現
		if (!m_menu2)
		{
			m_value += EASING * 2.0f;

			if (m_value > EASING_MAX)
			{
				m_value = EASING_MAX;
			}
		}

		//メニュー縮小
		if (m_menu2)
		{
			m_value -= EASING;

			if (m_value <= 0.0f)
			{
				m_value = 0.0f;
				m_menu = false;
				m_menu2 = false;
			}
		}

		if (InputSystem::GetButtonDown("MenuBack"_hash))
		{
			m_menu2 = true;		//でかくするイージングoff
		}

		//上を選択したとき、リスタートをオレンジに
		if (InputSystem::GetButtonDown("MenuUp"_hash))
		{
			m_rend->SetColor(255.0f,165.0f,0.0f,1.0f);
			m_rend2->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
		}
		//下を選択したとき、ステージ選択に戻るをオレンジに
		if (InputSystem::GetButtonDown("MenuDown"_hash))
		{
			m_rend2->SetColor(255.0f, 165.0f, 0.0f, 1.0f);
			m_rend->SetColor(255.0f, 255.0f, 255.0f, 1.0f);
		}
	}

	//スケール変更
	Vector3 offset = Vector3::zero;
	offset.x = Easing::InSine(m_value, EASING_MAX, 0.3f, 0.0f);
	offset.y = Easing::InSine(m_value, EASING_MAX, 0.3f, 0.0f);
	GetTransform()->SetScale(m_defaultScale + offset);


}