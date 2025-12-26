#include "GuideUIController.h"
#include "Easing.h"
#include "InputSystem.h"
#include "Player.h"
#include "GameState.h"




void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();

	//もつ・おく
	m_pRenderer = GetGameObject()->AddComponent<SpriteRenderer>();
	m_pRenderer->LoadTexture("Assets/Textures/Texts/oku.png");
	m_pRenderer->SetUI(true);
	m_pRenderer->SetSize(200);
	//のびる
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/Texts/nobiru.png");
	renderer1->SetOffsetPos(0.0f, -0.6f);
	renderer1->SetUI(true);
	renderer1->SetSize(200);
	//ちぢむ
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/Texts/chijimu.png");
	renderer2->SetOffsetPos(0.0f, -1.2f);
	renderer2->SetUI(true);
	renderer2->SetSize(200);


	//初期場所取得
	m_defaultPosition = GetTransform()->GetPosition();
	m_motu = false;
}

void GuideUIController::Update()
{
	Player* player = GameState::GetInstance()->GetPlayer();
	PlayerBlockHandler* playerBlockHandler = player->GetBlockHandler();

	//ここをプレイヤーがブロックを持っていたら
	m_motu = !playerBlockHandler->HasBlock();

	if (m_motu)
	{
		m_pRenderer->LoadTexture("Assets/Textures/Texts/motu.png", false);
	}
	else
	{
		m_pRenderer->LoadTexture("Assets/Textures/Texts/oku.png", false);
	}
}
