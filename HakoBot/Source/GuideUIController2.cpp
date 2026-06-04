#include "GuideUIController2.h"
#include "Easing.h"
#include "GameState.h"

static const float g_easeDuration = 0.8f;

GuideUIController2::GuideUIController2() :
	m_renderer(nullptr),
	m_easeTime(0.0f)
{
}

void GuideUIController2::Start()
{
	m_renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	m_renderer->SetUI(true);
	m_renderer->LoadTexture("Assets/Textures/Button/kansei.png");
	m_renderer->SetSize(512.0f);
	m_renderer->SetOffsetPos(-3.0f, 2.0f);
}

void GuideUIController2::Update()
{
	if (GameState::GetInstance()->IsClear())
	{
		m_easeTime += Time::GetDeltaTime();
	}
	else
	{
		m_easeTime -= Time::GetDeltaTime();
	}

	// ƒNƒ‰ƒ“ƒv
	if (m_easeTime > g_easeDuration)
	{
		m_easeTime = g_easeDuration;
	}
	if (m_easeTime < 0.0f)
	{
		m_easeTime = 0.0f;
	}

	float e = Easing::OutBack(m_easeTime, g_easeDuration, 1.7f, 0.0f, 1.0f);
	GetTransform()->SetEulerAngle(0.0f, 0.0f, e * 90.0f);

	if (GameState::GetInstance()->IsClearEnter())
	{
		GetGameObject()->SetActive(false);
	}
}
