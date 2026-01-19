//GuideUIMenuController.cpp
#include "GuideUIMenuController.h"
#include "Easing.h"
#include "GridField.h"
#include "GameState.h"
#include "InputManager.h"
#include "GameScene.h"
#include "StageSelectScene.h"
#include "Fade.h"

constexpr float BUTTON_SIZE = 1000.0f;//メニューボタンのサイズ
constexpr float MAX_MENU = 5000.0f;

GuideUIMeneController::GuideUIMeneController():
	m_menuState(MenuState::DEFAULT)
{
}

void GuideUIMeneController::Start()
{
	GetTransform()->SetScale(0.0f, 0.0f, 0.0f);

	//メニュー背景
	auto back = GetGameObject()->AddComponent<SpriteRenderer>();
	back->SetUI(true);
	back->LoadTexture("Assets/Textures/menuhaikei.png");
	back->SetOffsetPos(0.0f, 0.0f);
	back->SetUVScale(1.0f, 1.0f);
	back->SetSize(MAX_MENU / 2.0f);
	m_back = back;

	//メニュー画面
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/menuban.png");
	renderer->SetSize(MAX_MENU);
	m_frame = renderer;

	//メニュー画面ボタン
	Vector2 pos[4] = { {-5.5f, 1.8f}, {5.5f, 1.8f}, {-5.5f, -2.8f}, {5.5f, -2.8f} };
	Vector2 uvPos[4] = { {0.0f, 0.0f}, {0.5f, 0.0f}, {0.0f, 0.5f}, {0.5f, 0.5f} };
	for (int i = 0; i < 4; ++i)
	{
		auto button = GetGameObject()->AddComponent<SpriteRenderer>();
		button->SetUI(true);
		button->LoadTexture("Assets/Textures/menub.png");
		button->SetOffsetPos(pos[i]);
		button->SetSize(BUTTON_SIZE);
		button->SetUVOffsetPos(uvPos[i]);
		button->SetUVScale(0.5f, 0.5f);
		m_buttons[i] = button;
	}

	m_defaultPosition = GetTransform()->GetPosition();
	m_defaultScale = GetTransform()->GetScale();
	m_targetScale = Vector3(0.3f, 0.3f, 0.3f);
	m_closePhase = ClosePhase::None;
	m_closeValue = 0.0f;
	m_closeStartScale = Vector3::zero;
	m_menuX = 0;
	m_menuY = 0;

	m_bgScrollY = 0.0f;
	m_bgScrollSpeed = 0.001f;

	// カーソル反映
	int selectIndex = m_menuX + m_menuY * 2;
	for (int i = 0; i < 4; ++i)
	{
		if (selectIndex == i)
		{
			m_buttons[i]->LoadTexture("Assets/Textures/menuselect.png", false);
		}
		else
		{
			m_buttons[i]->LoadTexture("Assets/Textures/menub.png", false);
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

	case GuideUIMeneController::OPEN:
		UpdateOpen();
		break;

	case GuideUIMeneController::SELECT:
		UpdateSelect();
		break;

	case GuideUIMeneController::CLOSE:
		UpdateClose();
		break;
	
	case GuideUIMeneController::SCENE_CHANGE:
		UpdateChangeScene();
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
			m_menuState = MenuState::OPEN;
			//ボタン受付をUIタイプに変更
			InputManager::ChangeBindType(InputBindType::UI);
		}
	}
}

void GuideUIMeneController::UpdateOpen()
{
	//メニュー出現
	m_value1 += EASING * 2.0f;
	Vector3 scale;
	scale.x = Easing::InSine(m_value1, EASING_MAX, m_targetScale.x, 0.0f);
	scale.y = Easing::InSine(m_value1, EASING_MAX, m_targetScale.y, 0.0f);
	scale.z = 1.0f;
	GetTransform()->SetScale(scale);

	if (m_value1 >= EASING_MAX)
	{
		m_value1 = EASING_MAX;
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
		m_closeStartScale = GetTransform()->GetScale();
		m_menuState = MenuState::CLOSE;
		m_closePhase = ClosePhase::Pop;
		m_closeValue = 0.0f;
		m_value1 = 0.0f;
		InputManager::ChangeBindType(InputBindType::GAMEPLAY);
	}

	Vector2 input = Input::GetLeftStick();
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuUp"_hash) || input.y > 0.0f)
	{
		m_menuY = 0;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuDown"_hash) || input.y < 0.0f)
	{
		m_menuY = 1;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuLeft"_hash) || input.x < 0.0f)
	{
		m_menuX = 0;
	}
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuRight"_hash) || input.x > 0.0f)
	{
		m_menuX = 1;
	}

	// メニュー選択用
	int selectIndex = m_menuX + m_menuY * 2;
	for (int i = 0; i < 4; ++i)
	{
		if (selectIndex == i)
		{
			m_buttons[i]->LoadTexture("Assets/Textures/menuselect.png", false);
		}
		else
		{
			m_buttons[i]->LoadTexture("Assets/Textures/menub.png", false);
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
	m_closeValue += EASING * 3.0f;

	//Pop:少し拡大
	if (m_closePhase == ClosePhase::Pop)
	{
		Vector3 scale;
		scale.x = Easing::OutSine(m_closeValue, EASING_MAX / 2.0f, m_closeStartScale.x * 1.15f, m_closeStartScale.x);
		scale.y = Easing::OutSine(m_closeValue, EASING_MAX / 2.0f, m_closeStartScale.y * 1.15f, m_closeStartScale.y);
		scale.z = 1.0f;
		GetTransform()->SetScale(scale);

		if (m_closeValue >= EASING_MAX / 2.0f)
		{
			m_closePhase = ClosePhase::Shrink;
			m_closeValue = 0.0f;
		}
	}
	//Shrink:縮小
	else if (m_closePhase == ClosePhase::Shrink)
	{
		Vector3 scale;
		scale.x = Easing::OutSine(m_closeValue, EASING_MAX, 0.0f, m_closeStartScale.x * 1.15f);
		scale.y = Easing::OutSine(m_closeValue, EASING_MAX, 0.0f, m_closeStartScale.y * 1.15f);
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
					m_buttons[i]->LoadTexture("Assets/Textures/menuselect.png", false);
				}
				else
				{
					m_buttons[i]->LoadTexture("Assets/Textures/menub.png", false);
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

void GuideUIMeneController::SelectEnter()
{
	// リスタート
	if (m_menuX == 0 && m_menuY == 0)
	{
		m_menuState = MenuState::SCENE_CHANGE;
		Fade::StartIrisOut();
	}
	// 操作説明
	if (m_menuX == 1 && m_menuY == 0)
	{

	}
	// ステージセレクト
	if (m_menuX == 0 && m_menuY == 1)
	{
		m_menuState = MenuState::SCENE_CHANGE;
		Fade::StartIrisOut();
	}
	// メニューを閉じる
	if (m_menuX == 1 && m_menuY == 1)
	{
		m_closeStartScale = GetTransform()->GetScale();
		m_menuState = MenuState::CLOSE;
		m_closePhase = ClosePhase::Pop;
		m_closeValue = 0.0f;
		m_value1 = 0.0f;
		InputManager::ChangeBindType(InputBindType::GAMEPLAY);
	}
}
