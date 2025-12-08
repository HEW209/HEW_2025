//GuideUITimeController.cpp
#include "GuideUITimeController.h"
#include "GuideUIController.h"

void GuideUITimeController::Start()
{
	//m_pRenderer1 = GetGameObject()->GetComponent<SpriteRenderer>();
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/result2.png");
	renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	GetTransform()->SetScale(0.0f, 0.0f, 0.0f);

	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();

}

void GuideUITimeController::Update()
{
	if (Input::GetKeyHold(KeyCode::R))
	{
		m_value += EASING * 2.0f;

		if (m_value > EASING_MAX)
		{
			m_value = EASING_MAX;
		}

		
	}
	else
	{
		m_value -= EASING;

		if (m_value < 0.0f)
		{
			m_value = 0.0f;
		}
	}

	//スケール変更
	Vector3 offset = Vector3::zero;
	offset.x = Easing::InSine(m_value,EASING_MAX,0.3f,0.0f);
	offset.y = Easing::InSine(m_value,EASING_MAX, 0.3f, 0.0f);
	GetTransform()->SetScale(m_defaultScale + offset);//0.3,0.3

	////ポジション変更
	//Vector3 offset2 = Vector3::zero;
	//offset2.y = Easing::InSine(m_value, EASING_MAX, -6.0f, 0.0f);
	//GetTransform()->SetPosition(m_defaultPosition + offset2);

}



