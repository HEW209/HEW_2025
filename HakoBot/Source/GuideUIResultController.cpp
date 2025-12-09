//GuideUIResultController.cpp
#include "GuideUIResultController.h"
#include "GuideUIController.h"
#include "Easing.h"
#include "InputSystem.h"

void GuideUIResultController::Start()
{
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/menukiban.png");
	renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	GetTransform()->SetScale(0.0f, 0.0f, 0.0f);

	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->SetUI(true);
	renderer1->LoadTexture("Assets/Textures/menukiban.png");
	renderer1->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);

	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	m_menu = false;
	m_menu2 = false;
}

void GuideUIResultController::Update()
{
	if (InputSystem::GetButtonDown("Menu"_hash))
	{
		m_menu = true;
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
	}

	//スケール変更
	Vector3 offset = Vector3::zero;
	offset.x = Easing::InSine(m_value, EASING_MAX, 0.3f, 0.0f);
	offset.y = Easing::InSine(m_value, EASING_MAX, 0.3f, 0.0f);
	GetTransform()->SetScale(m_defaultScale + offset);


}