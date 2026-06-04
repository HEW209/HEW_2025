//TimeUI.cpp
#include "TimeUI.h"
#include "GameState.h"
#include "InputManager.h"
#include "GuideUIMenuController.h"

static const float g_digitSize = 36.0f;
static const float g_colonSize = 18.0f;
static const float g_spriteDistance = 0.33f;	// 数字同士の距離
static const float g_spriteSpace = 0.12f;		// 別単位への距離

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
		pos.x += (g_spriteDistance + g_spriteSpace) * 0.5f;
		renderer->SetOffsetPos(pos);
		renderer->SetUI(true);
		renderer->SetSize(g_colonSize);
		m_colonRenderer[x] = renderer;
	}
}

void TimeUI::Update()
{
	SetDigitUV();
}

void TimeUI::SetTextureType(TextureType textureType)
{
	for (int i = 0; i < 6; ++i)
	{
		switch (textureType)
		{
		case TimeUI::TextureType::GAME:
			m_digitRenderer[i]->LoadTexture("Assets/Textures/Texts/game_time.png", false);
			break;

		case TimeUI::TextureType::RESULT:
			m_digitRenderer[i]->LoadTexture("Assets/Textures/Texts/result_time.png", false);
			break;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		switch (textureType)
		{
		case TimeUI::TextureType::GAME:
			m_colonRenderer[i]->LoadTexture("Assets/Textures/Texts/game_colon.png", false);
			break;

		case TimeUI::TextureType::RESULT:
			m_colonRenderer[i]->LoadTexture("Assets/Textures/Texts/result_colon.png", false);
			break;
		}
	}
}

void  TimeUI::SetDigitUV()
{
	int currentTime = GameState::GetInstance()->GetGameTime();

	// 各桁の数字を求める
	int digit[6];
	int digitIndex = 5;
	for (int i = 0; i < 3; ++i)
	{
		int number = currentTime % 60;
		currentTime /= 60;

		digit[digitIndex] = number % 10;
		digitIndex--;
		digit[digitIndex] = number / 10;
		digitIndex--;
	}

	// UV更新
	for (int i = 0; i < 6; ++i)
	{
		Vector2 uvPos = m_digitRenderer[i]->GetUVScale();
		int uvIndex = digit[i] - 1;
		if (uvIndex < 0)
			uvIndex += 10;

		uvPos.x *= uvIndex % 5;
		uvPos.y *= uvIndex / 5;

		m_digitRenderer[i]->SetUVOffsetPos(uvPos);
	}
}

