#include "GameUI.h"

#include "GuideUIController2.h"
#include "GuideUITimeController.h"
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

    // タイマー1
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        obj->GetTransform()->SetParent(m_pRoot->GetTransform());
        obj->GetTransform()->SetPosition(4.1f, 3.2f, 0.0f);
        obj->AddComponent<GuideUITimeController>();
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
    m_pRoot->SetActive(!tutorial || tutorial->IsFinished());
}
