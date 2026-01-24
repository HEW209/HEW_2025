#include "StageArrow.h"
#include "Easing.h"
#include "StageNumber.h"
#include "SaveData.h"

//StageArrow.cpp

static const float g_hideTime = 0.7f;
static const float g_hideChangeTime = 0.3f;

StageArrow::StageArrow():
	m_ease(0.25f),
	m_hideTimer(g_hideChangeTime)
{
}

void StageArrow::Start()
{
	//矢印左
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->LoadTexture("Assets/Textures/StageSelect/Arrow.png");
	renderer->SetOffsetPos(-2.5f,-1.1f);
	renderer->SetSize(160.0f);
	renderer->SetUI(true);
	m_ArrowLeft = renderer;

	//矢印右
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/StageSelect/Arrow.png");
	renderer1->SetOffsetPos(2.5f, -1.1f);
	renderer1->SetSize(160.0f);
	renderer1->SetUVOffsetPos(1.0f, 1.0f);
	renderer1->SetUVScale(-1.0f, -1.0f);
	renderer1->SetUI(true);
	m_ArrowRight = renderer1;

	m_lastSelectNumber = m_stageNumber->GetSelectIndex();
}

void StageArrow::Update()
{
	float t = 0.0f;
	static const float ratio = 0.2f;

	m_ease += 0.0164f;

	if (m_ease >= 1.0f)
	{
		m_ease = 0.0f;
	}
	
	if (m_ease < ratio)
	{
		t = m_ease / ratio;
	}
	else
	{
		t = 1.0f - (m_ease - ratio) / (1.0f - ratio);
	}

	m_ArrowLeft->SetOffsetPos(Easing::InSine(t,1.0f,-2.7f,-2.6f),-1.1f);
	m_ArrowRight->SetOffsetPos(Easing::InSine(t, 1.0f, 2.7f, 2.6f), -1.1f);

	// 移動中に矢印を消す
	if (m_lastSelectNumber != m_stageNumber->GetSelectIndex())
	{
		m_lastSelectNumber = m_stageNumber->GetSelectIndex();
		m_hideTimer = g_hideTime + g_hideTime;
	}

	// 透明度変更
	m_hideTimer -= Time::GetDeltaTime();
	if (m_hideTimer > g_hideChangeTime)
	{
		m_ArrowLeft->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
		m_ArrowRight->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	else
	{
		if (m_hideTimer < 0.0f)
			m_hideTimer = 0.0f;

		float alpha = 1.0f - m_hideTimer / g_hideChangeTime;
		m_ArrowLeft->SetColor(1.0f, 1.0f, 1.0f, alpha);
		m_ArrowRight->SetColor(1.0f, 1.0f, 1.0f, alpha);
	}

	//ステージ1を選択しているとき矢印左を消す
	if (m_stageNumber->GetSelectIndex() == 1)
	{
		m_ArrowLeft->SetEnabled(false);
	}
	else
	{
		m_ArrowLeft->SetEnabled(true);
	}
	//ステージ30を選択しているとき矢印右を消す
	if (m_stageNumber->GetSelectIndex() >= 30)
	{
		m_ArrowRight->SetEnabled(false);
	}
	else if (m_stageNumber->GetSelectIndex() >= SaveData::GetClearLevel() + 1)
	{
		m_ArrowRight->SetEnabled(false);
	}
	else
	{
		m_ArrowRight->SetEnabled(true);
	}
}
