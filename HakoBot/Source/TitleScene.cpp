#include "TitleScene.h"
#include "TitleExit.h"
// サウンド
#include "SoundMaster.h"
#include "SoundManager.h"
#include "GameState.h"
#include "TitleUI.h"
#include "InputManager.h"
#include "PlayerModelDraw.h"
#include "TitlePlayerMove.h"

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
        Camera::Config offsetConfig = camera->GetConfig();
        offsetConfig.fovAngle = 10.0f;
        camera->SetConfig(offsetConfig);
        auto transform  = obj->GetTransform();
        transform->SetPosition(10.0f, 6.0f, -10.5f);
        transform->SetEulerAngle(13.0f,-25.2f,0.0f);
    }

    // タイトルUI
    {
        auto obj = CreateGameObject();
        obj->AddComponent<TitleExit>();
        //obj->AddComponent<TitleUI>();
    }

    {
        float scale = 0.5f;
        auto obj = CreateGameObject();
        auto player = obj->AddComponent<PlayerModelDraw>();
        auto transform = obj->GetTransform();
        transform->SetScale(scale, scale, scale);
        transform->SetPosition(0.0f, 0.0f, 7.0f);
        transform->SetEulerAngle(Vector3(0.0f, -90.0f, 0.0f));

        //移動
        auto playerMove = obj->AddComponent<TitlePlayerMove>();
        
        playerMove->SetPlayer(obj->GetTransform());
    }
    
    {//床
        int stageSize_x = 25;
        int stageSize_z = 25;
        float blockScale = 1.0f;
        for (int z = 0; z < stageSize_z; ++z) {
            for (int x = 0; x < stageSize_x; ++x) {
                Vector3 pos(x - stageSize_x * 0.5f + 0.5f , 0.0f, z - stageSize_z * 0.5f + 0.5f);
                pos *= blockScale;

                auto obj = CreateGameObject();
                auto renderer = obj->AddComponent<MeshRenderer>();
                renderer->LoadModel("Assets/Model/Stage/fbx/yuka.fbx");
                auto transform = obj->GetTransform();
                transform->SetPosition(pos);
                transform->SetScale(0.25f, 0.25f, 0.25f);
            }
        }
    }


}
