#include "GuideUIController.h"
#include "Easing.h"

//イージング関数台に引数用
#define EASING_MAX (5.0f)


void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	//のびる
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/nobiruRT.png");
	renderer1->SetOffsetPos(0.1f, -0.8f);
	renderer1->SetUI(true);
	renderer1->SetSize(MOZI_SIZE + 30.0f, MOZI_SIZE - 20.0f);
	//ちぢむ
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/tidimuLT.png");
	renderer2->SetOffsetPos(0.1f, -1.6f);
	renderer2->SetUI(true);
	renderer2->SetSize(MOZI_SIZE + 30.0f, MOZI_SIZE - 20.0f);


	//初期場所取得
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

	/*Vector3 offset = Vector3::zero;
	offset.y = Easing::OutQuad(m_value, 5.0f, 1.5f, 0.0f);
	GetTransform()->SetPosition(m_defaultPosition + offset);*/
	
}
