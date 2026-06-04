#include "TitleLogo.h"

static const float g_spriteSize = 800.0f;
static const float g_moveCycle = 2.0f;
static const float g_moveTime = 0.3f;
static const float g_moveDistance = 0.5f;

TitleLogo::TitleLogo() :
	m_TitleLeft(nullptr),
	m_TitleRight(nullptr),
	m_leftMoveTimer(0.0f),
	m_RightMoveTimer(0.0f)
{
}

void TitleLogo::Start()
{
	{
		const char* assetPath[3] =
		{
			"Assets/Textures/Title/title_botsirowakukage.png",
			"Assets/Textures/Title/title_botsirowaku.png",
			"Assets/Textures/Title/title_bot.png"
		};

		m_TitleRight = SceneManager::GetActiveScene()->CreateGameObject();
		m_TitleRight->GetTransform()->SetParent(GetTransform());
		for (int i = 0; i < 3; ++i)
		{
			auto renderer = m_TitleRight->AddComponent<SpriteRenderer>();
			renderer->LoadTexture(assetPath[i]);
			renderer->SetSize(g_spriteSize);
			renderer->SetOrder(i);
			renderer->SetUI(true);
		}
	}

	{
		const char* assetPath[3] =
		{
			"Assets/Textures/Title/title_hakosirowakukage.png",
			"Assets/Textures/Title/title_hakowaku.png",
			"Assets/Textures/Title/title_hako.png"
		};

		m_TitleLeft = SceneManager::GetActiveScene()->CreateGameObject();
		m_TitleLeft->GetTransform()->SetParent(GetTransform());
		for (int i = 0; i < 3; ++i)
		{
			auto renderer = m_TitleLeft->AddComponent<SpriteRenderer>();
			renderer->LoadTexture(assetPath[i]);
			renderer->SetSize(g_spriteSize);
			renderer->SetOrder(i);
			renderer->SetUI(true);
		}
	}

	// オフセットタイム
	m_RightMoveTimer -= 0.2f;
}

void TitleLogo::Update()
{
	m_leftMoveTimer += Time::GetDeltaTime();
	m_RightMoveTimer += Time::GetDeltaTime();
	float moveTime = g_moveCycle - g_moveTime;

	if (m_leftMoveTimer > moveTime)
	{
		float currentTime = m_leftMoveTimer - moveTime;
		float offset_y = std::sinf((currentTime / g_moveTime) * Math::PI);
		offset_y = std::fabsf(offset_y);
		offset_y *= g_moveDistance;

		m_TitleLeft->GetTransform()->SetPosition(0.0f, offset_y, 0.0f, Space::LOCAL);
	}
	if (m_leftMoveTimer > g_moveCycle)
	{
		m_leftMoveTimer -= g_moveCycle;
	}

	if (m_RightMoveTimer > moveTime)
	{
		float currentTime = m_RightMoveTimer - moveTime;
		float offset_y = std::sinf((currentTime / g_moveTime) * Math::PI);
		offset_y = std::fabsf(offset_y);
		offset_y *= g_moveDistance;

		m_TitleRight->GetTransform()->SetPosition(0.0f, offset_y, 0.0f, Space::LOCAL);
	}
	if (m_RightMoveTimer > g_moveCycle)
	{
		m_RightMoveTimer -= g_moveCycle;
	}
}
