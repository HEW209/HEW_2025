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

	pObj->AddComponent<PlayerMove>();
	auto collider = pObj->AddComponent<Collider>();
	collider->m_scale = Vector3(0.7f, 1.0f, 0.7f);
	collider->m_positionOffset = Vector3(0.0f, 0.5f, 0.0f);
	collider->IsStatic = false;

	// 頭作る	
	auto playerHead = SceneManager::GetActiveScene()->CreateGameObject();

	//頭にコンポーネントを追加
	auto renderer1 = playerHead->AddComponent<MeshRenderer>();
	renderer1->LoadModel("Assets/Model/Player/fbx/jyoubu.fbx");
	playerHead->GetTransform()->SetParent(pObj->GetTransform());
	auto playerStretch = playerHead->AddComponent<PlayerStretch>();

	//ブロック操作コンポーネントの追加
	m_pBlockHandler = playerHead->AddComponent<PlayerBlockHandler>();

	// 支柱作る
	auto playerPillar = SceneManager::GetActiveScene()->CreateGameObject();

	// 支柱にコンポーネントつける
	auto renderer3 = playerPillar->AddComponent<MeshRenderer>();
	renderer3->LoadModel("Assets/Model/Player/fbx/sityu.fbx");
	playerPillar->GetTransform()->SetParent(pObj->GetTransform());
	playerPillar->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

	// 支柱を登録
	playerStretch->SetPillarObject(playerPillar);
}
