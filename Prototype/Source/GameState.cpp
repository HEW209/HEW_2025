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
	m_pWorldBlocks.push_back(pBlockObject);
}

