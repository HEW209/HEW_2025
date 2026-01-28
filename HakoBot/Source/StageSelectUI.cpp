#include "StageSelectUI.h"
#include "Easing.h"
#include "StageNumber.h"
#include "SaveData.h"

//StageSelectUI.cpp

static const float g_hideTime = 0.5f;
static const float g_hideChangeTime = 0.3f;
static const float g_textSize = 280.0f;

StageSelectUI::StageSelectUI():
	m_ease(0.0f),
	m_hideTimer(g_hideChangeTime),
	m_animeTimer(0.0f)
{
}

void StageSelectUI::Start()
{
	//矢印左
	auto arrowLeft = GetGameObject()->AddComponent<SpriteRenderer>();
	arrowLeft->LoadTexture("Assets/Textures/StageSelect/Arrow.png");
	arrowLeft->SetOffsetPos(-2.5f,-1.1f);
	arrowLeft->SetSize(160.0f);
	arrowLeft->SetUI(true);
	m_arrowLeft = arrowLeft;

	//矢印右
	auto arrowRight = GetGameObject()->AddComponent<SpriteRenderer>();
	arrowRight->LoadTexture("Assets/Textures/StageSelect/Arrow.png");
	arrowRight->SetOffsetPos(2.5f, -1.1f);
	arrowRight->SetSize(160.0f);
	arrowRight->SetUVOffsetPos(1.0f, 1.0f);
	arrowRight->SetUVScale(-0.9f, -1.0f);
	arrowRight->SetUI(true);
	m_arrowRight = arrowRight;

	//テキスト
	auto text = GetGameObject()->AddComponent<SpriteRenderer>();
	text->LoadTexture("Assets/Textures/Texts/hajimeru.png");
	text->SetOffsetPos(0.0f, -2.8f);
	text->SetSize(g_textSize);
	text->SetUI(true);
	m_startText = text;

	m_lastSelectNumber = m_stageNumber->GetSelectIndex();
}

void StageSelectUI::Update()
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

	m_arrowLeft->SetOffsetPos(Easing::InSine(t,1.0f,-2.7f,-2.6f),-1.1f);
	m_arrowRight->SetOffsetPos(Easing::InSine(t, 1.0f, 2.7f, 2.6f), -1.1f);

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
		m_arrowLeft->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
		m_arrowRight->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
		m_startText->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	else
	{
		if (m_hideTimer < 0.0f)
			m_hideTimer = 0.0f;

		float alpha = 1.0f - m_hideTimer / g_hideChangeTime;
		m_arrowLeft->SetColor(1.0f, 1.0f, 1.0f, alpha);
		m_arrowRight->SetColor(1.0f, 1.0f, 1.0f, alpha);
		m_startText->SetColor(1.0f, 1.0f, 1.0f, alpha);
	}

	// スケーリングアニメーション
	m_animeTimer += Time::GetDeltaTime();
	float scale = std::fabsf(std::sinf(m_animeTimer * Math::TAU / 3.0f));
	scale = scale * 0.1f + 1.0f;
	m_startText->SetSize(g_textSize * scale);

	//ステージ1を選択しているとき矢印左を消す
	if (m_stageNumber->GetSelectIndex() == 1)
	{
		m_arrowLeft->SetEnabled(false);
	}
	else
	{
		m_arrowLeft->SetEnabled(true);
	}
	//ステージ30を選択しているとき矢印右を消す
	if (m_stageNumber->GetSelectIndex() >= 30)
	{
		m_arrowRight->SetEnabled(false);
	}
	else if (m_stageNumber->GetSelectIndex() >= SaveData::GetClearLevel() + 1)
	{
		m_arrowRight->SetEnabled(false);
	}
	else
	{
		m_arrowRight->SetEnabled(true);
	}
}
