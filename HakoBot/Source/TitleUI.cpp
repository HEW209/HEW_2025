//Title.cpp
#include "TitleUI.h"
#include "Easing.h"
#include "SoundManager.h"

void TitleUI::Start()
{
    SoundManager::Load();

    //タイトルロゴ
    m_pTitle = GetGameObject()->AddComponent<SpriteRenderer>();
    m_pTitle->SetUI(true);
    m_pTitle->LoadTexture("Assets/Textures/title.png");
    m_pTitle->SetSize(800.0f);
    m_pTitle->SetOffsetPos(0.0f, 5.0f);
    m_defaultTitlePos = m_pTitle->GetOffsetPos();

    //はじめる文字
	m_pRend = GetGameObject()->AddComponent<SpriteRenderer>();
    m_pRend->SetUI(true);
    m_pRend->LoadTexture("Assets/Textures/hajimeru.png");
    m_pRend->SetSize(400.0f);
    m_pRend->SetOffsetPos(0.0f, -10.0f);
    m_rendDefaultScale = m_pRend->GetSize();
    m_rendColor = m_pRend->GetColor();

    m_bPlayBGM = false;
    m_bTitleAnimComplete = false;
    m_bTitleFall = false;
    m_bTitleLanding = false;

    SoundManager::PlaySE("TitleFall", 1.0f, false);
}

void TitleUI::Update()
{
    //イージング用
    m_valueColor += EASING;
    m_valueTitle += EASING;
    if (m_valueTitle > 12.0f)
    {
        m_bTitleFall = true;
        m_valueTitle = 12.0f;
    }

    //着地音ならす
    if (m_bTitleFall && !m_bTitleLanding)
    {
        //一回だけ実行
        SoundManager::PlaySE("TitleLanding", 1.0f, false);
        m_bTitleLanding = true;
        //アニメーションが終わったらtrue今は仮でここ本来は「ハコボット！」みたいな音鳴らしたい
        m_bTitleAnimComplete = true;
        // SoundManager::PlaySE("TitleHakoBot", 1.0f, false);音が鳴り終わり次第ハコボット音ならしたいよねー
    }

    //ロゴ落下
    Vector2 offset = Vector2::zero;
    offset.y = Easing::InSine(m_valueTitle, 12.0f, -4.0f, 0.0f);
    m_pTitle->SetOffsetPos(m_defaultTitlePos + offset);

    

    //ロゴアニメーションが終わったら
    if (m_bTitleAnimComplete)
    {
        //「はじめる」点滅
        m_rendColor.a = Easing::InSine(m_valueColor, 7.0f, 1.0f, 0.0f);
        m_pRend->SetColor(m_rendColor);
        //「はじめる」表示
        m_pRend->SetOffsetPos(0.0f, -2.0f);

        //BGM再生
        if (!m_bPlayBGM)
        {
            //一回だけ実行
            m_bPlayBGM = true;
            SoundManager::PlayBGM("Title", 1.0f, true);
        }
    }
}
