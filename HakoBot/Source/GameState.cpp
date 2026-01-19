#include "GameState.h"
#include "InputManager.h"
#include <GameFrame/Time.h>

GameState* GameState::s_pInstance = nullptr;
int GameState::s_currentStageNo = 0;


GameState::GameState() :
	m_isBlockTransparent(false),
	m_colorBalanceMode(false),
	m_isClearEnter(false)
{
	if (s_pInstance == nullptr) {

		s_pInstance = this;
	}
}

GameState::~GameState()
{
	if (s_pInstance == this) {

		s_pInstance = nullptr;
	}
}

void GameState::Start()
{

}

void GameState::Update()
{
	m_isBlockTransparent = InputManager::CurrentInputSystem().GetButtonHold("ChangeBlockTransparency"_hash);

	if (IsClear() && InputManager::CurrentInputSystem().GetButtonDown("Clear"_hash))
		m_isClearEnter = true;

#ifdef _DEBUG
	float deltaTime = Time::GetDeltaTime();
	int fps = 1.0f / deltaTime;

	ImGui::Begin("Guide");
	ImGui::Text("FPS : %3d", fps);
	if (!m_colorBalanceMode)
	{
		if (ImGui::Button("ColorBalanceMode"))
		{
			Vector3 pos = Camera::GetMain()->GetTransform()->GetPosition();
			Quaternion rotation = Camera::GetMain()->GetTransform()->GetQuaternion();
			Camera::GetMain()->GetTransform()->SetParent((Transform*)nullptr);
			Camera::GetMain()->GetTransform()->SetPosition(pos);
			Camera::GetMain()->GetTransform()->SetQuaternion(rotation);
			m_colorBalanceMode = true;
		}
	}
	else
	{
		// ライト色取得
		DirectionalLight* light = DirectionalLight::GetMain();
		Color lightColor = light->GetLightColor();
		Color ambientColor = light->GetAmbientColor();
		float lightF[3];
		float ambientF[3];
		lightF[0] = lightColor.r;
		lightF[1] = lightColor.g;
		lightF[2] = lightColor.b;
		ambientF[0] = ambientColor.r;
		ambientF[1] = ambientColor.g;
		ambientF[2] = ambientColor.b;

		ImGui::DragFloat3("LightColor", lightF, 0.003f, 0.0f, 1.0f);
		ImGui::DragFloat3("AmbientColor", ambientF, 0.003f, 0.0f, 1.0f);
		
		if (ImGui::Button("Reset"))
		{
			lightF[0] = 1.0f;
			lightF[1] = 1.0f;
			lightF[2] = 1.0f;
			ambientF[0] = 0.5f;
			ambientF[1] = 0.5f;
			ambientF[2] = 0.5f;
		}

		// ライト色設定
		lightColor.r = lightF[0];
		lightColor.g = lightF[1];
		lightColor.b = lightF[2];
		ambientColor.r = ambientF[0];
		ambientColor.g = ambientF[1];
		ambientColor.b = ambientF[2];
		light->SetLightColor(lightColor);
		light->SetAmbientColor(ambientColor);
	}

	ImGui::End();

#endif // DEBUG
}

void GameState::SetLevelName(const std::string& levelName)
{
	m_levelName = levelName;
}

std::string GameState::GetLevelName()
{
	return m_levelName;
}


void GameState::SetGridField(GridField* pGridField)
{
	m_pGridField = pGridField;
}

void GameState::AppendWorldBlock(BlockObject* pBlockObject)
{
	auto it = std::find(m_pWorldBlocks.begin(), m_pWorldBlocks.end(), pBlockObject);

	//すでにブロックが追加されていた場合以降処理しない
	if (it != m_pWorldBlocks.end()) return;

	m_pWorldBlocks.push_back(pBlockObject);
}

void GameState::RemoveWorldBlock(BlockObject* pBlockObject)
{
	std::erase_if(m_pWorldBlocks, [pBlockObject](auto x) {
		return x == pBlockObject;
		});

}

bool GameState::IsBlockTransparent()
{
	return m_isBlockTransparent;
}

bool GameState::IsClear()
{
	GridField* gridField = GetGridField();
	if (!gridField)
	{
		return false;
	}
	
	Player* player = GetPlayer();
	if (!player)
	{
		return false;
	}

	bool isClear = gridField->IsClear();

	isClear &= !player->GetBlockHandler()->HasBlock();

	isClear &= m_pWorldBlocks.empty();

	return isClear;
}

bool GameState::IsClearEnter()
{
	return m_isClearEnter;
}

