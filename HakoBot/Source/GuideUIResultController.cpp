//GuideUIResultController.cpp
#include "GuideUIResultController.h"
#include "GuideUIController.h"
#include "Easing.h"
#include "GridField.h"
#include "GameState.h"
#include "InputManager.h"

constexpr float MAX_MENUB = 2000.0f;//メニューボタンのサイズ

void GuideUIResultController::Start()
{
	//メニュー画面
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/menuban.png");
	renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	GetTransform()->SetScale(0.0f, 0.0f, 0.0f);
	//メニュー画面ボタン
	auto renderer1 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer1->SetUI(true);
	renderer1->LoadTexture("Assets/Textures/menub.png");
	renderer1->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	renderer1->SetOffsetPos(0.0f,0.0f);
	renderer1->SetSize(MAX_MENUB);
	m_rend = renderer1;
	//メニュー画面選択
	auto renderer2 = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer2->SetUI(true);
	renderer2->LoadTexture("Assets/Textures/menuselect.png");
	renderer2->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	renderer2->SetOffsetPos(0.0f, 0.0f);
	renderer2->SetSize(0.0f);
	m_rend2 = renderer2;

	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	m_menu = false;
	m_menu2 = false;
	m_targetScale = Vector3(0.3f, 0.3f, 0.3f);
	m_closePhase = ClosePhase::None;
	m_closeValue = 0.0f;
	m_closeStartScale = Vector3::zero;
	m_menuX = 0;
	m_menuY = 0;
}

void GuideUIResultController::Update()
{
	//メニューをひらく
	if (InputManager::CurrentInputSystem().GetButtonDown("Menu"_hash))
	{
		
		m_value2 = 0.0f;
		GridField* gridfield = GameState::GetInstance()->GetGridField();
		//クリアしてたらメニュー表示できない
		if (!gridfield->IsClear())
		{
			m_menu = true;
			//ボタン受付をUIタイプに変更
			InputManager::ChangeBindType(InputBindType::UI);
		}
	}

	//メニュー開いてるとき
	if (m_menu)
	{
		//メニューを閉じるボタン押したとき
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuBack"_hash))
		{
			m_closeStartScale = GetTransform()->GetScale();
			m_menu2 = true;		//でかくするイージングoff
			m_closePhase = ClosePhase::Pop;
			m_closeValue = 0.0f;
			m_value1 = 0.0f;	//初期化
		
			InputManager::ChangeBindType(InputBindType::GAMEPLAY);
		}

		if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash))
		{
			m_menuY = 0;
		}
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash))
		{
			m_menuY = 1;
		}
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuLeft"_hash))
		{
			m_menuX = 0;
		}
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuRight"_hash))
		{
			m_menuX = 1;
		}

		//メニュー選択用
		MenuCell& cell = menuTable[m_menuY][m_menuX];
		m_rend2->SetSize(MAX_MENUB / 2.0f, MAX_MENUB / 3.5f);
		m_rend2->SetUVScale(0.5f, 0.5f);
		m_rend2->SetUVOffsetPos(cell.uvX, cell.uvY);
		m_rend2->SetOffsetPos(cell.posX, cell.posY);


		//メニュー出現
		if (!m_menu2)
		{
			m_value1 += EASING * 2.0f;
			//メニュー拡大
			Vector3 scale;
			scale.x = Easing::InSine(m_value1, EASING_MAX, m_targetScale.x, 0.0f);
			scale.y = Easing::InSine(m_value1, EASING_MAX, m_targetScale.y, 0.0f);
			scale.z = 1.0f;

			GetTransform()->SetScale(scale);

			if (m_value1 >= EASING_MAX)
			{
				m_value1 = EASING_MAX;
			}
		}

		//メニューを閉じる
		if (m_menu2)
		{
			m_closeValue += EASING * 3.0f;

			//Pop:少し拡大
			if (m_closePhase == ClosePhase::Pop)
			{
				Vector3 scale;
				scale.x = Easing::OutSine(m_closeValue,EASING_MAX / 2.0f,m_closeStartScale.x * 1.15f,m_closeStartScale.x);
				scale.y = Easing::OutSine(m_closeValue,EASING_MAX / 2.0f,m_closeStartScale.y * 1.15f,m_closeStartScale.y);
				scale.z = 1.0f;
				GetTransform()->SetScale(scale);

				//マックスになったら
				if (m_closeValue >= EASING_MAX / 2.0f)
				{
					m_closePhase = ClosePhase::Shrink;	//縮小開始
					m_closeValue = 0.0f;				//イージングの値を初期化
				}
			}

			//Shrink:縮小
			else if (m_closePhase == ClosePhase::Shrink)
			{
				Vector3 scale;
				scale.x = Easing::OutSine(m_closeValue,EASING_MAX,0.0f,m_closeStartScale.x * 1.15f);
				scale.y = Easing::OutSine(m_closeValue,EASING_MAX,0.0f,m_closeStartScale.y * 1.15f);
				scale.z = 1.0f;
				GetTransform()->SetScale(scale);

				//メニュー画面のサイズが0.1fを下回ったらサイズをゼロに
				if (scale.x <= 0.1f)
				{
					GetTransform()->SetScale(0.0f, 0.0f, 0.0f);
					m_menu = false;
					m_menu2 = false;
					m_closePhase = ClosePhase::None;
				}
			}

		}

		
	}

	


}

bool GuideUIResultController::GetMenuBool()
{
	return m_menu;
}

