#include "GameScene.h"
#include "StageSelectObject.h"
#include "LevelSerializer.h"



#include <memory>

#define FONT_SIZE (450.0f)

void StageSelectObject::Start()
{
	switch (m_StageID)
	{

	case 0:
		m_Pos = { 20.0f,-2.0f ,0.0f };
		break;
	case 1:
		m_Pos = { 24.0f,-2.0f ,0.0f };
		break;
	case 28:
		m_Pos = { 11.0f,-2.0f ,0.0f };
		break;
	case 29:
		m_Pos = { 15.0f,-2.0f ,0.0f };
		break;
	case 30:
		m_Pos = { 18.0f,-2.0f ,0.0f };
		break;
	default:
		m_Pos = { 20.0f,-2.0f ,0.0f };
		break;
	}
	
	m_TargetPos = m_Pos;
}


void StageSelectObject::Update()
{

	KeyEnter();
	SetPosID();


}

void StageSelectObject::KeyEnter()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (Input::GetKeyDown(KeyCode::UP))
	{
		
		m_selectIndex += 5;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
	if (Input::GetKeyDown(KeyCode::DOWN))
	{
		
		m_selectIndex-=5;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }
	}
	

	if (Input::GetKeyDown(KeyCode::LEFT))
	{
		m_selectIndex--;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }

	}

	if (Input::GetKeyDown(KeyCode::RIGHT))
	{
		m_selectIndex++;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
	//============================================================================


}

void StageSelectObject::SetStageID(int StageID)
{
	m_StageID = StageID;
}

void StageSelectObject::SetPosID()
{
	const int StageCount = 30;
	m_TargetPos = m_Pos;
	int diff = m_StageID - m_selectIndex;

	if (diff > StageCount / 2)
		diff -= StageCount;
	else if (diff < -StageCount / 2)
		diff += StageCount;

	float PosX = 0.0f;
	float PosY = -2.0f;
	float PosZ = 0.0f;
	float Distance = 9.0f;

	

	// 表示範囲外
	if (diff <= -3 || diff >= 3)
	{
		if (diff <= -2 || diff >= 2)
		{
			m_Pos =
			{
				PosX + Distance * diff,
				PosY,
				PosZ
			};
		}
		return;
	}
	else
	{
		

		m_TargetPos =
		{
			PosX + Distance * diff,
			PosY,
			PosZ
		};
		

	}


		Vector3 toTarget = m_TargetPos - m_Pos;
		float distance = toTarget.Magnitude();

		if (distance < 0.001f)
		{
			
			return;
		}
			


		Vector3 dir = toTarget.Normalized();
		float move = m_MoveSpeed;

		if (move >= distance)
		{
			m_Pos = m_TargetPos; // 行き過ぎ防止
		}
		else
		{
			m_Pos = m_Pos + dir * move;
		}

		GetGameObject()->GetTransform()->SetPosition(m_Pos);
	
}



