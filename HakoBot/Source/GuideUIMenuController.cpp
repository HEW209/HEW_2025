//GuideUIMenuController.cpp
#include "GuideUIMenuController.h"
#include "Easing.h"
#include "GridField.h"
#include "GameState.h"
#include "InputManager.h"
#include "GameScene.h"
#include "StageSelectScene.h"
#include "Fade.h"
#include "SoundManager.h"

constexpr float BUTTON_SIZE = 1100.0f;//メニューボタンのサイズ
constexpr float MAX_MENU = 5000.0f;
constexpr float MENU_SCALE = 0.3f;
constexpr float MANUAL_SCALE = 0.4f;	// 操作説明の時のフレームのスケール
constexpr float EASING_TIME = 0.3f;		// イージングする秒数
constexpr float MANUAL_SIZE = 3200.0f;	// 

GuideUIMeneController::GuideUIMeneController():
	m_menuState(MenuState::DEFAULT),
	m_time(0.0f)
{
}

void GuideUIMeneController::Start()
{
	GetTransform()->SetScale(0.0f, 0.0f, 0.0f);

	//メニュー画面
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/GameUI/menu_frame.png");
	renderer->SetSize(MAX_MENU);
	m_frame = renderer;

	//メニュー背景
	auto back = GetGameObject()->AddComponent<SpriteRenderer>();
	back->SetUI(true);
	back->LoadTexture("Assets/Textures/GameUI/menu_screen.png");
	back->SetOffsetPos(0.0f, 0.0f);
	back->SetUVScale(1.0f, 1.0f);
	back->SetSize(MAX_MENU * 0.5f);
	m_back = back;

	// 見出し
	{
		auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/GameUI/menu_midashi.png");
		renderer->SetSize(MAX_MENU);
		m_midashi = renderer;
	}

	//メニュー画面ボタン
	Vector2 pos[4] = { {-5.5f, 2.6f}, {5.5f, 2.6f}, {-5.5f, -2.8f}, {5.5f, -2.8f} };
	Vector2 uvPos[4] = { {0.0f, 0.0f}, {0.5f, 0.0f}, {0.0f, 0.5f}, {0.5f, 0.5f} };
	for (int i = 0; i < 4; ++i)
	{
		auto button = GetGameObject()->AddComponent<SpriteRenderer>();
		button->SetUI(true);
		button->LoadTexture("Assets/Textures/GameUI/menub.png");
		button->SetOffsetPos(pos[i]);
		button->SetSize(BUTTON_SIZE);
		button->SetUVOffsetPos(uvPos[i]);
		button->SetUVScale(0.5f, 0.5f);
		m_buttons[i] = button;
	}

	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	m_targetScale = Vector3(MENU_SCALE, MENU_SCALE, MENU_SCALE);
	m_closePhase = ClosePhase::None;
	m_closeValue = 0.0f;
	m_closeStartScale = Vector3::zero;
	m_menuX = 0;
	m_menuY = 0;

	m_bgScrollY = 0.0f;
	m_bgScrollSpeed = 0.001f;

	// 操作説明
	{
		m_manual = GetGameObject()->AddComponent<SpriteRenderer>();
		m_manual->SetUI(true);
		m_manual->LoadTexture("Assets/Textures/GameUI/Menu_sousasetumei.png");	
		m_manual->SetEnabled(false);
		m_manual->SetSize(MANUAL_SIZE);
	}

	// カーソル反映
	int selectIndex = m_menuX + m_menuY * 2;
	for (int i = 0; i < 4; ++i)
	{
		if (selectIndex == i)
		{
			m_buttons[i]->LoadTexture("Assets/Textures/GameUI/menuselect.png", false);
		}
		else
		{
			m_buttons[i]->LoadTexture("Assets/Textures/GameUI/menub.png", false);
		}
	}
}

void GuideUIMeneController::Update()
{
	switch (m_menuState)
	{
	case GuideUIMeneController::DEFAULT:
		UpdateDefault();
		break;

	case GuideUIMeneController::MenuState::OPEN:
		UpdateOpen();
		break;

	case GuideUIMeneController::SELECT:
		UpdateSelect();
		break;

	case GuideUIMeneController::MenuState::CLOSE:
		UpdateClose();
		break;
	
	case GuideUIMeneController::SCENE_CHANGE:
		UpdateChangeScene();
		break;

	case GuideUIMeneController::MANUAL:
		UpdateManual();
		break;
	}
}

void GuideUIMeneController::UpdateDefault()
{
	//メニューをひらく
	if (InputManager::CurrentInputSystem().GetButtonDown("Menu"_hash))
	{
		m_value2 = 0.0f;
		GridField* gridfield = GameState::GetInstance()->GetGridField();
		//クリアしてたらメニュー表示できない
		if (!GameState::GetInstance()->IsClear())
		{
			GameState::GetInstance()->StopTimer();
			m_menuState = MenuState::OPEN;
			//ボタン受付をUIタイプに変更
			InputManager::ChangeBindType(InputBindType::UI);
			SoundManager::PlaySE("Menu_Open", 1.0f, false);
		}
	}
}

