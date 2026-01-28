#include "TitleStage.h"
#include "SaveData.h"
#include "LevelSerializer.h"
#include "BlockData.h"

const static Vector3 g_blockDefaultPos(8.0f, 2.32f, 11.8f);
const static float g_blockEndPos_x = -25.0f;
const static float g_blockMoveSpeed = 2.05f;

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

    // ブロック
	for (int i = 0; i < 3; ++i)
    {
        auto blockObj = SceneManager::GetActiveScene()->CreateGameObject();
        m_block[i] = blockObj->AddComponent<MeshRenderer>();
        m_block[i]->SetShouldDrawShadow(true);
		SetRandomBlock(m_block[i].Get());
		blockObj->GetTransform()->SetPosition(g_blockDefaultPos + Vector3::right * 11.0f * i);
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

	for (int i = 0; i < 3; ++i)
	{
		m_block[i]->GetTransform()->Translate(Vector3::left * g_blockMoveSpeed * 1.0f / 60.0f);
		if (m_block[i]->GetTransform()->GetPosition().x < g_blockEndPos_x)
		{
			SetRandomBlock(m_block[i].Get());
		}
	}
}

void TitleStage::SetRandomBlock(MeshRenderer* block)
{

	SaveData::Load();
	int clearStage = SaveData::GetClearLevel();

	int randIndex = rand() % std::max(clearStage, 5) + 1;
	LevelData levelData;
	LevelSerializer::LoadLevelData("Assets/Level/Stages/Level" + std::to_string(randIndex) + ".json", levelData);
	BlockTemplateData blockData;
	if (levelData.inventoryBlockFiles.empty()) {
		LevelSerializer::LoadBlockTemplate("Assets/Level/Blocks/1masu.json", blockData);
	}
	else {
		LevelSerializer::LoadBlockTemplate("Assets/Level/Blocks/" + levelData.inventoryBlockFiles[0] + ".json", blockData);
	}
	BlockSetData blockSet;
	blockSet.blocks = blockData.blocks;

	Vector3 min{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	Vector3 max{ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
	for (auto&& blockPos : blockSet.blocks) {
		// ブロックの最小位置を記録
		min.x = std::min(min.x, (float)blockPos.x);
		min.y = std::min(min.y, (float)blockPos.y);
		min.z = std::min(min.z, (float)blockPos.z);

		// ブロックの最大位置を記録
		max.x = std::max(max.x, (float)blockPos.x);
		max.y = std::max(max.y, (float)blockPos.y);
		max.z = std::max(max.z, (float)blockPos.z);
	}
	Vector3 center = (min + max) * 0.5f;

	// ブロック（立方体）の中心から見た8つの頂点へのオフセット
	const std::vector<Vector3> cornerOffsets = {
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f }
	};

	float minY = (std::numeric_limits<float>::max)();

	const Quaternion rotation = GetTransform()->GetQuaternion();

	for (const auto& blockPos : blockSet.blocks) {
		// ブロックの中心座標
		const Vector3 center(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z));

		for (const auto& offset : cornerOffsets) {
			// ブロックの頂点座標
			Vector3 corner = center + offset;

			Vector3 rotatedCorner = rotation * corner;

			if (rotatedCorner.y < minY) {
				minY = rotatedCorner.y;
			}
		}
	}

	float groundY = (minY == (std::numeric_limits<float>::max)()) ? 0.0f : -minY;

	Vector3 offset = center * -1.0f;
	offset.y = groundY;

	block->LoadModel(blockData.modelPath);
	block->GetTransform()->SetPosition(g_blockDefaultPos + offset);
}

