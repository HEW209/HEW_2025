//GuideUITimeController.cpp
#include "GuideUITimeController.h"
#include "GameState.h"
#include "InputManager.h"
#include "GuideUIMenuController.h"

static const float g_spriteSize = 48.0f;

static const int digitToIndex[10] =
{
0,  // 0の位置
1,  // 1
2,  // 2
3,  // 3
4,  // 4
5,  // 5
6,  // 6
7,  // 7
8,  // 8
9   // 9
};

void GuideUITimeController::Start()
{
	float PosX = 0.0f;

	m_root = SceneManager::GetActiveScene()->CreateGameObject();
	m_root->GetTransform()->SetParent(GetTransform());

	for (int x = 0; x < 6; ++x)
	{
		sprite[x] = m_root->AddComponent<SpriteRenderer>();
		sprite[x]->SetUI(true);
		sprite[x]->LoadTexture("Assets/Textures/Texts/Number.png");
		sprite[x]->SetOffsetPos(PosX,0.0f);
		PosX += 0.3f;
		if (x % 2)
		{
			PosX += 0.2f;
		}
		sprite[x]->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
		sprite[x]->SetSize(g_spriteSize, g_spriteSize);
	}

	
	//時計マーク
	auto renderer1 = m_root->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/Texts/Number.png");
	renderer1->SetOffsetPos(-0.5f, 0.0f);
	renderer1->SetUI(true);
	renderer1->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
	renderer1->SetUVOffsetPos(5.0f / 6.0f, 1.0f / 2.0f);
	renderer1->SetSize(g_spriteSize * 1.2f, g_spriteSize * 1.2f);
	sprite1[0] = renderer1;

	//点１
	auto renderer2 = m_root->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/Texts/Number.png");
	renderer2->SetOffsetPos(0.55f, 0.0f);
	renderer2->SetUI(true);
	renderer2->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
	renderer2->SetUVOffsetPos(4.0f / 6.0f, 1.0f / 2.0f);
	renderer2->SetSize(g_spriteSize, g_spriteSize);
	sprite1[1] = renderer2;

	//点２
	auto renderer3 = m_root->AddComponent<SpriteRenderer>();
	renderer3->LoadTexture("Assets/Textures/Texts/Number.png");
	renderer3->SetOffsetPos(1.35f, 0.0f);
	renderer3->SetUI(true);
	renderer3->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
	renderer3->SetUVOffsetPos(4.0f / 6.0f, 1.0f / 2.0f);
	renderer3->SetSize(g_spriteSize, g_spriteSize);
	sprite1[2] = renderer3;

	m_totalTime = 0;
	m_b = true;
	m_b2 = false;
}

void GuideUITimeController::Update()
{
	if (m_totalTime < std::numeric_limits<int>::max()) {
		++m_totalTime;
	}
	if (m_b)
	{	
		SetTimer();
		SetDigitUV();
	}

	if (GameState::GetInstance()->IsClearEnter())
	{
		m_b = false;
		int clearTime = GameState::GetInstance()->GetClearTime();
		if (clearTime == 0) {
			clearTime = m_totalTime;
			GameState::GetInstance()->SetClearTime(clearTime);
			m_root->SetActive(false);
		}
		if (m_totalTime - clearTime >= 100) {
			m_root->SetActive(true);
			float rate = Math::Clamp(0.0f, 5.0f, (m_totalTime - clearTime - 100) * 0.2f);
			GetTransform()->SetPosition(Easing::InSine(rate, 5.0f, 3.0f, 5.75f),
				Easing::InSine(rate, 5.0f, 2.0f, 2.0f), 0.0f);
		}
	}
}



void  GuideUITimeController::SetDigitUV()
{
	// 1マスのUVサイズ
	const float uSize = 1.0f / 6.0f;
	const float vSize = 1.0f / 2.0f;

	for (int x = 0; x < 6; ++x)
	{
		int index = digitToIndex[m_digit[x]];

		float u = (index % 6) * uSize;
		float v = (index / 6) * vSize;

		sprite[x]->SetUVOffsetPos(u, v);
	}
}

void GuideUITimeController::SetTimer()
{
	int totalSeconds = m_totalTime / 60;

	int h = totalSeconds / 3600;
	int m = (totalSeconds / 60) % 60;
	int s = totalSeconds % 60;

	m_digit[0] = h / 10;
	m_digit[1] = h % 10;//1が一桁目、2が二桁目
	m_digit[2] = m / 10;
	m_digit[3] = m % 10;
	m_digit[4] = s / 10;
	m_digit[5] = s % 10;

}

