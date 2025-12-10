#include "TitleScene.h"
#include "TitleExit.h"

void TitleScene::Init()
{
    // ƒ^ƒCƒgƒ‹
    {
        auto obj = CreateGameObject();
        auto renderer = obj->AddComponent<SpriteRenderer>();
        renderer->SetUI(true);
        renderer->LoadTexture("Assets/Textures/title.png");
        renderer->SetSize(150.0f, 150.0f);
        renderer->SetOffsetPos(0.0f, 0.1f);
    }

    // ‚Í‚¶‚ß‚é
    {
        auto obj = CreateGameObject();
        auto renderer = obj->AddComponent<SpriteRenderer>();
        renderer->SetUI(true);
        renderer->LoadTexture("Assets/Textures/hajimeru.png");
        renderer->SetSize(70.0f);
        renderer->SetOffsetPos(0.0f, -0.6f);
        obj->AddComponent<TitleExit>();
    }
}
