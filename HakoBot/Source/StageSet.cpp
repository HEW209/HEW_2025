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
        renderer->SetShouldDrawShadow(true);
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-23.1f, -1.0f, -4.2f, Space::LOCAL);
        transform->SetEulerAngle(0.0f, 270.0f, 0.0f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Conveyors.fbx");
        renderer->SetShouldDrawShadow(true);
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
        renderer->SetShouldDrawShadow(true);
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
            material.SetPixelShader("Assets/Shader/Transparent_PS.cso");
            struct TransparentParam
            {
                float transparency;
                Vector3 pad;
            } transparentParam = { 1.0f, Vector3::zero };
            material.SetParameter(&transparentParam, sizeof(TransparentParam));
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
            material.SetPixelShader("Assets/Shader/Transparent_PS.cso");
            struct TransparentParam
            {
                float transparency;
                Vector3 pad;
            } transparentParam = { 1.0f, Vector3::zero };
            material.SetParameter(&transparentParam, sizeof(TransparentParam));
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
        m_fanAnim = renderer->LoadAnimation("Assets/Model/Stage/fbx/MiddleWall.fbx");
        for (auto& material : *renderer->GetMaterials())
        {
            material.SetVertexShader("Assets/Shader/Anime_VS.cso");
        }
        renderer->PlayAnime(m_fanAnim, true, 0.2f);
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/Transparent_PS.cso");
            struct TransparentParam
            {
                float transparency;
                Vector3 pad;
            } transparentParam = { 1.0f, Vector3::zero };
            material.SetParameter(&transparentParam, sizeof(TransparentParam));
        }
        m_pMiddleWall = renderer;
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
        renderer->SetShouldDrawShadow(true);
    }

    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-25.5f, 3.5f, 11.5f, Space::LOCAL);
        auto renderer = obj->AddComponent<MeshRenderer>();
        renderer->LoadModel("Assets/Model/Stage/fbx/Pillar.fbx");
        for (auto&& material : *renderer->GetMaterials()) {
            material.SetPixelShader("Assets/Shader/Transparent_PS.cso");
            struct TransparentParam
            {
                float transparency;
                Vector3 pad;
            } transparentParam = { 1.0f, Vector3::zero };
            material.SetParameter(&transparentParam, sizeof(TransparentParam));
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
            material.SetPixelShader("Assets/Shader/Transparent_PS.cso");
            struct TransparentParam
            {
                float transparency;
                Vector3 pad;
            } transparentParam = { 1.0f, Vector3::zero };
            material.SetParameter(&transparentParam, sizeof(TransparentParam));
        }
        m_pRightPillar = renderer;
    }

    // “–‚½‚è”»’è
    {
		// -x‘¤
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-23.1f, 7.5f, -4.2f, Space::LOCAL);
        transform->SetScale(4.0f, 15.0f, 29.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(4.0f, 15.0f, 29.0f);
    }
    {
        // +x‘¤
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(7.7f, 7.5f, -3.0f, Space::LOCAL);
        transform->SetScale(1.0f, 15.0f, 30.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(1.0f, 15.0f, 30.0f);
    }
    {
        // -z‘¤
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-6.0f, 7.5f, -15.0f, Space::LOCAL);
        transform->SetScale(30.0f, 15.0f, 1.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(30.0f, 15.0f, 1.0f);
    }
    {
        // +z‘¤
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
        transform->SetPosition(-6.0f, 7.5f, 11.5f, Space::LOCAL);
        transform->SetScale(30.0f, 15.0f, 1.0f);
        auto collider = obj->AddComponent<Collider>();
        collider->m_scale = Vector3(30.0f, 15.0f, 1.0f);
    }

    {
        // Šp‚Ì”zü•”•ª
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

        if (transparency < 1.0f) {
            m_pRightWall->SetTransparent(true);
            for (auto&& material : *m_pRightWall->GetMaterials()) {
                material.SetBlendState(BlendState::ALPHA);
                material.SetDepthStencilState(DepthStencilState::READ_ONLY);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { transparency, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
        else {
			m_pRightWall->SetTransparent(false);
            for (auto&& material : *m_pRightWall->GetMaterials()) {
                material.SetBlendState(BlendState::DEFAULT);
                material.SetDepthStencilState(DepthStencilState::DEFAULT);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { 1.0f, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Right());
        float transparency = 1.0f - Math::Clamp01(dot);

        if (transparency < 1.0f) {
            m_pLeftWall->SetTransparent(true);
            for (auto&& material : *m_pLeftWall->GetMaterials()) {
                material.SetBlendState(BlendState::ALPHA);
                material.SetDepthStencilState(DepthStencilState::READ_ONLY);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { transparency, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
        else {
            m_pLeftWall->SetTransparent(false);
            for (auto&& material : *m_pLeftWall->GetMaterials()) {
                material.SetBlendState(BlendState::DEFAULT);
                material.SetDepthStencilState(DepthStencilState::DEFAULT);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { 1.0f, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Back());
        float transparency = 1.0f - Math::Clamp01(dot);

        if (transparency < 1.0f) {
            m_pMiddleWall->SetTransparent(true);
            for (auto&& material : *m_pMiddleWall->GetMaterials()) {
                material.SetBlendState(BlendState::ALPHA);
                material.SetDepthStencilState(DepthStencilState::READ_ONLY);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { transparency, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
        else {
            m_pMiddleWall->SetTransparent(false);
            for (auto&& material : *m_pMiddleWall->GetMaterials()) {
                material.SetBlendState(BlendState::DEFAULT);
                material.SetDepthStencilState(DepthStencilState::DEFAULT);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { 1.0f, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Left());
        float transparency = 1.0f - Math::Clamp01(dot);

        if (transparency < 1.0f) {
            m_pRightPillar->SetTransparent(true);
            for (auto&& material : *m_pRightPillar->GetMaterials()) {
                material.SetBlendState(BlendState::ALPHA);
                material.SetDepthStencilState(DepthStencilState::READ_ONLY);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { transparency, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
        else {
            m_pRightPillar->SetTransparent(false);
            for (auto&& material : *m_pRightPillar->GetMaterials()) {
                material.SetBlendState(BlendState::DEFAULT);
                material.SetDepthStencilState(DepthStencilState::DEFAULT);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { 1.0f, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
    }
    {
        float dot = static_cast<Vec3>(cameraDir).Dot(Vec3::Right());
        float transparency = 1.0f - Math::Clamp01(dot);

        if (transparency < 1.0f) {
            m_pLeftPillar->SetTransparent(true);
            for (auto&& material : *m_pLeftPillar->GetMaterials()) {
                material.SetBlendState(BlendState::ALPHA);
                material.SetDepthStencilState(DepthStencilState::READ_ONLY);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { transparency, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
        else {
            m_pLeftPillar->SetTransparent(false);
            for (auto&& material : *m_pLeftPillar->GetMaterials()) {
                material.SetBlendState(BlendState::DEFAULT);
                material.SetDepthStencilState(DepthStencilState::DEFAULT);
                struct TransparentParam
                {
                    float transparency;
                    Vector3 pad;
                } transparentParam = { 1.0f, Vector3::zero };
                material.SetParameter(&transparentParam, sizeof(TransparentParam));
            }
        }
    }
}
