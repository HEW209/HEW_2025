#include "Fade.h"
#include "SoundManager.h"

static const float g_weight = 0.0f;
static const float g_defaultFadeSpeed = 1.2f;
static const int g_skipFrameCount = 2;

Fade* Fade::s_main = nullptr;
Fade::FadeMode Fade::s_fadeMode = Fade::FadeMode::IRIS;
bool Fade::s_isFade = false;
bool Fade::s_isActive = false;
float Fade::s_fadeRatio = 0.0f;
int Fade::s_frameCount = 0;

Fade::Fade() :
	m_renderer(nullptr),
	m_fadeSpeed(g_defaultFadeSpeed)
{
	if (s_main == nullptr)
		s_main = this;
}

Fade::~Fade()
{
	if (s_main == this)
		s_main = nullptr;
}

void Fade::Awake()
{
	m_renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	m_renderer->SetSize(1280, 1280);
	m_renderer->SetOrder(100);
	m_renderer->SetColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_renderer->SetUI(true);
}

void Fade::Update()
{
	if (s_main != this)
		return;

	// 最初の2フレームは処理スキップ (DeltaTimeが読み込みで大きくなる)
	if (s_frameCount < g_skipFrameCount)
	{
		++s_frameCount;
	}
	else
	{
		// フェード割合更新
		if (s_fadeRatio > 0.0f && !s_isFade)
		{
			s_fadeRatio -= m_fadeSpeed * Time::GetDeltaTime();
			if (s_fadeRatio < 0.0f)
			{
				s_fadeRatio = 0.0f;
				s_isActive = false;
			}

			if (s_frameCount == g_skipFrameCount) {
				SoundManager::PlaySE("FadeIn", 1.0f, false);
				++s_frameCount;
			}
		}
		if (s_fadeRatio < 1.0f && s_isFade)
		{
			s_fadeRatio += m_fadeSpeed * Time::GetDeltaTime();
			if (s_fadeRatio > 1.0f)
			{
				s_fadeRatio = 1.0f;
				s_isActive = false;
			}

			if (s_frameCount == g_skipFrameCount) {
				SoundManager::PlaySE("FadeOut", 1.0f, false);
				++s_frameCount;
			}
		}
	}

	// フェードモード切り替え
	switch (s_fadeMode)
	{
	case Fade::IRIS:
		//m_renderer->LoadTexture("Assets/Textures/Mask.png");
		m_renderer->GetMaterial()->SetPixelShader("Assets/Shader/IrisOut_PS.cso");
		break;
	}

	// レンダラー更新
	if (s_fadeRatio <= 0.0f)
	{
		m_renderer->SetEnabled(false);
	}
	else
	{
		m_renderer->SetEnabled(true);
	}
	m_renderer->SetCustomData(&s_fadeRatio, sizeof(s_fadeRatio));
}

void Fade::SetFadeSpeed(float fadeSpeed)
{
	m_fadeSpeed = fadeSpeed;
}

bool Fade::IsActive()
{
	return s_isActive;
}

void Fade::SetFadeRatio(float ratio)
{
	s_fadeRatio = ratio;
	s_main->m_renderer->SetCustomData(&s_fadeRatio, sizeof(s_fadeRatio));
}

void Fade::StartIrisOut()
{
	s_fadeMode = FadeMode::IRIS;
	s_fadeRatio = -g_weight;
	s_isActive = true;
	s_isFade = true;
	s_frameCount = 0;
}

void Fade::StartIrisIn()
{
	s_fadeMode = FadeMode::IRIS;
	s_fadeRatio = 1.0f + g_weight;
	s_isActive = true;
	s_isFade = false;
	s_frameCount = 0;
}
