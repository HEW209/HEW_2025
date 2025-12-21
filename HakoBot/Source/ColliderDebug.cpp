#include "ColliderDebug.h"
#include <GameFrame/ColliderSystem.h>
#include <GameFrame/ColliderSystem.h>

ColliderDebug::ColliderDebug()
{
	m_IsDraw = false;
	ColliderSystem::Instance().SetDrawFlag(m_IsDraw);
}

void ColliderDebug::Update()
{
	if (Input::GetKeyDown(KeyCode::C))
	{
		if (m_IsDraw)	// ‰Ÿ‚µ‚½‚çØ‚è‘Ö‚¦	
		{
			m_IsDraw = false;
		}
		else
		{
			m_IsDraw = true;
		}
		ColliderSystem::Instance().SetDrawFlag(m_IsDraw);
	}
}
