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

    // タイトルUI
    {
        auto obj = CreateGameObject();
        obj->AddComponent<TitleExit>();
        obj->AddComponent<TitleUI>();
    }

    
}
