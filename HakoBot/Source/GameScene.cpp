#include "GameScene.h"
#include "LevelSerializer.h"
#include "InputManager.h"
#include "GameState.h"
#include "Player.h"
#include "PlayerCamera.h"
// サウンド
#include "SoundMaster.h"
#include "SoundManager.h"

#include "BlockObject.h"
#include "GridField.h"
#include "ClearProduce.h"
#include "ResultController.h"

#include "GameUI.h"
#include "Fade.h"
#include "StageSet.h"

#include "ColliderDebug.h"
#include <cmath>

GameScene::GameScene(const std::string& levelName)
    : m_levelName(levelName) {
}

void GameScene::Init() {
    SoundManager::StopAll();
    SoundManager::Load();
    RenderSystem::Instance().SetClearColor(Color(1.0f, 0.7f, 0.2f, 1.0f));
    InputManager::ChangeBindType(InputBindType::GAMEPLAY);

    std::string path = "Assets/Level/Stages/" + m_levelName + ".json";
    if (!LevelSerializer::LoadLevelData(path, m_levelData)) {
        Debug::ErrorMessage("Failed to load level: " + m_levelName);
    }

    KeyBind();

    // ゲームステート
    {
        auto obj = CreateGameObject();
        auto gameState = obj->AddComponent<GameState>();
        gameState->SetLevelName(m_levelName);
    }

    //移動できるオブジェクト（プレイヤー）を作成	
    //移動できるプレイヤーオブジェクトを作る
    auto player = CreateGameObject();
    player->GetTransform()->SetPosition(0.0f, 0.0f, -11.0f);
    player->GetTransform()->SetEulerAngle(0.0f, 180.0f, 0.0f);
    auto playerComp = player->AddComponent<Player>();
    GameState::GetInstance()->SetPlayer(playerComp);

    auto cameraObj = CreateGameObject();
    auto playerCamera = cameraObj->AddComponent<PlayerCamera>();
    playerCamera->SetPlayer(player->GetTransform());
    playerCamera->SetCameraDistance(5.0f + m_levelData.gridSize.y * 3.0f);



    CreateStageSet();
    CreateGridField();

    const Vector3 defaultPos(-10.0f, 0.0f, -3.0f);
    const int maxCountZ = 5;
    const float intervalZ = -4.0f;
    const float intervalX = -4.0f;

    // プレイヤーが動かすブロックを置く
    for (size_t i = 0; i < m_levelData.inventoryBlockFiles.size(); ++i) {
        std::string blockName = m_levelData.inventoryBlockFiles[i];
        std::string blockPath = "Assets/Level/Blocks/" + blockName + ".json";


        BlockTemplateData blockData;
        if (LevelSerializer::LoadBlockTemplate(blockPath, blockData)) {

            BlockSetData blockSet;
            for (const auto& pos : blockData.blocks) {
                blockSet.blocks.push_back({ pos.x, pos.y, pos.z });
            }

            auto obj = CreateGameObject();

            auto blockComp = obj->AddComponent<BlockObject>();
            blockComp->SetBlockSet(blockSet);

            if (!blockData.modelPath.empty()) {
                blockComp->SetModel(blockData.modelPath);
            }

            float index_z = i % maxCountZ;
            float index_x = i / maxCountZ;
            int count_z;
            if (i < (m_levelData.inventoryBlockFiles.size() / maxCountZ) * maxCountZ)
            {
                count_z = maxCountZ;
            }
            else
            {
                count_z = m_levelData.inventoryBlockFiles.size() % maxCountZ;
            }

            index_z -= (count_z - 1) * 0.5f;

            Vector3 pos = defaultPos;
            pos.x += index_x * intervalX;
            pos.y = 0.0f;
            pos.z += index_z * intervalZ;

            obj->GetTransform()->SetPosition(pos + blockComp->GetGroundOffset());

            GameState::GetInstance()->AppendWorldBlock(blockComp);
        }
    }

    // ライト
    auto lightObj = CreateGameObject();
    auto light = lightObj->AddComponent<DirectionalLight>();
    light->SetLightSize(0.1f);
    lightObj->GetTransform()->SetEulerAngle(50.0f, -15.0f, 0.0f);

    {
        auto obj = CreateGameObject();
        auto fade = obj->AddComponent<Fade>();
        Fade::StartIrisIn();
    }

    // クリア演出
    auto clearObj = CreateGameObject();
    clearObj->AddComponent<ClearProduce>();

    // BGM再生
    SoundManager::PlayBGM("Game", 0.6f, true);

    // 使用していないリソース解放
    TextureManager::Instance().CollectGarbage();
    ModelManager::Instance().CollectGarbage();

	int stageNo = GameState::GetInstance()->GetCurrentStegaNo();
    if (stageNo == 1) {
        auto obj = CreateGameObject();
        auto tutorial = obj->AddComponent<Tutorial1>();
        GameState::GetInstance()->SetTutorial(tutorial);
    }

    // UIオブジェクト
    CreateUIObject();

    // 当たり判定表示機能
#ifdef _DEBUG
    auto obj = CreateGameObject();
    obj->AddComponent<ColliderDebug>();
#endif
}

