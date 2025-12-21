#include "Player.h"
#include "PlayerMove.h"
#include "PlayerStretch.h"
#include <Component/Collider.h>

void Player::Awake()
{
	GameObject* pObj = GetGameObject();
	//プレイヤールートにコンポーネントを追加
	auto renderer2 = pObj->AddComponent<MeshRenderer>();
	renderer2->LoadModel("Assets/Model/Player/fbx/kabu.fbx");

	auto playerMove = pObj->AddComponent<PlayerMove>();
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

	//ブロック操作コンポーネントの追加
	m_pBlockHandler = playerHead->AddComponent<PlayerBlockHandler>();

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
		playerPillar->GetTransform()->SetPosition(0.0f, i * 0.2f , 0.0f, Space::LOCAL);
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
	playerMove->SetCaterpillar(renderer4);
}
