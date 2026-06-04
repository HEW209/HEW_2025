#include "ColliderDebug.h"
#include <GameFrame/ColliderSystem.h>
#include <GameFrame/ColliderSystem.h>

ColliderDebug::ColliderDebug()
{
	m_IsDraw = false;
#ifdef _DEBUG
	ColliderSystem::Instance().SetDrawFlag(m_IsDraw);
#endif // _DEBUG
}

void ColliderDebug::Update()
{
	if (Input::GetKeyDown(KeyCode::C))
	{
		if (m_IsDraw)	// âüÇµÇΩÇÁêÿÇËë÷Ç¶	
		{
			m_IsDraw = false;
		}
		else
		{
			m_IsDraw = true;
		}
#ifdef _DEBUG
		ColliderSystem::Instance().SetDrawFlag(m_IsDraw);
#endif // _DEBUG
	}
}
