#include "GameUI.h"

#include "GuideUIController2.h"
#include "TimeUI.h"
#include "GuideUIMenuController.h"
#include "InputUI.h"
#include "GameStart.h"
#include "GameState.h"

void GameUI::Awake()
{
    // ルートUIオブジェクト
    {
        m_pRoot = SceneManager::GetActiveScene()->CreateGameObject();
        m_pRoot->GetTransform()->SetParent(GetTransform());
		m_pRoot->SetActive(false);
	}

    // 入力ガイドUI
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        auto renderer = obj->AddComponent<InputUI>();
    }

    // 完成 
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        obj->GetTransform()->SetPosition(7.8f, -4.6f, 0.0f);
        obj->AddComponent<GuideUIController2>();
    }

    // メニュー開いたやつ
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        obj->AddComponent<GuideUIMeneController>();
    }

    // タイマー背景
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        auto back = obj->AddComponent<SpriteRenderer>();
        back->LoadTexture("Assets/Textures/GameUI/time_frame.png");
        back->SetSize(350.0f);
        Vector2 backOffset = back->GetSize() * -0.5f * 0.01f;
        back->SetOffsetPos(backOffset + Vector2(6.4f, 3.6f));
        back->SetUI(true);
        back->GetMaterial()->SetSamplerState(SamplerState::POINT_CLAMP);
    }

    // タイマー1
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        obj->GetTransform()->SetPosition(4.25f, 3.25f, 0.0f);
        obj->AddComponent<TimeUI>();
    }

    // 開始演出
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        obj->AddComponent<StartUI>();
    }
}

void GameUI::Update()
{
	ITutorial* tutorial = GameState::GetInstance()->GetTutorial();
    if (!tutorial || tutorial->IsFinished())
    {
        if (!m_pRoot->IsActiveSelf())
            GameState::GetInstance()->StartTimer();

        m_pRoot->SetActive(true);
    }
    else
    {
        m_pRoot->SetActive(false);
    }
}
