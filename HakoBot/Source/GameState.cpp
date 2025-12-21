#include "GameState.h"
#include <GameFrame/Time.h>

GameState* GameState::s_pInstance = nullptr;


GameState::GameState()
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
#ifdef _DEBUG
	float deltaTime = Time::GetDeltaTime();
	int fps = 1.0f / deltaTime;

	ImGui::Begin("Guide");
	ImGui::Text("FPS : %d", fps);
	ImGui::End();
#endif // DEBUG
}


void GameState::SetGridField(GridField* pGridField)
{
	m_pGridField = pGridField;
}

void GameState::AppendWorldBlock(BlockObject* pBlockObject)
{
	auto it = std::find(m_pWorldBlocks.begin(), m_pWorldBlocks.end(), pBlockObject);

	//Ç∑Ç≈Ç…ÉuÉçÉbÉNÇ™í«â¡Ç≥ÇÍÇƒÇ¢ÇΩèÍçáà»ç~èàóùÇµÇ»Ç¢
	if (it != m_pWorldBlocks.end()) return;

	m_pWorldBlocks.push_back(pBlockObject);
}

void GameState::RemoveWorldBlock(BlockObject* pBlockObject)
{
	std::erase_if(m_pWorldBlocks, [pBlockObject](auto x) {
		return x == pBlockObject;
		});

}

