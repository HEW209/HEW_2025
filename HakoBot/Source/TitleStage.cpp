#include "TitleStage.h"

TitleStage::TitleStage():
	m_time(0.0f)
{
}

void TitleStage::Awake()
{
    auto transform = GetTransform();
    transform->Rotate(Vector3(0.0f, 180.0f, 0.0f));

    // ステージ本体
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto mesh = obj->AddComponent<MeshRenderer>();
        mesh->LoadModel("Assets/Model/Title/fbx/Stage.fbx");
        auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
    }

    // 柵
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto mesh = obj->AddComponent<MeshRenderer>();
        mesh->LoadModel("Assets/Model/Title/fbx/saku.fbx");
        mesh->SetTransparent(true);
        for (auto&& material : *mesh->GetMaterials()) {
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
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
    }

    // コンベア
    {
        auto obj = SceneManager::GetActiveScene()->CreateGameObject();
        auto mesh = obj->AddComponent<MeshRenderer>();
        mesh->LoadModel("Assets/Model/Title/fbx/LongConveyors.fbx");
        auto transform = obj->GetTransform();
        transform->SetParent(GetTransform());
		transform->Translate(0.0f, -0.5f, 0.0f);

        mesh->GetMaterial(0)->SetPixelShader("Assets/Shader/UVScroll_PS.cso");
        struct Params {
            Vector2 uvOffset = { 0.0f, 0.0f };
        } params;
        mesh->GetMaterial(0)->SetParameter(&params, sizeof(Params));
        m_pConveyors = mesh;
    }
}

void TitleStage::Update()
{
    m_time += Time::GetDeltaTime();

    struct Params {
        Vector2 uvOffset;
    } params;
    params.uvOffset = { m_time * -0.1f, 0.0f };
    m_pConveyors->GetMaterial(0)->SetParameter(&params, sizeof(Params));
}
