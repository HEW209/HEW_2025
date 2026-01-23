#include "Manual.h"
#include "Easing.h"
#include "InputManager.h"

constexpr float MANUAL_SIZE = 700.0f;

constexpr float DEFAULT_POS_Y = 5.0f;
constexpr float DISPLAY_POS_Y = 0.0f;

constexpr float BACK_RATIO = 1.7;

constexpr float DOWN_TIME = 0.5f;

Manual::Manual():
	m_currentState(State::DEFAULT),
	m_time(0.0f)
{
}

void Manual::Start()
{
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/Menu_sousasetumei.png");
	renderer->SetSize(MANUAL_SIZE);
	renderer->SetEnabled(false);
	renderer->SetOffsetPos(0.0f, 0.0f);// 最初は画面外に配置
	m_manual = renderer;

	auto transform = GetTransform();
	transform->SetPosition(0.0f, DEFAULT_POS_Y, 0.0f);
}

void Manual::Update()
{
	switch (m_currentState)
	{
	case Manual::DEFAULT:
		UpdateDefault();
		break;

	case Manual::DOWN:
		UpdateDown();
		break;

	case Manual::UP:
		UpdateUp();
		break;

	case Manual::DISPLAY:
		UpdateDisplay();
		break;
	}
}

void Manual::ChangeState(State state)
{
	m_currentState = state;
	m_time = 0.0f;

	if (state == State::DOWN)
	{
		m_manual->SetEnabled(true);
	}
}

void Manual::UpdateDefault()
{
}

void Manual::UpdateDown()
{
	m_time += Time::GetDeltaTime();
	if (m_time >= DOWN_TIME)
	{
		m_time = DOWN_TIME;
		m_currentState = State::DISPLAY;
	}
	auto transform = GetTransform();
	transform->SetPosition(0.0f, Easing::OutBack(m_time, DOWN_TIME, BACK_RATIO, DISPLAY_POS_Y, DEFAULT_POS_Y), 0.0f);
}

void Manual::UpdateUp()
{
	m_time += Time::GetDeltaTime();
	if (m_time >= DOWN_TIME)
	{
		m_time = DOWN_TIME;
		m_currentState = State::DEFAULT;
		m_manual->SetEnabled(false);
	}

	auto transform = GetTransform();
	transform->SetPosition(0.0f, Easing::InBack(m_time, DOWN_TIME, BACK_RATIO, DEFAULT_POS_Y, DISPLAY_POS_Y), 0.0f);
}

void Manual::UpdateDisplay()
{
	//メニューを閉じるボタンかメニューのキャンセルボタン押したとき
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuBack"_hash) || InputManager::CurrentInputSystem().GetButtonDown("MenuClose"_hash))
	{
		ChangeState(UP);
	}
}