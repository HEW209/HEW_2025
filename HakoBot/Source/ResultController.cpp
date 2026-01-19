//ResultController.cpp
#include "ResultController.h"
#include "Easing.h"

//黒
constexpr float KURO_SIZE = 1350.0f;
constexpr float KURO_POS_X_END = -9.0f;
constexpr float KURO_POS_Y_END = 0.07f;
constexpr float KURO_POS_X_START = 3.65f;
constexpr float KURO_POS_Y_START = 4.1f;

//リザルト
constexpr float RESULT_SIZE = 500.0f;
constexpr float RESULT_POS_X_END = -2.9f;
constexpr float RESULT_POS_Y_END = 1.4f;
constexpr float RESULT_POS_X_START = 6.8f;
constexpr float RESULT_POS_Y_START = 1.4f;

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
constexpr float ILLUST_POS_Y_START = -5.4f;


void ResultController::Start()
{

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
	m_kuroDown = kuroDownRenderer;

	//リザルトのときでる黒(上)
	auto kuroUpRenderer = GetGameObject()->AddComponent<SpriteRenderer>();
	kuroUpRenderer->LoadTexture("Assets/Textures/Result/uekuro.png");
	kuroUpRenderer->SetUI(true);
	kuroUpRenderer->SetSize(KURO_SIZE);
	kuroUpRenderer->SetOffsetPos(KURO_POS_X_START, KURO_POS_Y_START);
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
	tugiMozi->LoadTexture("Assets/Textures/Result/tugi_stageoff.png");
	tugiMozi->SetUI(true);
	tugiMozi->SetSize(TUGI_SIZE);
	tugiMozi->SetOffsetPos(TUGI_POS_X_START, TUGI_POS_Y_START);
	m_tugi = tugiMozi;

	//ステージセレクト
	auto stageMozi = GetGameObject()->AddComponent<SpriteRenderer>();
	stageMozi->LoadTexture("Assets/Textures/Result/stage_selectoff.png");
	stageMozi->SetUI(true);
	stageMozi->SetSize(STAGE_SIZE);
	stageMozi->SetOffsetPos(STAGE_POS_X_START, STAGE_POS_Y_START);
	m_stage = stageMozi;

	//リスタート
	auto reMozi = GetGameObject()->AddComponent<SpriteRenderer>();
	reMozi->LoadTexture("Assets/Textures/Result/re_startoff.png");
	reMozi->SetUI(true);
	reMozi->SetSize(RE_SIZE);
	reMozi->SetOffsetPos(RE_POS_X_START, RE_POS_Y_START);
	m_re = reMozi;

	

	//時間リセット
	m_resultTime = 0.0f;
	m_resultTimeUse == false;
}

void ResultController::Update()
{
	//時間加算
	if (m_resultTime <= 5.0f && m_resultTimeUse == false)
	{
		m_resultTime += 0.1f;
	}
	else
	{
		m_resultTimeUse = true;
	}
	
	//リザルトイラストの移動
	m_illust->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, ILLUST_POS_X_END, ILLUST_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, ILLUST_POS_Y_END, ILLUST_POS_Y_START));

	//黒下の移動
	m_kuroDown->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, -KURO_POS_X_END, -KURO_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, -KURO_POS_Y_END, -KURO_POS_Y_START));

	//黒上の移動
	m_kuroUp->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, KURO_POS_X_END, KURO_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, KURO_POS_Y_END, KURO_POS_Y_START));

	//リザルトの移動
	m_result->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, RESULT_POS_X_END, RESULT_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, RESULT_POS_Y_END, RESULT_POS_Y_START));

	//次のステージの移動
	m_tugi->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, TUGI_POS_X_END, TUGI_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, TUGI_POS_Y_END, TUGI_POS_Y_START));

	//ステージセレクトの移動
	m_stage->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, STAGE_POS_X_END, STAGE_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, STAGE_POS_Y_END, STAGE_POS_Y_START));

	//リスタートの移動
	m_re->SetOffsetPos(Easing::InSine(m_resultTime, 10.0f, RE_POS_X_END, RE_POS_X_START),
		Easing::InSine(m_resultTime, 5.0f, RE_POS_Y_END, RE_POS_Y_START));

	
}