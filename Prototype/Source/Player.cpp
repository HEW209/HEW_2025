#include "Player.h"
#include "PlayerMove.h"
#include "PlayerBlockHandler.h"
#include "PlayerStretch.h"

void Player::Awake()
{
	GameObject* pObj = GetGameObject();
	//プレイヤールートにコンポーネントを追加
	auto renderer2 = pObj->AddComponent<MeshRenderer>();
	renderer2->LoadModel("Assets/Model/HEW/FBX/kabu.fbx");

	pObj->AddComponent<PlayerMove>();
	auto collider = pObj->AddComponent<Collider>();
	collider->IsStatic = false;

	// 頭作る	
	auto playerHead = SceneManager::GetActiveScene()->CreateGameObject();

	//頭にコンポーネントを追加
	auto renderer1 = playerHead->AddComponent<MeshRenderer>();
	renderer1->LoadModel("Assets/Model/HEW/FBX/jyoubu.fbx");
	playerHead->GetTransform()->SetParent(pObj->GetTransform());
	playerHead->AddComponent<PlayerStretch>();

	//ブロック操作コンポーネントの追加
	auto blockHandler = playerHead->AddComponent<PlayerBlockHandler>();

	// 支柱作る
	auto playerPillar = SceneManager::GetActiveScene()->CreateGameObject();

	// 支柱にコンポーネントつける
	auto renderer3 = playerPillar->AddComponent<MeshRenderer>();
	renderer3->LoadModel("Assets/Model/HEW/FBX/sityu.fbx");
	playerPillar->GetTransform()->SetParent(pObj->GetTransform());
	playerPillar->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

	//ブロックオブジェクトを生成
	auto blockObj = SceneManager::GetActiveScene()->CreateGameObject();

	//オフセットで位置を変える
	blockObj->GetTransform()->SetPosition(0.0f, 2.0f, 0.0f);
	auto blockComponent = blockObj->AddComponent<BlockObject>();
	blockHandler->SetBlockObject(blockComponent);
}
