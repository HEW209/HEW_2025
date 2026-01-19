//ResultController.cpp
#include "ResultController.h"
#include "Easing.h"
#include "StageSelectScene.h"
#include "GameScene.h"
#include "Fade.h"
#include "InputManager.h"
#include "GameState.h"
#include "SaveData.h"


//黒
constexpr float KURO_SIZE = 1350.0f;
constexpr float KURO_POS_X_END = 0.0f;
constexpr float KURO_POS_Y_END = 3.9f;
constexpr float KURO_POS_X_START = 0.0f;
constexpr float KURO_POS_Y_START = 7.0f;

////リザルト　右バージョン
//constexpr float RESULT_SIZE = 500.0f;
//constexpr float RESULT_POS_X_END = -2.9f;
//constexpr float RESULT_POS_Y_END = 1.4f;
//constexpr float RESULT_POS_X_START = 6.8f;
//constexpr float RESULT_POS_Y_START = 1.4f;

//リザルト 左上バージョン
constexpr float RESULT_SIZE = 510.0f;
constexpr float RESULT_POS_X_END = 1.3f;
constexpr float RESULT_POS_Y_END = 2.45f;
constexpr float RESULT_POS_X_START = -5.8f;
constexpr float RESULT_POS_Y_START = 2.45f;

//次のステージ
constexpr float TUGI_SIZE = 450.0f;
constexpr float TUGI_POS_X_END = -1.3f;
constexpr float TUGI_POS_Y_END = -0.4f;
constexpr float TUGI_POS_X_START = 6.8f;
constexpr float TUGI_POS_Y_START = -0.4f;

//ステージセレクト
constexpr float STAGE_SIZE = 450.0f;
constexpr float STAGE_POS_X_END = -1.3f;
constexpr float STAGE_POS_Y_END = -1.4f;
constexpr float STAGE_POS_X_START = 6.8f;
constexpr float STAGE_POS_Y_START = -1.4f;

//リスタート
constexpr float RE_SIZE = 450.0f;
constexpr float RE_POS_X_END = -1.2f;
constexpr float RE_POS_Y_END = -2.4f;
constexpr float RE_POS_X_START = 6.8f;
constexpr float RE_POS_Y_START = -2.4f;

//リザルトイラスト
constexpr float ILLUST_SIZE = 900.0f;
constexpr float ILLUST_POS_X_END = -3.7f;
constexpr float ILLUST_POS_Y_END = -1.1f;
constexpr float ILLUST_POS_X_START = -3.7f;
constexpr float ILLUST_POS_Y_START = -5.2f;


ResultController::ResultController():
	m_state(ResultState::MOVE), m_currentSelect(0), m_resultTime(0.0f)
{

}

void ResultController::Start()
{
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

	//次のステージ
	auto tugiMozi = GetGameObject()->AddComponent<SpriteRenderer>();
	tugiMozi->LoadTexture("Assets/Textures/Result/tugi_stage.png");
	tugiMozi->SetUI(true);
	tugiMozi->SetSize(TUGI_SIZE);
	tugiMozi->SetOffsetPos(TUGI_POS_X_START, TUGI_POS_Y_START);
	m_selectText[Select::NEXT] = tugiMozi;

	//ステージセレクト
	auto stageMozi = GetGameObject()->AddComponent<SpriteRenderer>();
	stageMozi->LoadTexture("Assets/Textures/Result/stage_selectoff.png");
	stageMozi->SetUI(true);
	stageMozi->SetSize(STAGE_SIZE);
	stageMozi->SetOffsetPos(STAGE_POS_X_START, STAGE_POS_Y_START);
	m_selectText[Select::STAGE_SELECT] = stageMozi;

	//リスタート
	auto reMozi = GetGameObject()->AddComponent<SpriteRenderer>();
	reMozi->LoadTexture("Assets/Textures/Result/re_startoff.png");
	reMozi->SetUI(true);
	reMozi->SetSize(RE_SIZE);
	reMozi->SetOffsetPos(RE_POS_X_START, RE_POS_Y_START);
	m_selectText[Select::RESTART] = reMozi;

	


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
	
	
}

