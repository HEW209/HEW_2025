#include "StageSet.h"
#include "Vec.h"
#include "GameState.h"

StageSet::StageSet()
    : m_time(0.0f)
    , m_fanAnim(MeshGroup::ANIME_NONE)
{
}

void StageSet::Awake()
{
    GameState::GetInstance()->SetStageSize(Vector3(29.2f, 25.0f, 25.0f));
    GameState::GetInstance()->SetStagePos(Vector3(-7.0f, 0.0f, -2.0f));

    AlphaDitherParam alphaDitherParam;
	alphaDitherParam.pad = Vector3::zero;

    // スカイドーム
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(0.0f, -10.0f, 0.0f);
        transform->SetScale(500.0f, 500.0f, 500.0f);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/SkyDome.fbx");
        renderer->GetMaterial(0)->SetPixelShader("Assets/Shader/Unlit_PS.cso");
    }

    // 柵
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(2.5f, 0.75f, -11.5f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Boundary.fbx");
		renderer->SetTransparent(true);
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/Boundary_PS.cso");
            material.SetBlendState(BlendState::ALPHA);
            material.SetDepthStencilState(DepthStencilState::READ_ONLY);
            struct TransparentParam
            {
                float transparency;
                Vector3 pad;
            } transparentParam = { 0.6f, Vector3::zero };
            material.SetParameter(&transparentParam, sizeof(TransparentParam));
        }
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-23.1f, -1.0f, -4.2f, Space::LOCAL);
        transform->SetEulerAngle(0.0f, 270.0f, 0.0f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Conveyor_frame.fbx");
        //renderer->SetShouldDrawShadow(true);
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-23.1f, -1.0f, -4.2f, Space::LOCAL);
        transform->SetEulerAngle(0.0f, 270.0f, 0.0f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Conveyors.fbx");
        //renderer->SetShouldDrawShadow(true);
        renderer->GetMaterial(0)->SetPixelShader("Assets/Shader/UVScroll_PS.cso");
        struct Params {
            Vector2 uvOffset = { 0.0f, 0.0f };
        } params;
        renderer->GetMaterial(0)->SetParameter(&params, sizeof(Params));
        m_pConveyors = renderer;
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/outlet.fbx");
        //renderer->SetShouldDrawShadow(true);
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Floor.fbx");
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-25.5f, 3.5f, -1.0f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/LeftWall.fbx");
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/AlphaDither_PS.cso");
			alphaDitherParam.transparency = 1.0f;
            material.SetParameter(&alphaDitherParam, sizeof(alphaDitherParam));
        }
        m_pLeftWall = renderer;
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(8.0f, 3.5f, 2.5f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/RightWall.fbx");
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/AlphaDither_PS.cso");
            alphaDitherParam.transparency = 1.0f;
            material.SetParameter(&alphaDitherParam, sizeof(alphaDitherParam));
        }
        m_pRightWall = renderer;
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-8.5f, 5.0f, 11.0f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/MiddleWall.fbx");
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/AlphaDither_PS.cso");
            alphaDitherParam.transparency = 1.0f;
            material.SetParameter(&alphaDitherParam, sizeof(alphaDitherParam));
        }
        m_pMiddleWall = renderer;
    }

    // ファン
    for (int i = 0; i < 3; ++i)
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-14.5f + i * 2.0f, 8.5f, 10.2f, Space::LOCAL);
        transform->Rotate(0.0f, 0.0f, i * 20.0f);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Fan.fbx");
        m_pFans[i] = renderer;
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/pin.fbx");
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-14.0f, 0.03f, -3.0f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/sheet.fbx");
        //renderer->SetShouldDrawShadow(true);
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-25.5f, 3.5f, 11.5f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Pillar.fbx");
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/AlphaDither_PS.cso");
            alphaDitherParam.transparency = 1.0f;
            material.SetParameter(&alphaDitherParam, sizeof(alphaDitherParam));
        }
        m_pLeftPillar = renderer;
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(8.0f, 3.5f, 11.5f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Pillar.fbx");
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/AlphaDither_PS.cso");
            alphaDitherParam.transparency = 1.0f;
            material.SetParameter(&alphaDitherParam, sizeof(alphaDitherParam));
        }
        m_pRightPillar = renderer;
    }

    // 当たり判定
    {
		// -x側
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-23.1f, 7.5f, -4.2f, Space::LOCAL);
        transform->SetScale(4.0f, 15.0f, 29.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(4.0f, 15.0f, 29.0f);
    }
    {
        // +x側
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(7.7f, 7.5f, -3.0f, Space::LOCAL);
        transform->SetScale(1.0f, 15.0f, 30.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(1.0f, 15.0f, 30.0f);
    }
    {
        // -z側
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-6.0f, 7.5f, -15.0f, Space::LOCAL);
        transform->SetScale(30.0f, 15.0f, 1.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(30.0f, 15.0f, 1.0f);
    }
    {
        // +z側
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-6.0f, 7.5f, 11.5f, Space::LOCAL);
        transform->SetScale(30.0f, 15.0f, 1.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(30.0f, 15.0f, 1.0f);
    }

    {
        // 角の配線部分
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-21.8f, 7.5f, 10.5f, Space::LOCAL);
        transform->SetScale(10.0f, 15.0f, 1.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(10.0f, 15.0f, 1.0f);
    }
}

void StageSet::Update()
{
    m_time += Time::GetDeltaTime();

    // コンベアUVスクロール
    struct Params {
        Vector2 uvOffset;
    } params;
    params.uvOffset = { m_time * -0.1f, 0.0f };
    m_pConveyors->GetMaterial(0)->SetParameter(&params, sizeof(Params));

	Camera* pCamera = Camera::GetMain();
    Vector3 cameraDir = pCamera->GetTransform()->GetQuaternion() * Vector3::forward;
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Left());
        float transparency = 1.0f - Math::Clamp01(dot);
        SetTransparent(m_pRightWall.Get(), transparency);
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Right());
        float transparency = 1.0f - Math::Clamp01(dot);
        SetTransparent(m_pLeftWall.Get(), transparency);
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Back());
        float transparency = 1.0f - Math::Clamp01(dot);
        SetTransparent(m_pMiddleWall.Get(), transparency);

        // ファン
        for (int i = 0; i < 3; ++i)
        {
            if (transparency < 1.0f)
            {
                m_pFans[i]->SetEnabled(false);
            }
            else
            {
                m_pFans[i]->SetEnabled(true);
            }

            m_pFans[i]->GetTransform()->Rotate(0.0f, 0.0f, Time::GetDeltaTime() * 360.0f);
        }
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Left());
        float transparency = 1.0f - Math::Clamp01(dot);
        SetTransparent(m_pRightPillar.Get(), transparency);
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Right());
        float transparency = 1.0f - Math::Clamp01(dot);
        SetTransparent(m_pLeftPillar.Get(), transparency);
    }
}

void StageSet::SetTransparent(MeshRenderer* renderer, float transparency)
{
    AlphaDitherParam alphaDitherParam;
    alphaDitherParam.pad = Vector3::zero;

    if (transparency < 1.0f) {
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/AlphaDither_PS.cso");
            alphaDitherParam.transparency = transparency;
            material.SetParameter(&alphaDitherParam, sizeof(alphaDitherParam));
        }
    }
    else {
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/Default_PS.cso");
        }
    }
}
