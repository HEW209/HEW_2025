//ResultController.cpp
#include "ResultController.h"
#include "Easing.h"
#include "StageSelectScene.h"
#include "GameScene.h"
#include "Fade.h"
#include "InputManager.h"
#include "GameState.h"
#include "SaveData.h"
#include "StageNumber.h"
#include "SoundManager.h"

constexpr float EASE_TOTAL_TIME = 0.7f;

//黒
constexpr float KURO_SIZE = 1350.0f;
constexpr float KURO_POS_X_END = 0.0f;
constexpr float KURO_POS_Y_END = 3.9f;
constexpr float KURO_POS_X_START = KURO_POS_X_END;
constexpr float KURO_POS_Y_START = KURO_POS_Y_END + 5.0f;

//リザルト
constexpr float RESULT_SIZE = 640.0f;
constexpr float RESULT_POS_X_END = 3.4f;
constexpr float RESULT_POS_Y_END = 2.4f;
constexpr float RESULT_POS_X_START = RESULT_POS_X_END + 8.0f;
constexpr float RESULT_POS_Y_START = RESULT_POS_Y_END;

//リザルトイラスト
constexpr float ILLUST_SIZE = 1400.0f;
constexpr float ILLUST_POS_X_END = -2.0f;
constexpr float ILLUST_POS_Y_END = 0.1f;
constexpr float ILLUST_POS_X_START = ILLUST_POS_X_END + 15.0f;
constexpr float ILLUST_POS_Y_START = ILLUST_POS_Y_END;

// 選択肢
constexpr float SELECT_SIZE_MAX = 550.0f;
constexpr float SELECT_SIZE_MIN = 450.0f;
constexpr float SELECT_POS_X_END = 4.0f;
constexpr float SELECT_POS_Y_END = -0.7f;
constexpr float SELECT_POS_X_START = SELECT_POS_X_END + 8.0f;
constexpr float SELECT_POS_Y_START = SELECT_POS_Y_END;
constexpr float SELECT_SPACE_RATIO = 0.5f * 0.45f;
constexpr float SELECT_SCALING_SPEED = 800.0f;

static const char* g_activeText[3] = {
		"Assets/Textures/Result/tugi_stage.png",
		"Assets/Textures/Result/stage_select.png",
		"Assets/Textures/Result/re_start.png"
};

static const char* g_defaultText[3] = {
	"Assets/Textures/Result/tugi_stageoff.png",
	"Assets/Textures/Result/stage_selectoff.png",
	"Assets/Textures/Result/re_startoff.png"
};

////次のステージ
//constexpr float TUGI_SIZE = 450.0f;
//constexpr float TUGI_POS_X_END = 4.0f;
//constexpr float TUGI_POS_Y_END = -1.6f;
//constexpr float TUGI_POS_X_START = TUGI_POS_X_END + 8.0f;
//constexpr float TUGI_POS_Y_START = TUGI_POS_Y_END;
//
////ステージセレクト
//constexpr float STAGE_SIZE = 450.0f;
//constexpr float STAGE_POS_X_END = 4.0f;
//constexpr float STAGE_POS_Y_END = -2.4f;
//constexpr float STAGE_POS_X_START = STAGE_POS_X_END + 8.0f;
//constexpr float STAGE_POS_Y_START = STAGE_POS_Y_END;
//
////リスタート
//constexpr float RE_SIZE = 450.0f;
//constexpr float RE_POS_X_END = 4.0f;
//constexpr float RE_POS_Y_END = -3.2f;
//constexpr float RE_POS_X_START = RE_POS_X_END + 8.0f;
//constexpr float RE_POS_Y_START = RE_POS_X_START;

ResultController::ResultController():
	m_state(ResultState::MOVE), m_currentSelect(0), m_resultTime(0.0f), m_isStartedBGMLoop(false)
{

}

