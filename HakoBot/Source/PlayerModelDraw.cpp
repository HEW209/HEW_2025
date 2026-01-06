#include "PlayerModelDraw.h"
#include "PlayerMove.h"
#include "PlayerStretch.h"
#include "TitlePlayerMove.h"

//ブロック読込み用
#include "EditorTypes.h"
#include "LevelSerializer.h"

#include "GameScene.h"
#include "GameState.h"
//12/25 ブロックの描画が未実装
void PlayerModelDraw::Awake()
{
	GameObject* pObj = GetGameObject();
	//プレイヤールートにコンポーネントを追加
	auto renderer2 = pObj->AddComponent<MeshRenderer>();
	renderer2->LoadModel("Assets/Model/Player/fbx/kabu.fbx");


	auto collider = pObj->AddComponent<Collider>();
	collider->m_scale = Vector3(0.7f, PlayerDefaultSize_y, 0.7f);
	collider->m_positionOffset = Vector3(0.0f, 0.5f, 0.0f);
	collider->IsStatic = false;

	// 頭作る	
	auto playerHead = SceneManager::GetActiveScene()->CreateGameObject();

	//頭にコンポーネントを追加
	auto renderer1 = playerHead->AddComponent<MeshRenderer>();
	renderer1->LoadModel("Assets/Model/Player/fbx/jyoubu.fbx");
	playerHead->GetTransform()->SetParent(pObj->GetTransform());
	auto playerStretch = playerHead->AddComponent<PlayerStretch>();
	playerStretch->SetPlayerCollider(collider);
	

	{//ブロック操作コンポーネントの追加
		m_pBlockHolder = SceneManager::GetActiveScene()->CreateGameObject();
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		obj->GetComponent<Transform>()->SetParent(m_pBlockHolder->GetTransform());
		m_pBlockObject = obj->AddComponent<BlockObject>();
		m_pBlockObject->SetUseCollider(false);
	}
	{//アニメーション
		MeshGroup::AnimeNo m_motuAnime;
		m_motuAnime = renderer1->LoadAnimation("Assets/Model/Player/fbx/motiage.fbx");
		auto materials = renderer1->GetMaterials();
		for (auto& material : *materials)
		{
			material.SetVertexShader("Assets/Shader/Anime_VS.cso");
		}
		renderer1->PlayAnime(m_motuAnime, true);
	}



	// 支柱作る
	int max_gridStretch = 4;
	int max_pillar = max_gridStretch * 5 + 2;
	for (int i = 0; i < max_pillar; ++i)
	{
		auto playerPillar = SceneManager::GetActiveScene()->CreateGameObject();
		playerPillar->SetActive(false);

		// 支柱にコンポーネントつける
		auto renderer3 = playerPillar->AddComponent<MeshRenderer>();
		renderer3->LoadModel("Assets/Model/Player/fbx/sityu.fbx");
		playerPillar->GetTransform()->SetParent(pObj->GetTransform());
		playerPillar->GetTransform()->SetPosition(0.0f, i * 0.2f, 0.0f, Space::LOCAL);
		playerPillar->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

		// 支柱を登録
		playerStretch->SetPillarObject(playerPillar);
	}

	// キャタピラ
	auto caterpillar = SceneManager::GetActiveScene()->CreateGameObject();
	auto renderer4 = caterpillar->AddComponent<MeshRenderer>();
	renderer4->LoadModel("Assets/Model/Player/fbx/kyatapira.fbx");
	renderer4->GetMaterial(0)->SetPixelShader("Assets/Shader/UVScroll_PS.cso");
	caterpillar->GetTransform()->SetParent(pObj->GetTransform());


	{
		
		std::string blockPath = "Assets/Level/Blocks/" + blockFileNames[0] + ".json";


		BlockTemplateData blockData;
		if (LevelSerializer::LoadBlockTemplate(blockPath, blockData)) {

			BlockSetData m_pBlockData;
			for (const auto& pos : blockData.blocks) {
				m_pBlockData.blocks.push_back({ pos.x, pos.y, pos.z });
			}

			auto obj = SceneManager::GetActiveScene()->CreateGameObject();

			auto blockComp = obj->AddComponent<BlockObject>();
			blockComp->SetBlockSet(m_pBlockData);

			if (!blockData.modelPath.empty()) {
				blockComp->SetModel(blockData.modelPath);
			}
			float startX = 8.0f;
			float startZ = 5.0f;
			float intervalZ = -3.0f;
			auto position = playerHead->GetTransform()->GetPosition();
			//Vector3 pos;
			//pos.x = startX + position.x;
			//pos.y = position.y;
			//pos.z = startZ + ( intervalZ)+position.z;

			obj->GetTransform()->SetPosition(GetTransform()->GetPosition().x, 1.0f, GetTransform()->GetPosition().z);
			GameState* gameState = nullptr;
			gameState = GameState::GetInstance();
			if (gameState)gameState->AppendWorldBlock(blockComp);
			
		}
	}
}

void PlayerModelDraw::Update()
{
	//auto playerTransform = GetTransform();
	//m_pBlockObject->GetTransform()->SetParent(playerTransform);
}
