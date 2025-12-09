#include "GuideUIController.h"
#include "Easing.h"




void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	//‚Ì‚Ñ‚é
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/nobiruRT.png");
	renderer1->SetOffsetPos(0.1f, -0.8f);
	renderer1->SetUI(true);
	renderer1->SetSize(MOZI_SIZE + 30.0f, MOZI_SIZE - 20.0f);
	//‚¿‚À‚Þ
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/tidimuLT.png");
	renderer2->SetOffsetPos(0.1f, -1.6f);
	renderer2->SetUI(true);
	renderer2->SetSize(MOZI_SIZE + 30.0f, MOZI_SIZE - 20.0f);


	//‰ŠúêŠŽæ“¾
	m_defaultPosition = GetTransform()->GetPosition();
	motu = false;
}

void GuideUIController::Update()
{
	if (Input::GetKeyHold(KeyCode::F))
	{
		if (!motu)
		{
			m_pRenderer->LoadTexture("Assets/Textures/motuB.png");
			m_pRenderer->SetSize(MOZI_SIZE + 70.0f, MOZI_SIZE + 20.0f);
			motu = true;
		}
		else
		{
			m_pRenderer->LoadTexture("Assets/Textures/okuB.png");
			m_pRenderer->SetSize(MOZI_SIZE + 70.0f, MOZI_SIZE + 20.0f);
			motu = false;
		}
	}
	
	if (Input::GetKeyHold(KeyCode::D))
	{
		m_value += EASING;

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

	
	
}