void ResultController::Start()
{
	SoundManager::PlayBGM("ResultStart", 1.0f, false);

	if (GameState::GetInstance()->GetCurrentStegaNo() >= StageCount)
	{
		m_currentSelect = Select::STAGE_SELECT;
	}

	//フェード
	auto fade = GetGameObject()->AddComponent<SpriteRenderer>();
	fade->SetUI(true);
	fade->SetSize(1280, 720);
	fade->SetColor(0.0f, 0.0f, 0.0f, 0.4f);
	m_screenFade = fade;

	//リザルトイラスト
	auto illust = GetGameObject()->AddComponent<SpriteRenderer>();
	illust->LoadTexture("Assets/Textures/Result/result_illust.png");
	illust->SetUI(true);
	illust->SetSize(ILLUST_SIZE);
	illust->SetOffsetPos(ILLUST_POS_X_START, ILLUST_POS_Y_START);
	m_illust = illust;

	//リザルトのときでる黒(下)
	auto kuroDownRenderer = GetGameObject()->AddComponent<SpriteRenderer>();
	kuroDownRenderer->LoadTexture("Assets/Textures/Result/sitakuro.png");
	kuroDownRenderer->SetUI(true);
	kuroDownRenderer->SetSize(KURO_SIZE);
	kuroDownRenderer->SetOffsetPos(-KURO_POS_X_START, -KURO_POS_Y_START);
	kuroDownRenderer->GetMaterial()->SetSamplerState(SamplerState::LINEAR_CLAMP);
	m_kuroDown = kuroDownRenderer;

	//リザルトのときでる黒(上)
	auto kuroUpRenderer = GetGameObject()->AddComponent<SpriteRenderer>();
	kuroUpRenderer->LoadTexture("Assets/Textures/Result/uekuro.png");
	kuroUpRenderer->SetUI(true);
	kuroUpRenderer->SetSize(KURO_SIZE);
	kuroUpRenderer->SetOffsetPos(KURO_POS_X_START, KURO_POS_Y_START);
	kuroUpRenderer->GetMaterial()->SetSamplerState(SamplerState::LINEAR_CLAMP);
	m_kuroUp = kuroUpRenderer;

	//リザルト
	auto resultMozi = GetGameObject()->AddComponent<SpriteRenderer>();
	resultMozi->LoadTexture("Assets/Textures/Result/rizaruto4.png");
	resultMozi->SetUI(true);
	resultMozi->SetSize(RESULT_SIZE);
	resultMozi->SetOffsetPos(RESULT_POS_X_START, RESULT_POS_Y_START);
	m_result = resultMozi;

	//選択肢
	Vector2 offsetPos(SELECT_POS_X_START, SELECT_POS_Y_START);
	for (int i = 0; i < Select::COUNT; ++i)
	{
		auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
		if (i == m_currentSelect)
		{
			renderer->LoadTexture(g_activeText[i]);
			renderer->SetSize(SELECT_SIZE_MAX);
		}
		else
		{
			renderer->LoadTexture(g_defaultText[i]);
			renderer->SetSize(SELECT_SIZE_MIN);
		}
		// オフセットをずらす
		offsetPos.y -= renderer->GetSize().y * SELECT_SPACE_RATIO * 0.01f;
		renderer->SetOffsetPos(offsetPos);
		renderer->SetUI(true);

		// オフセットをずらす
		offsetPos.y -= renderer->GetSize().y * SELECT_SPACE_RATIO * 0.01f;

		m_selectText[i] = renderer;
	}

	// ラストステージ用
	if (GameState::GetInstance()->GetCurrentStegaNo() >= StageCount)
	{
		m_selectText[Select::NEXT]->SetEnabled(false);
	}
}

void ResultController::Update()
{
	switch (m_state)
	{
	case ResultController::ResultState::MOVE:
		MoveUpdate();
		break;
	case ResultController::ResultState::SELECT:
		SelectUpdate();
		break;
	case ResultController::ResultState::END:
		EndUpdate();
		break;
	default:
		break;
	}
	
	if (!m_isStartedBGMLoop && !SoundManager::IsBGMPlaying())
	{
		SoundManager::PlayBGM("ResultLoop", 1.0f, true);
		m_isStartedBGMLoop = true;
	}
}

void ResultController::MoveUpdate()
{
	//時間加算
	m_resultTime += Time::GetDeltaTime();
	if (m_resultTime > EASE_TOTAL_TIME)
	{
		m_resultTime = EASE_TOTAL_TIME;
		m_state = ResultState::SELECT;
	}


	//リザルトイラストの移動
	m_illust->SetOffsetPos(
		Easing::OutQuart(m_resultTime, EASE_TOTAL_TIME, ILLUST_POS_X_END, ILLUST_POS_X_START),
		Easing::OutQuart(m_resultTime, EASE_TOTAL_TIME, ILLUST_POS_Y_END, ILLUST_POS_Y_START));

	//黒下の移動
	m_kuroDown->SetOffsetPos(
		Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, -KURO_POS_X_END, -KURO_POS_X_START),
		Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, -KURO_POS_Y_END, -KURO_POS_Y_START));

	//黒上の移動
	m_kuroUp->SetOffsetPos(
		Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, KURO_POS_X_END, KURO_POS_X_START),
		Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, KURO_POS_Y_END, KURO_POS_Y_START));

	//リザルトの移動
	m_result->SetOffsetPos(
		Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, RESULT_POS_X_END, RESULT_POS_X_START),
		Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, RESULT_POS_Y_END, RESULT_POS_Y_START));

	// 選択肢の移動
	for (int i = 0; i < Select::COUNT; ++i)
	{
		Vector2 offset = m_selectText[i]->GetOffsetPos();
		offset.x = Easing::OutCubic(m_resultTime, EASE_TOTAL_TIME, SELECT_POS_X_END, SELECT_POS_X_START);
		m_selectText[i]->SetOffsetPos(offset);
	}
}

