#include "GuideUIController.h"
#include "Easing.h"
#include "InputSystem.h"
#include "Player.h"
#include "GameState.h"
#include "InputManager.h"


constexpr float B_SIZE_MAX = MOZI_SIZE - 40.0f;
constexpr float B_SIZE_MIN = B_SIZE_MAX * 0.8f;


void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	//のびる
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/nobiru.png");
	renderer1->SetOffsetPos(0.2f, -0.55f);
	renderer1->SetUI(true);
	renderer1->SetSize(MOZI_SIZE + 50.0f);
	//ちぢむ
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/tidimu.png");
	renderer2->SetOffsetPos(0.2f, -1.1f);
	renderer2->SetUI(true);
	renderer2->SetSize(MOZI_SIZE + 50.0f);
	//RT
	auto renderer3 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer3->LoadTexture("Assets/Textures/nobiruRT.png");
	renderer3->SetOffsetPos(-0.7f,-0.55f);
	renderer3->SetUI(true);
	renderer3->SetSize(MOZI_SIZE - 30.0f);
	//LT
	auto renderer4 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer4->LoadTexture("Assets/Textures/tidimuLT.png");
	renderer4->SetOffsetPos(-0.7f, -1.1f);
	renderer4->SetUI(true);
	renderer4->SetSize(MOZI_SIZE - 30.0f);
	//B
	auto renderer5 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer5->LoadTexture("Assets/Textures/B.png");
	renderer5->SetUI(true);
	renderer5->SetUVScale(1.0f / 2.0f, 1.0f);
	renderer5->SetOffsetPos(-0.7f, 0.0f);
	renderer5->SetSize(B_SIZE_MAX);
	m_bRend = renderer5;
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
		m_pRenderer->LoadTexture("Assets/Textures/motu.png");
		m_pRenderer->SetSize(MOZI_SIZE + 20.0f);
	}
	else
	{
		m_pRenderer->LoadTexture("Assets/Textures/oku.png");
		m_pRenderer->SetSize(MOZI_SIZE + 20.0f);
	}
	
		if (InputManager::CurrentInputSystem().GetButtonDown("PlaceAndRemove"_hash))
		{
			//薄い方にUVを変える
			m_bRend->SetColor(0.5f, 0.5f, 0.5f,1.0f);
			m_bRend->SetSize(B_SIZE_MIN);
		}
		if (InputManager::CurrentInputSystem().GetButtonUp("PlaceAndRemove"_hash))
		{
			//濃い方にUVを変える
			m_bRend->SetColor(1.0f, 1.0f, 1.0f,1.0f);
			m_bRend->SetSize(B_SIZE_MAX);
		}
	
	
	
}
