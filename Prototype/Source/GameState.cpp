#include "GameState.h"

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