void ResultController::SelectUpdate()
{
	bool isUp = false;

	//上選択
	if ((Input::GetLeftStick().y > 0.0f && Input::GetLastLeftStick().y <= 0.0f) || 
		Input::GetKeyDown(KeyCode::UP) || Input::GetKeyDown(KeyCode::W))
	{
		SoundManager::PlaySE("Result_Select", 1.0f, false);
		isUp = true;
		m_currentSelect--;
		if (m_currentSelect < 0)
		{
			m_currentSelect += Select::COUNT;
		}
	}

	//下選択
	if ((Input::GetLeftStick().y < 0.0f && Input::GetLastLeftStick().y >= 0.0f) ||
		Input::GetKeyDown(KeyCode::DOWN) || Input::GetKeyDown(KeyCode::S))
	{
		SoundManager::PlaySE("Result_Select", 1.0f, false);
		isUp = false;
		m_currentSelect++;
		if (m_currentSelect >= Select::COUNT)
		{
			m_currentSelect -= Select::COUNT;
		}
	}

	// 次のステージを選択できなくする
	if (GameState::GetInstance()->GetCurrentStegaNo() >= StageCount &&
		m_currentSelect == Select::NEXT)
	{
		if (isUp)
		{
			m_currentSelect--;
			if (m_currentSelect < 0)
			{
				m_currentSelect += Select::COUNT;
			}
		}
		else
		{
			m_currentSelect++;
			if (m_currentSelect >= Select::COUNT)
			{
				m_currentSelect -= Select::COUNT;
			}
		}
	}

	// 選択肢更新
	Vector2 offsetPos(SELECT_POS_X_END, SELECT_POS_Y_END);
	for (int i = 0; i < Select::COUNT; ++i)
	{
		if (m_currentSelect == i)
		{
			m_selectText[i]->LoadTexture(g_activeText[i], false);
			m_selectText[i]->SetSize(SELECT_SIZE_MAX);
		}
		else
		{
			m_selectText[i]->LoadTexture(g_defaultText[i], false);
			m_selectText[i]->SetSize(SELECT_SIZE_MIN);
		}

		// オフセットをずらす
		offsetPos.y -= m_selectText[i]->GetSize().y * SELECT_SPACE_RATIO * 0.01f;
		m_selectText[i]->SetOffsetPos(offsetPos);
		m_selectText[i]->SetUI(true);

		// オフセットをずらす
		offsetPos.y -= m_selectText[i]->GetSize().y * SELECT_SPACE_RATIO * 0.01f;
	}

	//Enter B
	if (InputManager::CurrentInputSystem().GetButtonDown("ResultSelect"_hash))
	{
		SoundManager::StopBGM();
		SoundManager::PlaySE("Result_Decision", 1.0f, false);
		m_state = ResultState::END;
		switch (m_currentSelect)
		{
		case 0:
		case 2:
			Fade::StartIrisOut();//フェード
			break;
		case 1:
			Fade::StartIconIrisOut();//フェード
			break;
		}
	}

}

void ResultController::EndUpdate()
{
	if (Fade::IsActive()) return;

	int stageNo = GameState::GetCurrentStegaNo();
	if (stageNo > SaveData::GetClearLevel())
	{
		SaveData::SetClearLevel(stageNo);
		SaveData::Save();
	}

	//次のステージへ
	if (m_currentSelect == 0)
	{
		int stageNumber = GameState::GetCurrentStegaNo() + 1;
		std::string StageIDstr = std::to_string(stageNumber);
		std::string path = "Level" + StageIDstr + "";

		GameState::SetCurrentStegaNo(stageNumber);
		SceneManager::ChangeScene(std::make_unique<GameScene>(path));//ステージ読み込むやつ
	}
	//ステージセレクトへ
	else if (m_currentSelect == 1)
	{
		SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
	}
	//リスタート
	else
	{
		int stageNumber = GameState::GetCurrentStegaNo();
		std::string StageIDstr = std::to_string(stageNumber);
		std::string path = "Level" + StageIDstr + "";

		GameState::SetCurrentStegaNo(stageNumber);
		SceneManager::ChangeScene(std::make_unique<GameScene>(path));
	}
}
