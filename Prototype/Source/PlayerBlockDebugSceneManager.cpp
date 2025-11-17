#include "PlayerBlockDebugSceneManager.h"

PlayerBlockDebugSceneManager* PlayerBlockDebugSceneManager::s_pInstance = nullptr;


PlayerBlockDebugSceneManager::PlayerBlockDebugSceneManager()
{
	if (s_pInstance == nullptr) {

		s_pInstance = this;
	}
}

PlayerBlockDebugSceneManager::~PlayerBlockDebugSceneManager()
{
	if (s_pInstance == this) {

		s_pInstance = nullptr;
	}
}

void PlayerBlockDebugSceneManager::Start()
{

}

void PlayerBlockDebugSceneManager::Update()
{
	
	

}


void PlayerBlockDebugSceneManager::SetGridField(GridField* pGridField)
{
	m_pGridField = pGridField;
}

