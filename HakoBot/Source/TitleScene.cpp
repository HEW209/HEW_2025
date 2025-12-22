#include "TitleScene.h"
#include "TitleExit.h"
// サウンド
#include "SoundMaster.h"
#include "SoundManager.h"
#include "GameState.h"
#include "TitleUI.h"
#include "InputManager.h"
#include "PlayerModelDraw.h"
void TitleScene::Init()
{
    InputManager::ChangeBindType(InputBindType::UI);
    SoundManager::StopAll();
    SoundManager::Load();
    RenderSystem::Instance().SetClearColor(Color(1.0f, 0.7f, 0.2f, 1.0f));
    
    // カメラ
    {
        
        auto obj = CreateGameObject();
        auto camera = obj->AddComponent<Camera>();
        obj->GetTransform()->SetPosition(0.0f, 1.0f, -5.0f);

    }

    // タイトルUI
    {
        auto obj = CreateGameObject();
        obj->AddComponent<TitleExit>();
        obj->AddComponent<TitleUI>();
    }

    {
        float scale = 0.8;
        auto obj = CreateGameObject();
        auto player = obj->AddComponent<PlayerModelDraw>();
        auto transform = obj->GetTransform();
        transform->SetScale(scale, scale, scale);
        transform->SetPosition(0.5f, 0.0f, -4.0f);
        transform->SetEulerAngle(Vector3(0.0f, 45.0f, 0.0f));
    }
    
}
