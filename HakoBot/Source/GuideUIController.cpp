#include "GuideUIController.h"
#include "Easing.h"

//イージング関数台に引数用
#define EASING_MAX (5.0f)


void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/nobiru.png");
	renderer1->SetOffsetPos(0.0f, -0.6f);
	renderer1->SetUI(true);
	renderer1->SetSize(MOZI_SIZE, MOZI_SIZE);
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/tidimu.png");
	renderer2->SetOffsetPos(0.0f, -1.2f);
	renderer2->SetUI(true);
	renderer2->SetSize(MOZI_SIZE, MOZI_SIZE);
	m_defaultPosition = GetTransform()->GetPosition();

}

void GuideUIController::Update()
{


	if (Input::GetKeyHold(KeyCode::D))
	{
		m_value += 0.1f;

		if (m_value > 5.0f)
		{
			m_value = 5.0f;
		}
	}
	else
	{
		m_value -= 0.1f;
		if (m_value < 0.0f)
		{
			m_value = 0.0f;
		}
	}

	Vector3 offset = Vector3::zero;
	offset.y = Easing::OutQuad(m_value,5.0f,1.0f,0.0f);
	GetTransform()->SetPosition(m_defaultPosition + offset);


	/*m_time += 1.0f / 60.0f;

	if (m_time > 10.0f)
	{
		m_time = 10.0f;
	}

	m_pRenderer->GetTransform()->SetPosition(Easing::InSine(m_time, 10.0f, 2.0f, 1.0f), 1.5f, 0.0f);*/
}
