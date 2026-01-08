//GuideUITimeController.cpp
#include "GuideUITimeController.h"
#include "GameState.h"
#include "InputManager.h"
#include "GuideUIResultController.h"

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

	for (int x = 0; x < 6; ++x)
	{
		sprite[x] = GetGameObject()->AddComponent<SpriteRenderer>();
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
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/Texts/Number.png");
	renderer1->SetOffsetPos(-0.5f, 0.0f);
	renderer1->SetUI(true);
	renderer1->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
	renderer1->SetUVOffsetPos(5.0f / 6.0f, 1.0f / 2.0f);
	renderer1->SetSize(g_spriteSize * 1.2f, g_spriteSize * 1.2f);
	sprite1[0] = renderer1;

	//点１
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/Texts/Number.png");
	renderer2->SetOffsetPos(0.55f, 0.0f);
	renderer2->SetUI(true);
	renderer2->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
	renderer2->SetUVOffsetPos(4.0f / 6.0f, 1.0f / 2.0f);
	renderer2->SetSize(g_spriteSize, g_spriteSize);
	sprite1[1] = renderer2;

	//点２
	auto renderer3 = GetGameObject()->AddComponent<SpriteRenderer>();
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
	if (m_b == true)
	{
	
		
			m_totalTime++;
			SetTimer();
			SetDigitUV();
		

	}

	

	GridField* gridfield = GameState::GetInstance()->GetGridField();

	if (gridfield->IsClear() && InputManager::CurrentInputSystem().GetButtonDown("Clear"_hash))
	{
		m_b2 = true;
	}

	if (m_b2)
	{
		m_time++;
	}

	if (m_time / 60 > 4)
	{
		m_b = false;
		for (int x = 0; x < 6; ++x)
		{
			// y -0.15fでした
			sprite[x]->GetTransform()->SetPosition(-1.5f + x * 0.1f, 0.25f, 0.0f);
			sprite[x]->GetTransform()->SetScale(1.5f, 1.5f, 0.0f);
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

