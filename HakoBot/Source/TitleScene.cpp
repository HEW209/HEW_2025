#include "TitleScene.h"
#include "TitleExit.h"
#include "SoundManager.h"

void TitleScene::Init()
{
    SoundManager::StopAll();
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
        auto renderer = obj->AddComponent<SpriteRenderer>();
        renderer->SetUI(true);
        renderer->LoadTexture("Assets/Textures/hajimeru.png");
        renderer->SetSize(400.0f);
        renderer->SetOffsetPos(0.0f, -0.0f);
        obj->AddComponent<TitleExit>();
        obj->GetTransform()->Translate(0.0f, -2.0f, 0.0f);
    }
}