void GameScene::KeyBind() {
    InputManager::ChangeBindType(InputBindType::GAMEPLAY);
}

void GameScene::CreateGridField() {
    int size_x = m_levelData.gridSize.x;
    int size_y = m_levelData.gridSize.y;
    int size_z = m_levelData.gridSize.z;

    {
        auto obj = CreateGameObject();
        auto component = obj->AddComponent<GridField>();
        GameState::GetInstance()->SetGridField(component);

        component->SetSize({ size_x, size_y, size_z });

        component->SetClearShape(
            m_levelData.targetShapes[0],
            m_levelData.targetShapes[1],
            m_levelData.targetShapes[2]
        );

        obj->GetTransform()->SetPosition(0.0f, 0.2f, 0.0f);
    }

    // 投影機
    for (int i = 0; i < 4; ++i) {
        auto obj = CreateGameObject();
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/toueiki.fbx");

        Transform* transform = obj->GetTransform();
        switch (i)
        {
        case 0:
            transform->SetPosition(-(3.0f + size_x * 0.5f), 0.0f, 0.0f);
            transform->SetEulerAngle(0.0f, 90.0f, 0.0f);
            break;

        case 1:
            transform->SetPosition(0.0f, 0.0f, 3.0f + size_z * 0.5f);
            transform->SetEulerAngle(0.0f, 0.0f, 0.0f);
            break;

        case 2:
            transform->SetPosition((3.0f + size_x * 0.5f), 0.0f, 0.0f);
            transform->SetEulerAngle(0.0f, -90.0f, 0.0f);
            break;

        case 3:
            transform->SetPosition(0.0f, 0.0f, -(3.0f + size_z * 0.5f));
            transform->SetEulerAngle(0.0f, 180.0f, 0.0f);
            break;
        }

        auto effectObj = CreateGameObject();
        effectObj->GetTransform()->SetParent(obj);
        effectObj->GetTransform()->SetPosition(0.0f, -0.25f, 0.0f, Space::LOCAL);
        effectObj->GetTransform()->SetScale(1.6f, 1.6f, 1.6f);
        auto effect = effectObj->AddComponent<EffectRenderer>();
        effect->Load("Assets/Effect/Toueiki/toueiki_efe.efkefc");
        effect->Play();
    }

    // 危険エリア Z
    for (int i = 0; i < 2; ++i) {
        float flip = i == 0 ? 1.0f : -1.0f;
        for (int x = 0; x < size_x; ++x) {
            auto obj = CreateGameObject();
            auto renderer = obj->AddComponent<MeshRenderer>();
            renderer->LoadModel("Assets/Model/Stage/fbx/kiken.fbx");

            Vector3 pos;
            pos.x = x - size_x * 0.5f + 0.5f;
            pos.z = (size_z * 0.5f + 0.5f) * flip;
            obj->GetTransform()->SetPosition(pos);
            obj->GetTransform()->SetEulerAngle(0.0f, 90.0f * flip, 0.0f);
        }
    }

    // 危険エリア X
    for (int i = 0; i < 2; ++i) {
        float flip = i == 0 ? 1.0f : -1.0f;
        for (int z = 0; z < size_z; ++z) {
            auto obj = CreateGameObject();
            auto renderer = obj->AddComponent<MeshRenderer>();
            renderer->LoadModel("Assets/Model/Stage/fbx/kiken.fbx");

            Vector3 pos;
            pos.x = (size_x * 0.5f + 0.5f) * flip;
            pos.z = z - size_z * 0.5f + 0.5f;
            obj->GetTransform()->SetPosition(pos);
            obj->GetTransform()->SetEulerAngle(0.0f, 90.0f + 90.0f * flip, 0.0f);
        }
    }

    // 危険エリア コーナー
    for (int i = 0; i < 4; ++i) {
        Vector3 pos;
        pos.x = size_x * 0.5f + 0.5f;
        pos.z = size_z * 0.5f + 0.5f;
        if (i == 1 || i == 2) pos.x *= -1;
        if (i == 2 || i == 3) pos.z *= -1;

        auto obj = CreateGameObject();
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Kiken_Corner.fbx");
        obj->GetTransform()->SetPosition(pos);
        obj->GetTransform()->SetEulerAngle(0.0f, 90.0f - 90.0f * i, 0.0f);
    }

    // 当たり判定
    {
        // 床スクリーン
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(0.0f, 0.0f, 0.0f);
        transform->SetScale(size_x, 0.4f, size_z);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { float(size_x), 0.4f, float(size_z) };
    }

    float rad = atanf(0.2f);
    float sin = sinf(rad);
    float cos = cosf(rad);
    float y = 0.1f - 0.25f * cos;
    float xz = 0.5f - 0.25f * sin;
    float scale = sqrt(1.0f + 0.04f);
    float angle = Math::RadToDeg(rad);
    {
        // -z危険線
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(0.0f, y, -(size_z * 0.5f + xz));
        transform->SetScale(size_x, 0.5f, scale);
        transform->SetEulerAngle(-angle, 0.0f, 0.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { float(size_x), 0.5f, scale };
    }

    {
        // +z危険線
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(0.0f, y, size_z * 0.5f + xz);
        transform->SetScale(size_x, 0.5f, scale);
        transform->SetEulerAngle(angle, 0.0f, 0.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { float(size_x), 0.5f, scale };
    }
    {
        // -x危険線
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(-(size_z * 0.5f + xz), y, 0.0f);
        transform->SetScale(scale, 0.5f, size_z);
        transform->SetEulerAngle(0.0f, 0.0f, angle);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { scale , 0.5f, float(size_z) };
    }

    {
        // +x危険線
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(size_z * 0.5f + xz, y, 0.0f);
        transform->SetScale(scale, 0.5f, size_z);
        transform->SetEulerAngle(0.0f, 0.0f, -angle);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { scale , 0.5f, float(size_z) };
    }

    float scaleX = sqrt(2.04f);

    // -z,+x角
    {
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(size_x * 0.5f, y, -(size_z * 0.5f + xz));
        transform->SetScale(scaleX, 0.5f, 0.75f);
        transform->SetEulerAngle(0.0f, 45.0f, 0.0f);
        transform->Rotate(-angle, 0.0f, 0.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { scaleX, 0.5f,  0.75f };
    }
    {
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(size_x * 0.5f + xz, y, -size_z * 0.5f);
        transform->SetScale(0.75f, 0.5f, scaleX);
        transform->SetEulerAngle(0.0f, -45.0f, 0.0f);
        transform->Rotate(0.0f, 0.0f, -angle);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { 0.75f , 0.5f,  scaleX };
    }

    {
        // -z,-x角
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(-size_x * 0.5f, y, -(size_z * 0.5f + xz));
        transform->SetScale(scaleX, 0.5f, 0.75f);
        transform->SetEulerAngle(0.0f, -45.0f, 0.0f);
        transform->Rotate(-angle, 0.0f, 0.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { scaleX, 0.5f, 0.75f };
    }
    {
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(-(size_x * 0.5f + xz), y, -size_z * 0.5f);
        transform->SetScale(0.75f, 0.5f, scaleX);
        transform->SetEulerAngle(0.0f, 45.0f, 0.0f);
        transform->Rotate(0.0f, 0.0f, angle);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { 0.75f , 0.5f, scaleX };
    }

    {
        // +z,+x角
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(size_x * 0.5f, y, (size_z * 0.5f + xz));
        transform->SetScale(scaleX, 0.5f, 0.75f);
        transform->SetEulerAngle(0.0f, 135.0f, 0.0f);
        transform->Rotate(angle, 0.0f, 0.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { scaleX, 0.5f, 0.75f };
    }
    {
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(size_x * 0.5f + xz, y, size_z * 0.5f);
        transform->SetScale(0.75f, 0.5f, scaleX);
        transform->SetEulerAngle(0.0f, 45.0f, 0.0f);
        transform->Rotate(0.0f, 0.0f, -angle);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { 0.75f , 0.5f, scaleX };
    }

    {
        // +z,-x角
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(-size_x * 0.5f, y, (size_z * 0.5f + xz));
        transform->SetScale(scaleX, 0.5f, 0.75f);
        transform->SetEulerAngle(0.0f, -135.0f, 0.0f);
        transform->Rotate(angle, 0.0f, 0.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { scaleX, 0.5f, 0.75f };
    }
    {
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(-(size_x * 0.5f + xz), y, size_z * 0.5f);
        transform->SetScale(0.75f, 0.5f, scaleX);
        transform->SetEulerAngle(0.0f, -45.0f, 0.0f);
        transform->Rotate(0.0f, 0.0f, angle);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { 0.75f , 0.5f, scaleX };
    }
}

void GameScene::CreateStageSet() {
    // 床
    int stageSize_x = 25;
    int stageSize_z = 25;
    Vector3 posOffset(3.0f, 0.0f, 0.0f);
    float blockScale = 1.0f;
    //for (int z = 0; z < stageSize_z; ++z) {
    //    for (int x = 0; x < stageSize_x; ++x) {
    //        Vector3 pos(x - stageSize_x * 0.5f + 0.5f, 0.0f, z - stageSize_z * 0.5f + 0.5f);
    //        pos *= blockScale;

    //        auto obj = CreateGameObject();
    //        auto renderer = obj->AddComponent<MeshRenderer>();
    //        renderer->LoadModel("Assets/Model/Stage/fbx/yuka.fbx");
    //        obj->GetTransform()->SetPosition(pos + posOffset);
    //        obj->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
    //    }
    //}

    //// 柵
    //for (int z = 0; z < stageSize_z; ++z) {
    //    for (int x = 0; x < stageSize_x; ++x) {
    //        Vector3 rotateAngle = Vector3::zero;
    //        if (x == 0) rotateAngle.y = 180.0f;
    //        else if (x == stageSize_x - 1) rotateAngle.y = 0.0f;
    //        else if (z == 0) rotateAngle.y = 90.0f;
    //        else if (z == stageSize_z - 1) rotateAngle.y = 270.0f;
    //        else continue;

    //        if (x % 2 == 0 && z % 2 == 0) continue;

    //        Vector3 pos(x - stageSize_x * 0.5f + 0.5f, 0.0f, z - stageSize_z * 0.5f + 0.5f);
    //        pos *= blockScale;

    //        auto obj = CreateGameObject();
    //        auto renderer = obj->AddComponent<MeshRenderer>();
    //        renderer->LoadModel("Assets/Model/Stage/fbx/saku.fbx");
    //        renderer->SetShouldDrawShadow(true);
    //        obj->GetTransform()->SetPosition(pos + posOffset);
    //        obj->GetTransform()->SetEulerAngle(rotateAngle);
    //    }
    //}

    {
        auto obj = CreateGameObject();
        obj->AddComponent<StageSet>();
    }

    // 床当たり判定
    {
        auto obj = CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetPosition(Vector3(0.0f, -0.5f, 0.0f) + posOffset);
        transform->SetScale(stageSize_x, 1.0f, stageSize_z);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = { float(stageSize_x * 100) , 1.0f,float(stageSize_z * 100) };
    }

    //// +z壁当たり判定
    //{
    //    auto obj = CreateGameObject();
    //    auto transform = obj->GetTransform();
    //    transform->SetPosition(Vector3(0.0f, 5.0f, stageSize_z * 0.5f) + posOffset);
    //    transform->SetScale(stageSize_x, 10.0f, 1.0f);
    //    auto collider = obj->AddComponent<Collider>();
    //    collider->m_scale = { float(stageSize_x), 10.0f, 1.0f };
    //}
    //// -z壁当たり判定
    //{
    //    auto obj = CreateGameObject();
    //    auto transform = obj->GetTransform();
    //    transform->SetPosition(Vector3(0.0f, 5.0f, -stageSize_z * 0.5f) + posOffset);
    //    transform->SetScale(stageSize_x, 10.0f, 1.0f);
    //    auto collider = obj->AddComponent<Collider>();
    //    collider->m_scale = { float(stageSize_x), 10.0f, 1.0f };
    //}
    //// +x壁当たり判定
    //{
    //    auto obj = CreateGameObject();
    //    auto transform = obj->GetTransform();
    //    transform->SetPosition(Vector3(stageSize_x * 0.5f, 5.0f, 0.0f) + posOffset);
    //    transform->SetScale(1.0f, 10.0f, stageSize_z);
    //    auto collider = obj->AddComponent<Collider>();
    //    collider->m_scale = { 1.0f, 10.0f, float(stageSize_z) };
    //}
    //// -x壁当たり判定
    //{
    //    auto obj = CreateGameObject();
    //    auto transform = obj->GetTransform();
    //    transform->SetPosition(Vector3(-stageSize_x * 0.5f, 5.0f, 0.0f) + posOffset);
    //    transform->SetScale(1.0f, 10.0f, stageSize_z);
    //    auto collider = obj->AddComponent<Collider>();
    //    collider->m_scale = { 1.0f, 10.0f, float(stageSize_z) };
    //}
}

void GameScene::CreateUIObject() {
    auto uiObj = CreateGameObject();
    uiObj->AddComponent<GameUI>();
}

