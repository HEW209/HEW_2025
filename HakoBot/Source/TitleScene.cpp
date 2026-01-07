#include "TitleScene.h"
#include "TitleExit.h"
// サウンド
#include "SoundMaster.h"
#include "SoundManager.h"
#include "TitleLogo.h"
#include "InputManager.h"
#include "TitlePlayer.h"

void TitleScene::Init()
{
    InputManager::ChangeBindType(InputBindType::UI);
    SoundManager::StopAll();
    SoundManager::Load();
    RenderSystem::Instance().SetClearColor(Color(1.0f, 0.7f, 0.2f, 1.0f));
    
    // カメラ
    {
        auto cameraRoot = CreateGameObject();
        auto obj = CreateGameObject();

        auto camera = obj->AddComponent<Camera>();
        Camera::Config cameraConfig;
        cameraConfig.fovAngle = 10.0f;
        camera->SetConfig(cameraConfig);

        obj->GetTransform()->SetParent(cameraRoot);
        obj->GetTransform()->SetPosition(0.0f, 3.0f, -40.0f, Space::LOCAL);
        cameraRoot->GetTransform()->SetEulerAngle(10.0f, -30.0f, 0.0f);
    }

    {
        // ライト
        auto lightObj = CreateGameObject();
        auto light = lightObj->AddComponent<DirectionalLight>();
        light->SetLightSize(0.1f);
        lightObj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
    }

    // 地形
    {
        // 床
        Vector3 posOffset(-6.0f, 0.0f, 3.0f);
        int stageSize_x = 30;
        int stageSize_z = 16;
        float blockScale = 1.0f;
        for (int z = 0; z < stageSize_z; ++z) {
            for (int x = 0; x < stageSize_x; ++x) {
                Vector3 pos(x - stageSize_x * 0.5f + 0.5f, 0.0f, z - stageSize_z * 0.5f + 0.5f);
                pos *= blockScale;

                auto obj = CreateGameObject();
                auto renderer = obj->AddComponent<MeshRenderer>();
                renderer->LoadModel("Assets/Model/Stage/fbx/yuka.fbx");
                obj->GetTransform()->SetPosition(pos + posOffset);
                obj->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
            }
        }

        // 柵
        posOffset = Vector3(-6.0f, 0.0f, 10.5f);
        for (int x = 0; x < stageSize_x; ++x) {
            if (x % 2 == 0)
                continue;

            Vector3 pos(x - stageSize_x * 0.5f + 0.5f, 0.0f, 0.0f);
            pos *= blockScale;

            auto obj = CreateGameObject();
            auto renderer = obj->AddComponent<MeshRenderer>();
            renderer->LoadModel("Assets/Model/Stage/fbx/saku.fbx");
            renderer->SetShouldDrawShadow(true);
            obj->GetTransform()->SetPosition(pos + posOffset);
            obj->GetTransform()->SetEulerAngle(0.0f, 90.0f, 0.0f);
        }
    }

    // プレイヤー
    {
        auto obj = CreateGameObject();
        obj->AddComponent<TitlePlayer>();
        
    }

    // タイトルロゴ
    {
        auto obj = CreateGameObject();
        obj->GetTransform()->SetPosition(0.0f, 1.2f, 0.0f);
        obj->AddComponent<TitleLogo>();
    }

    //タイトルUI
    {
        auto obj = CreateGameObject();
        obj->AddComponent<TitleExit>();
        obj->GetTransform()->SetPosition(0.0f, -2.0f, 0.0f);
    }
}
