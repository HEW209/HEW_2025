#include "GuideUIController.h"
#include "Easing.h"
#include "InputSystem.h"
#include "Player.h"
#include "GameState.h"




void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	//のびる
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/nobiruRT.png");
	renderer1->SetOffsetPos(0.1f, -0.8f);
	renderer1->SetUI(true);
	renderer1->SetSize(MOZI_SIZE + 50.0f);
	//ちぢむ
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/tidimuLT.png");
	renderer2->SetOffsetPos(0.1f, -1.6f);
	renderer2->SetUI(true);
	renderer2->SetSize(MOZI_SIZE + 50.0f);


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
		m_pRenderer->LoadTexture("Assets/Textures/motuB.png");
		m_pRenderer->SetSize(MOZI_SIZE + 70.0f);
	}
	else
	{
		m_pRenderer->LoadTexture("Assets/Textures/okuB.png");
		m_pRenderer->SetSize(MOZI_SIZE + 70.0f);
	}
	
	

	
	
}
