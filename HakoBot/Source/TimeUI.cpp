//TimeUI.cpp
#include "TimeUI.h"
#include "GameState.h"
#include "InputManager.h"
#include "GuideUIMenuController.h"

static const float g_digitSize = 48.0f;
static const float g_colonSize = 24.0f;
static const float g_spriteDistance = 0.3f;		// ”š“¯m‚Ì‹——£
static const float g_spriteSpace = 0.2f;		// •Ê’PˆÊ‚Ö‚Ì‹——£

TimeUI::TimeUI():
	m_digitRenderer(),
	m_colonRenderer()
{
}

void TimeUI::Awake()
{
	float PosX = 0.0f;

	for (int x = 0; x < 6; ++x)
	{
		auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/Texts/game_time.png");
		renderer->SetOffsetPos(PosX, 0.0f);
		PosX += g_spriteDistance;
		if (x % 2)
		{
			PosX += g_spriteSpace;
		}
		renderer->SetUVScale(1.0f / 5.0f, 1.0f / 2.0f);
		renderer->SetSize(g_digitSize, g_digitSize);
		m_digitRenderer[x] = renderer;
	}

	for (int x = 0; x < 2; ++x)
	{
		auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
		renderer->LoadTexture("Assets/Textures/Texts/game_colon.png");
		Vector2 pos = m_digitRenderer[x * 2 + 1]->GetOffsetPos();
		pos.x += g_spriteDistance * 0.5f + g_spriteSpace;
		renderer->SetOffsetPos(pos);
		renderer->SetUI(true);
		renderer->SetSize(g_colonSize);
		m_colonRenderer[x] = renderer;
	}
}

void TimeUI::Update()
{
	if (GameState::GetInstance()->IsClearEnter())
	{
		GetGameObject()->SetActive(false);
	}
}



void  TimeUI::SetDigitUV()
{

}

