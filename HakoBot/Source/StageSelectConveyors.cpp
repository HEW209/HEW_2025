
#include "StageSelectConveyors.h"




#include <memory>


#define DEFAULT_POSY -6.6f

void StageSelectConveyors::Start()
{


	
}


void StageSelectConveyors::Update()
{
	if (m_farst == true)
	{
		float PosX = 0.0f;
		float PosY = DEFAULT_POSY;
		float PosZ = 0.0f;
		float Distance = 9.0f;


		int diff = m_ConveyorsID - m_selectIndex;
		if (diff > m_stageCount / 2)
			diff -= m_stageCount;
		else if (diff < -m_stageCount / 2)
			diff += m_stageCount;


		m_Pos = { PosX + Distance * diff,PosY,PosZ };
		GetGameObject()->GetTransform()->SetPosition(m_Pos);
		m_TargetPos = m_Pos;
		m_farst = false;
	}
	KeyEnter();
	SetPosConveyors();

}

void StageSelectConveyors::KeyEnter()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (m_bCoolCount == false)
	{
		if (Input::GetKeyDown(KeyCode::UP))
		{

			m_selectIndex += 5;
			if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
			m_bCoolCount = true;
		}
		if (Input::GetKeyDown(KeyCode::DOWN))
		{

			m_selectIndex -= 5;
			if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }
			m_bCoolCount = true;
		}
	}
	else
	{
		CountUPTimer++;
		if (CountUPTimer > 90)
		{
			CountUPTimer = 0.0f;
			m_bCoolCount = false;
		}
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

void StageSelectConveyors::SetConveyorsID(int ConveyorsID)
{
	m_ConveyorsID = ConveyorsID;
}



void StageSelectConveyors::SetPosConveyors()
{

	float PosX = 0.0f;
	float PosY = DEFAULT_POSY;
	float PosZ = 0.0f;
	float Distance = 9.0f;

	m_TargetPos = m_Pos;

	int diff = m_ConveyorsID - m_selectIndex;
	if (diff > m_stageCount / 2)
		diff -= m_stageCount;
	else if (diff < -m_stageCount / 2)
		diff += m_stageCount;

	// 表示範囲外
	if (diff <= -(m_stageCount / 2 -7) || diff >= (m_stageCount / 2 -7))
	{
		m_Pos = { PosX + Distance * diff,PosY,PosZ };
		return;
	}
	else
	{
		m_TargetPos = { PosX + Distance * diff,PosY,PosZ };
	}

	if(m_ConveyorsID%2==0)
	{
		m_TargetPos = { PosX + Distance * diff,PosY-10,PosZ };
	}
	Vector3 toTarget = m_TargetPos - m_Pos;
	float distance = toTarget.Magnitude();
	if (distance < 0.001f)return;

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








