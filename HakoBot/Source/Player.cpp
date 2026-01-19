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
	renderer2->SetShouldDrawShadow(true);
	auto playerMove = pObj->AddComponent<PlayerMove>();
	auto collider = pObj->AddComponent<Collider>();
	collider->m_scale = Vector3(0.7f, PlayerDefaultSize_y, 0.7f);
	collider->m_positionOffset = Vector3(0.0f, PlayerDefaultSize_y * 0.5, -0.35f);
	collider->IsStatic = false;

	// 頭作る	
	auto playerHead = SceneManager::GetActiveScene()->CreateGameObject();

	// 頭登録
	playerMove->SetHead(playerHead);

	//頭にコンポーネントを追加
	auto renderer1 = playerHead->AddComponent<MeshRenderer>();
	renderer1->LoadModel("Assets/Model/Player/fbx/jyoubu.fbx");
	renderer1->SetShouldDrawShadow(true);
	playerHead->GetTransform()->SetParent(pObj->GetTransform());
	auto playerStretch = playerHead->AddComponent<PlayerStretch>();
	playerStretch->SetPlayerCollider(collider);

	// 目
	auto eyeObj = SceneManager::GetActiveScene()->CreateGameObject();
	eyeObj->GetTransform()->SetParent(playerHead);
	eyeObj->GetTransform()->SetPosition(0.0f, 0.0f, -0.3f, Space::LOCAL);
	auto eye = eyeObj->AddComponent<MeshRenderer>();
	eye->LoadModel("Assets/Model/Player/fbx/eye.fbx");
	auto eyeAnimeNo = eye->LoadAnimation("Assets/Model/Player/fbx/eye.fbx");
	eye->GetMaterial(0)->SetVertexShader("Assets/Shader/Anime_VS.cso");
	eye->GetMaterial(0)->SetPixelShader("Assets/Shader/OneColor_PS.cso");
	Color eyeColor(0.0f, 0.5f, 0.8f, 1.0f);
	eye->GetMaterial(0)->SetParameter(&eyeColor, sizeof(eyeColor));
	eye->PlayAnime(eyeAnimeNo, true);

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
		renderer3->SetShouldDrawShadow(true);
		playerPillar->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

		// 支柱を登録
		playerStretch->SetPillarObject(playerPillar);
	}

	// キャタピラ
	auto caterpillar = SceneManager::GetActiveScene()->CreateGameObject();
	auto renderer4 = caterpillar->AddComponent<MeshRenderer>();
	renderer4->LoadModel("Assets/Model/Player/fbx/kyatapira.fbx");
	renderer4->GetMaterial(0)->SetPixelShader("Assets/Shader/UVScroll_PS.cso");
	renderer4->SetShouldDrawShadow(true);
	caterpillar->GetTransform()->SetParent(pObj->GetTransform());
	playerMove->SetCaterpillar(renderer4);
	//{
	//	auto effectObj = SceneManager::GetActiveScene()->CreateGameObject();
	//	effectObj->GetTransform()->SetScale(100.0f, 100.0f, 100.0f);
	//	effectObj->GetTransform()->SetParent(GetTransform());
	//	auto effect = effectObj->AddComponent<EffectRenderer>();
	//	effect->Load("Assets/Effect/dash.efkefc");
	//	effect->Play();
	//}
}