void ResultController::MoveUpdate()
{
	//時間加算
	m_resultTime += 0.2f;

	
	if (m_resultTime > 5.0f)
	{
		m_resultTime = 5.0f;
		m_state = ResultState::SELECT;
	}

	
	
	////リザルトイラストの移動
	//m_illust->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, ILLUST_POS_X_END, ILLUST_POS_X_START),
	//	Easing::OutBack(m_resultTime, 5.0f, ILLUST_POS_Y_END, ILLUST_POS_Y_END + 1.0f, ILLUST_POS_Y_START));

	//リザルトイラストの移動
	m_illust->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, ILLUST_POS_X_END, ILLUST_POS_X_START),
		Easing::OutBack(m_resultTime, 5.0f, 1.7f, ILLUST_POS_Y_END, ILLUST_POS_Y_START));

	//黒下の移動
	m_kuroDown->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, -KURO_POS_X_END, -KURO_POS_X_START),
		Easing::OutBack(m_resultTime, 5.0f, 1.5f, -KURO_POS_Y_END, -KURO_POS_Y_START));

	//黒上の移動
	m_kuroUp->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, KURO_POS_X_END, KURO_POS_X_START),
		Easing::OutBack(m_resultTime, 5.0f, 1.5f, KURO_POS_Y_END, KURO_POS_Y_START));

	//リザルトの移動
	m_result->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, RESULT_POS_X_END, RESULT_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, RESULT_POS_Y_END, RESULT_POS_Y_START));

	//次のステージの移動
	m_selectText[Select::NEXT]->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, TUGI_POS_X_END, TUGI_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, TUGI_POS_Y_END, TUGI_POS_Y_START));

	//ステージセレクトの移動
	m_selectText[Select::STAGE_SELECT]->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, STAGE_POS_X_END, STAGE_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, STAGE_POS_Y_END, STAGE_POS_Y_START));

	//リスタートの移動
	m_selectText[Select::RESTART]->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, RE_POS_X_END, RE_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, RE_POS_Y_END, RE_POS_Y_START));

}

void ResultController::SelectUpdate()
{
	//上選択
	if ((Input::GetLeftStick().y > 0.0f && Input::GetLastLeftStick().y <= 0.0f) || 
		Input::GetKeyDown(KeyCode::UP) || Input::GetKeyDown(KeyCode::W))
	{
		m_currentSelect --;
		if (m_currentSelect < 0)
		{
			m_currentSelect += Select::COUNT;
		}
	}

	//下選択
	if ((Input::GetLeftStick().y < 0.0f && Input::GetLastLeftStick().y >= 0.0f) ||
		Input::GetKeyDown(KeyCode::DOWN) || Input::GetKeyDown(KeyCode::S))
	{
		m_currentSelect ++;
		if (m_currentSelect >= Select::COUNT)
		{
			m_currentSelect -= Select::COUNT;
		}
	}

	const char* activeText[3] = {
		"Assets/Textures/Result/tugi_stage.png",
		"Assets/Textures/Result/stage_select.png",
		"Assets/Textures/Result/re_start.png" };

	const char* defaultText[3] = {
		"Assets/Textures/Result/tugi_stageoff.png",
		"Assets/Textures/Result/stage_selectoff.png",
		"Assets/Textures/Result/re_startoff.png" };

	for (int i = 0; i < 3; i++)
	{
		if (m_currentSelect == i)
		{
			m_selectText[i]->LoadTexture(activeText[i], false);
		}
		else
		{
			m_selectText[i]->LoadTexture(defaultText[i], false);
		}
	}

	//Enter B
	if (InputManager::CurrentInputSystem().GetButtonDown("ResultSelect"_hash))
	{
		m_state = ResultState::END;
		Fade::StartIrisOut();//フェード
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