void GuideUIMeneController::UpdateOpen()
{
	//メニュー出現
	m_value1 += 0.2f;
	Vector3 scale;
	scale.x = Easing::InSine(m_value1, 5.0f, m_targetScale.x, 0.0f);
	scale.y = Easing::InSine(m_value1, 5.0f, m_targetScale.y, 0.0f);
	scale.z = 1.0f;
	GetTransform()->SetScale(scale);

	if (m_value1 >= 5.0f)
	{
		m_value1 = 5.0f;
		m_menuState = MenuState::SELECT;
	}
}

void GuideUIMeneController::UpdateSelect()
{
	//メニュー開いてるとき
	//位置追従
	m_back->GetTransform()->SetPosition(
		m_frame->GetTransform()->GetPosition()
	);

	//背景スクロール
	m_bgScrollY -= m_bgScrollSpeed;

	//ループ処理
	while (m_bgScrollY >= 1.0f)
	{
		m_bgScrollY -= 1.0f;
	}
	while (m_bgScrollY < 0.0f)
	{
		m_bgScrollY += 1.0f;
	}

	//UVオフセットでスクロール
	m_back->SetUVOffsetPos(0.0f, m_bgScrollY);

	//メニューを閉じるボタンかメニューのキャンセルボタン押したとき
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuBack"_hash) || InputManager::CurrentInputSystem().GetButtonDown("MenuClose"_hash))
	{
		SoundManager::PlaySE("Menu_Close", 1.0f, false);
		GameState::GetInstance()->StartTimer();
		m_closeStartScale = GetTransform()->GetScale();
		m_menuState = MenuState::CLOSE;
		m_closePhase = ClosePhase::Pop;
		m_closeValue = 0.0f;
		m_value1 = 0.0f;
		InputManager::ChangeBindType(InputBindType::GAMEPLAY);
	}

	// カーソル移動
	Vector2 input = Input::GetLeftStick();
	int targetX = m_menuX;
	int targetY = m_menuY;
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash) || input.y > 0.5f)
	{
		targetY = 0;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash) || input.y < -0.5f)
	{
		targetY = 1;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuLeft"_hash) || input.x < -0.5f)
	{
		targetX = 0;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuRight"_hash) || input.x > 0.5f)
	{
		targetX = 1;
	}

	// カーソル移動してたら鳴らす
	if (targetX != m_menuX || targetY != m_menuY)
	{
		SoundManager::PlaySE("Menu_Select", 1.0f, false);
	}

	// カーソル位置更新
	m_menuX = targetX;
	m_menuY = targetY;

	// メニュー選択用
	int selectIndex = m_menuX + m_menuY * 2;
	for (int i = 0; i < 4; ++i)
	{
		if (selectIndex == i)
		{
			m_buttons[i]->LoadTexture("Assets/Textures/GameUI/menuselect.png", false);
		}
		else
		{
			m_buttons[i]->LoadTexture("Assets/Textures/GameUI/menub.png", false);
		}
	}

	// 決定
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
	{
		SelectEnter();
	}
}

void GuideUIMeneController::UpdateClose()
{
	//メニューを閉じる
	m_closeValue += 0.3f;

	//Pop:少し拡大
	if (m_closePhase == ClosePhase::Pop)
	{
		Vector3 scale;
		scale.x = Easing::OutSine(m_closeValue, 5.0f / 2.0f, m_closeStartScale.x * 1.15f, m_closeStartScale.x);
		scale.y = Easing::OutSine(m_closeValue, 5.0f / 2.0f, m_closeStartScale.y * 1.15f, m_closeStartScale.y);
		scale.z = 1.0f;
		GetTransform()->SetScale(scale);

		if (m_closeValue >= 5.0f / 2.0f)
		{
			m_closePhase = ClosePhase::Shrink;
			m_closeValue = 0.0f;
		}
	}
	//Shrink:縮小
	else if (m_closePhase == ClosePhase::Shrink)
	{
		Vector3 scale;
		scale.x = Easing::OutSine(m_closeValue, 5.0f, 0.0f, m_closeStartScale.x * 1.15f);
		scale.y = Easing::OutSine(m_closeValue, 5.0f, 0.0f, m_closeStartScale.y * 1.15f);
		scale.z = 1.0f;
		GetTransform()->SetScale(scale);

		if (scale.x <= 0.1f)
		{
			GetTransform()->SetScale(0.0f, 0.0f, 0.0f);
			m_menuState = MenuState::DEFAULT;
			m_closePhase = ClosePhase::None;

			// カーソル位置初期化
			m_menuX = 0;
			m_menuY = 0;

			// カーソル反映
			int selectIndex = m_menuX + m_menuY * 2;
			for (int i = 0; i < 4; ++i)
			{
				if (selectIndex == i)
				{
					m_buttons[i]->LoadTexture("Assets/Textures/GameUI/menuselect.png", false);
				}
				else
				{
					m_buttons[i]->LoadTexture("Assets/Textures/GameUI/menub.png", false);
				}
			}
		}
	}
}

void GuideUIMeneController::UpdateChangeScene()
{
	if (Fade::IsActive())
		return;

	// リスタート
	if (m_menuX == 0 && m_menuY == 0)
	{
		std::string levelName = GameState::GetInstance()->GetLevelName();
		SceneManager::ChangeScene(std::make_unique<GameScene>(levelName));
	}
	if (m_menuX == 0 && m_menuY == 1)
	{
		SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
	}
}

void GuideUIMeneController::UpdateManual()
{
	//メニュー開いてるとき
	//位置追従
	m_back->GetTransform()->SetPosition(
		m_frame->GetTransform()->GetPosition()
	);

	//背景スクロール
	m_bgScrollY -= m_bgScrollSpeed;

	//ループ処理
	while (m_bgScrollY >= 1.0f)
	{
		m_bgScrollY -= 1.0f;
	}
	while (m_bgScrollY < 0.0f)
	{
		m_bgScrollY += 1.0f;
	}

	//UVオフセットでスクロール
	m_back->SetUVOffsetPos(0.0f, m_bgScrollY);

	float scale = 0.0f;
	float alpha = 0.0f;

	switch (m_manualState)
	{
	case ManualState::OPEN:
		// 大きくする
		m_time += Time::GetDeltaTime();

		if (m_time > EASING_TIME)
		{
			m_time = EASING_TIME;
			m_manualState = ManualState::DISPLAY;
		}

		scale = Easing::OutSine(m_time, EASING_TIME,MANUAL_SCALE, MENU_SCALE);
		alpha = Easing::OutSine(m_time, EASING_TIME, 1.0f, 0.0f);
		GetTransform()->SetScale(scale, scale, 0.0f);
		m_manual->SetColor(1.0f,1.0f,1.0f,alpha);

		break;

	case ManualState::DISPLAY:
		//メニューを閉じるボタンかメニューのキャンセルボタン押したとき
		if (InputManager::CurrentInputSystem().GetButtonDown("MenuBack"_hash) || InputManager::CurrentInputSystem().GetButtonDown("MenuClose"_hash)
			|| InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
		{
			m_manualState = ManualState::CLOSE;
			m_time = 0.0f;
			SoundManager::PlaySE("Menu_Close", 1.0f, false);
		}

		break;

	case ManualState::CLOSE:
		// 小さくする
		m_time += Time::GetDeltaTime();

		if (m_time > EASING_TIME)
		{
			m_time = EASING_TIME;
			m_manualState = ManualState::OPEN;
			m_menuState = MenuState::SELECT;
			for (int i = 0; i < 4; ++i)
			{
				m_buttons[i]->SetEnabled(true);
			}
			m_manual->SetEnabled(false);
			m_midashi->SetEnabled(true);
		}

		scale = Easing::OutSine(m_time, EASING_TIME, MENU_SCALE, MANUAL_SCALE);
		GetTransform()->SetScale(scale, scale, 0.0f);
		alpha = Easing::OutSine(m_time, EASING_TIME, 0.0f, 1.0f);
		m_manual->SetColor(1.0f, 1.0f, 1.0f, alpha);

		break;
	}
}

void GuideUIMeneController::SelectEnter()
{
	// リスタート
	if (m_menuX == 0 && m_menuY == 0)
	{
		SoundManager::StopBGM();
		SoundManager::PlaySE("Menu_Decision", 1.0f, false);
		m_menuState = MenuState::SCENE_CHANGE;
		Fade::StartIrisOut();
	}
	// 操作説明
	if (m_menuX == 1 && m_menuY == 0)
	{
		m_menuState = MenuState::MANUAL;
		for (int i = 0; i < 4; ++i)
		{
			m_buttons[i]->SetEnabled(false);
		}
		m_time = 0.0f;
		m_midashi->SetEnabled(false);
		m_manual->SetEnabled(true);
		SoundManager::PlaySE("Menu_Decision", 1.0f, false);
	}
	// ステージセレクト
	if (m_menuX == 0 && m_menuY == 1)
	{
		SoundManager::StopBGM();
		SoundManager::PlaySE("Menu_Decision", 1.0f, false);
		m_menuState = MenuState::SCENE_CHANGE;
		Fade::StartIconIrisOut();
	}
	// メニューを閉じる
	if (m_menuX == 1 && m_menuY == 1)
	{
		SoundManager::PlaySE("Menu_Close", 1.0f, false);
		GameState::GetInstance()->StartTimer();
		m_closeStartScale = GetTransform()->GetScale();
		m_menuState = MenuState::CLOSE;
		m_closePhase = ClosePhase::Pop;
		m_closeValue = 0.0f;
		m_value1 = 0.0f;
		InputManager::ChangeBindType(InputBindType::GAMEPLAY);
	}
}
