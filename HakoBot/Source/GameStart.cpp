#include "GameStart.h"
#define DRAW_TIME 120.0f
StartUI::StartUI()
{
    m_Timer = 0.0f;
    m_HalfProcessed = false;
    m_IsVisible = true;
    m_Alpha = 0.0f;

    // サイズ
    m_StartSize={ 400.0f, 200.0f };  // フェード開始時サイズ
    m_EndSize={ 300.0f, 150.0f };    // 最終表示サイズ
    m_CurrentSize = m_StartSize;

}

void StartUI::Start()
{
    m_StartSprite = GetGameObject()->AddComponent<SpriteRenderer>();
    m_StartSprite->SetUI(true);
    m_StartSprite->LoadTexture("Assets/Textures/Texts/Start.png");
    m_StartSprite->SetOffsetPos(0.0f, 0.0f);
    

    
    m_StartSprite->SetSize(m_StartSize.x, m_StartSize.y);
}

void StartUI::Update()
{

    if (!m_IsVisible)
        return;

        m_Timer +=1.0f;

        float halfTime = DRAW_TIME * 0.5f;

        // フェードイン処理
        if (m_Timer >= halfTime && m_Timer < DRAW_TIME)
        {
            // 0.0f ～ 1.0f の進行率
            float t = (m_Timer - halfTime) / halfTime;
            t = std::clamp(t, 0.0f, 1.0f);

            // Alpha補間
            m_Alpha = Lerp(0.0f, 1.0f, t);
            
            // Size補間
            m_CurrentSize.x = Lerp(m_StartSize.x, m_EndSize.x, t);
            m_CurrentSize.y = Lerp(m_StartSize.y, m_EndSize.y, t);
        }

        // 終了処理
        if (m_Timer >= DRAW_TIME)
        {
			// 非表示にする
			m_StartSprite->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
			m_IsVisible = false;
        }

		// スプライト更新
		m_StartSprite->SetColor(1.0f, 1.0f, 1.0f, m_Alpha);
		m_StartSprite->SetSize(m_CurrentSize.x, m_CurrentSize.y);
}

float  StartUI::Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}