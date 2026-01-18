#include "GameState.h"
#include "InputManager.h"
#include <GameFrame/Time.h>

GameState* GameState::s_pInstance = nullptr;
int GameState::s_currentStageNo = 0;


GameState::GameState() :
	m_isBlockTransparent(false)
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

#ifdef _DEBUG
	float deltaTime = Time::GetDeltaTime();
	int fps = 1.0f / deltaTime;

	ImGui::Begin("Guide");
	ImGui::Text("FPS : %3d", fps);
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

	//‚·‚Å‚ÉƒuƒƒbƒN‚ª’Ç‰Á‚³‚ê‚Ä‚¢‚½ê‡ˆÈ~ˆ—‚µ‚È‚¢
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

