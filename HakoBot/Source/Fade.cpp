#include "Fade.h"
#include "SoundManager.h"
#include "Easing.h"

static const float g_weight = 0.0f;
static const float g_defaultFadeSpeed = 1.2f;
static const int g_skipFrameCount = 2;

Fade* Fade::s_main = nullptr;
Fade::FadeMode Fade::s_fadeMode = Fade::FadeMode::SIMPLE_IRIS;
bool Fade::s_isFade = false;
bool Fade::s_isActive = false;
float Fade::s_fadeRatio = 0.0f;
int Fade::s_frameCount = 0;
Fade::IconType Fade::s_maskIcon = Fade::IconType::SIMPLE;

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

	if (s_fadeMode == FadeMode::ICON_IRIS)
		s_main->SetMaskIcon(s_maskIcon);
}

void Fade::Update()
{
	if (s_main != this)
		return;

	// フェードイン最初の2フレームは処理スキップ (DeltaTimeが読み込みで大きくなる)
	if (s_frameCount < g_skipFrameCount && !s_isFade)
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

			if (s_frameCount == 0) {
				SoundManager::PlaySE("FadeOut", 1.0f, false);
				++s_frameCount;
			}
		}
	}

	// フェードモード切り替え
	switch (s_fadeMode)
	{
	case Fade::SIMPLE_IRIS:
		m_renderer->GetMaterial()->SetPixelShader("Assets/Shader/IrisOut_PS.cso");
		m_renderer->LoadTexture("Assets/Textures/Default.png", false);
		break;

	case Fade::ICON_IRIS:
		m_renderer->GetMaterial()->SetPixelShader("Assets/Shader/IrisOut_PS.cso");
		break;
	}

	// 色リセット
	m_renderer->SetColor(0.1f, 0.1f, 0.1f, 1.0f);

	// レンダラー更新
	if (s_fadeRatio <= 0.0f)
	{
		m_renderer->SetEnabled(false);
	}
	else
	{
		m_renderer->SetEnabled(true);
	}
	if (s_fadeMode == FadeMode::ICON_IRIS)
	{
		float pointTime = 0.3f;		// 最もフェードが遅くなるタイミング
		float pointRatio = 0.97f;	// 最もフェードが遅くなっているときの割合
		float ratio;

		if (s_fadeRatio > pointTime)
		{
			ratio = Easing::InCirc(s_fadeRatio - pointTime, 1.0f - pointTime, 1.0f, pointRatio);
		}
		else
		{
			ratio = Easing::OutCirc(s_fadeRatio, pointTime, pointRatio, 0.0f);
		}
		m_renderer->SetCustomData(&ratio, sizeof(ratio));

		float alphaMaxRatio = 0.25f;		// 半透明になる最大フェード割合
		float alphaMinRatio = 0.15f;		// 半透明になる最小フェード割合
		if (s_fadeRatio < alphaMaxRatio)
		{
			float alphaRatioLength = alphaMaxRatio - alphaMinRatio;
			float alpha = (s_fadeRatio - alphaMinRatio) / alphaRatioLength;
			alpha = std::min(alpha, 1.0f);
			m_renderer->SetColor(0.1f, 0.1f, 0.1f, alpha);
		}
	}
	else
	{
		m_renderer->SetCustomData(&s_fadeRatio, sizeof(s_fadeRatio));
	}
}

void Fade::SetFadeSpeed(float fadeSpeed)
{
	m_fadeSpeed = fadeSpeed;
}

void Fade::SetMaskIcon(IconType iconType)
{
	float size = 1280.0f * 25.0f;
	switch (iconType)
	{
	case Fade::IconType::SIMPLE:
		m_renderer->LoadTexture("Assets/Textures/Fade/mask_simple.png");
		break;
	case Fade::IconType::SMILE:
		m_renderer->LoadTexture("Assets/Textures/Fade/mask_smile.png");
		break;
	case Fade::IconType::ANGRY:
		m_renderer->LoadTexture("Assets/Textures/Fade/mask_angry.png");
		break;
	case Fade::IconType::KIRAKIRA:
		m_renderer->LoadTexture("Assets/Textures/Fade/mask_kirakira.png");
		break;
	case Fade::IconType::RELAX:
		m_renderer->LoadTexture("Assets/Textures/Fade/mask_relax.png");
		break;
	}

	m_renderer->GetMaterial()->SetSamplerState(SamplerState::LINEAR_CLAMP);
	m_renderer->SetSize(size);
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
	s_fadeMode = FadeMode::SIMPLE_IRIS;
	StartFadeOut();
}

void Fade::StartIrisIn()
{
	s_fadeMode = FadeMode::SIMPLE_IRIS;
	StartFadeIn();
}

void Fade::StartIconIrisOut(bool maskIconChange)
{
	s_fadeMode = FadeMode::ICON_IRIS;
	if (maskIconChange)
	{
		s_maskIcon = (IconType)(rand() % (int)IconType::COUNT);
	}
	if (s_main)
	{
		s_main->SetMaskIcon(s_maskIcon);
	}
	StartFadeOut();
}

void Fade::StartIconIrisIn(bool maskIconChange)
{
	s_fadeMode = FadeMode::ICON_IRIS;
	if (maskIconChange)
	{
		s_maskIcon = (IconType)(rand() % (int)IconType::COUNT);
	}
	if (s_main)
	{
		s_main->SetMaskIcon(s_maskIcon);
	}
	StartFadeIn();
}

void Fade::StartFadeOut()
{
	s_fadeRatio = -g_weight;
	s_frameCount = 0;
	s_isActive = true;
	s_isFade = true;
	s_frameCount = 0;
}

void Fade::StartFadeIn()
{
	s_fadeRatio = 1.0f + g_weight;
	s_frameCount = 0;
	s_isActive = true;
	s_isFade = false;
	s_frameCount = 0;
}
