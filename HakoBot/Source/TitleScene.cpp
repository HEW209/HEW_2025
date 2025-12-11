#include "TitleScene.h"
#include "TitleExit.h"
// サウンド
#include "SoundMaster.h"
#include "SoundManager.h"
#include "GameState.h"
#include "TitleUI.h"
#include "InputManager.h"

void TitleScene::Init()
{
    InputManager::ChangeBindType(InputBindType::UI);
    SoundManager::StopAll();
    SoundManager::Load();
    RenderSystem::Instance().SetClearColor(Color(1.0f, 0.7f, 0.2f, 1.0f));
    
    // カメラ
    {
        auto obj = CreateGameObject();
        obj->AddComponent<Camera>();
    }
    // タイトル
    {
        auto obj = CreateGameObject();
        auto renderer = obj->AddComponent<SpriteRenderer>();
        renderer->SetUI(true);
        renderer->LoadTexture("Assets/Textures/title.png");
        renderer->SetSize(800.0f);
        renderer->SetOffsetPos(0.0f, 1.0f);
    }

    // はじめる
    {
        auto obj = CreateGameObject();
        obj->AddComponent<TitleExit>();
        obj->AddComponent<TitleUI>();
    }

    //BGM再生
    SoundManager::PlayBGM("Title", 1.0f, true);
}
