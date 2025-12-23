#include "GuideUIController.h"
#include "Easing.h"
#include "InputSystem.h"
#include "Player.h"
#include "GameState.h"
#include "InputManager.h"


constexpr float B_SIZE_MAX = MOZI_SIZE - 40.0f;
constexpr float B_SIZE_MIN = B_SIZE_MAX * 0.8f;
constexpr float RTLT_SIZE_MAX = MOZI_SIZE - 30.0f;
constexpr float RTLT_SIZE_MIN = RTLT_SIZE_MAX * 0.8f;
constexpr float KAITEN_MAX = 80.0f;
constexpr float RB_MAX = 80.0f;
constexpr float LB_MAX = 80.0f;


void GuideUIController::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<SpriteRenderer>();
	//のびる
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->LoadTexture("Assets/Textures/nobiru.png");
	renderer1->SetOffsetPos(0.15f, -0.55f);
	renderer1->SetUI(true);
	renderer1->SetSize(MOZI_SIZE);
	//ちぢむ
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->LoadTexture("Assets/Textures/tidimu.png");
	renderer2->SetOffsetPos(0.15f, -1.1f);
	renderer2->SetUI(true);
	renderer2->SetSize(MOZI_SIZE);
	//RT
	auto renderer3 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer3->LoadTexture("Assets/Textures/nobiruRT.png");
	renderer3->SetOffsetPos(-0.7f,-0.55f);
	renderer3->SetUI(true);
	renderer3->SetSize(RTLT_SIZE_MAX);
	m_rtRend = renderer3;
	//LT
	auto renderer4 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer4->LoadTexture("Assets/Textures/tidimuLT.png");
	renderer4->SetOffsetPos(-0.7f, -1.1f);
	renderer4->SetUI(true);
	renderer4->SetSize(RTLT_SIZE_MAX);
	m_ltRend = renderer4;
	//B
	auto renderer5 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer5->LoadTexture("Assets/Textures/B.png");
	renderer5->SetUI(true);
	renderer5->SetUVScale(1.0f / 2.0f, 1.0f);
	renderer5->SetOffsetPos(-0.7f, 0.0f);
	renderer5->SetSize(B_SIZE_MAX);
	m_bRend = renderer5;
	//回転
	auto renderer6 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer6->LoadTexture("Assets/Textures/kaiten.png");
	renderer6->SetUI(true);
	renderer6->SetOffsetPos(0.05f, 0.6f);
	renderer6->SetSize(0.0f);
	m_kaitenRend = renderer6;
	//RB
	auto renderer7 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer7->LoadTexture("Assets/Textures/RB.png");
	renderer7->SetUI(true);
	renderer7->SetOffsetPos(-0.7f, 0.6f);
	renderer7->SetSize(0.0f);
	m_rbRend = renderer7;
	//LB
	auto renderer8 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer8->LoadTexture("Assets/Textures/LB.png");
	renderer8->SetUI(true);
	renderer8->SetOffsetPos(-1.2f, 0.6f);
	renderer8->SetSize(0.0f);
	m_lbRend = renderer8;
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
		m_pRenderer->SetSize(MOZI_SIZE - 30.0f);
		m_kaitenRend->SetSize(0.0f);
		m_rbRend->SetSize(0.0f);
		m_lbRend->SetSize(0.0f);
	}
	else
	{
		m_pRenderer->LoadTexture("Assets/Textures/oku.png");
		m_pRenderer->SetSize(MOZI_SIZE - 30.0f);
		m_kaitenRend->SetSize(KAITEN_MAX);
		m_rbRend->SetSize(RB_MAX);
		m_lbRend->SetSize(LB_MAX);
	}
	
		//Bボタン
		if (InputManager::CurrentInputSystem().GetButtonDown("PlaceAndRemove"_hash))
		{
			//ボタン小さくする
			m_bRend->SetColor(0.5f, 0.5f, 0.5f,1.0f);
			m_bRend->SetSize(B_SIZE_MIN);
		}
		if (InputManager::CurrentInputSystem().GetButtonUp("PlaceAndRemove"_hash))
		{
			//ボタンサイズ戻す
			m_bRend->SetColor(1.0f, 1.0f, 1.0f,1.0f);
			m_bRend->SetSize(B_SIZE_MAX);
		}
		

		//RTボタン
		if (InputManager::CurrentInputSystem().GetButtonDown("Up"_hash))
		{
			//ボタン小さくする
			m_rtRend->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
			m_rtRend->SetSize(RTLT_SIZE_MIN);
		}
		if (InputManager::CurrentInputSystem().GetButtonUp("Up"_hash))
		{
			//ボタンサイズ戻す
			m_rtRend->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_rtRend->SetSize(RTLT_SIZE_MAX);
		}
	
		//LTボタン
		if (InputManager::CurrentInputSystem().GetButtonDown("Down"_hash))
		{
			//ボタン小さくする
			m_ltRend->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
			m_ltRend->SetSize(RTLT_SIZE_MIN);
		}
		if (InputManager::CurrentInputSystem().GetButtonUp("Down"_hash))
		{
			//ボタンサイズ戻す
			m_ltRend->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_ltRend->SetSize(RTLT_SIZE_MAX);
		}
}
