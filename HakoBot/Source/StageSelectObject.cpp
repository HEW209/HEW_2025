#include "GameScene.h"
#include "StageSelectObject.h"
#include "LevelSerializer.h"



#include <memory>


#define DEFAULT_POSY -3.5f
#define CENTER_POSZ -1.8f

void StageSelectObject::Start()
{
	//==========開始時に右から流れる演出の為Posを設定==========
	switch (m_StageID)
	{

	case 0:
		m_Pos = { 20.0f,DEFAULT_POSY ,0.0f };
		break;
	case 1:
		m_Pos = { 24.0f,DEFAULT_POSY ,0.0f };
		break;
	case 2:
		m_Pos = { 28.0f,DEFAULT_POSY ,0.0f };
		break;
	case 3:
		m_Pos = { 32.0f,DEFAULT_POSY ,0.0f };
		break;
	case 23:
		m_Pos = { -9.0f,DEFAULT_POSY ,0.0f };
		break;
	case 24:
		m_Pos = { -5.0f,DEFAULT_POSY ,0.0f };
		break;
	case 25:
		m_Pos = { -1.0f,DEFAULT_POSY ,0.0f };
		break;
	case 26:
		m_Pos = { 3.0f,DEFAULT_POSY ,0.0f };
		break;
	case 27:
		m_Pos = { 7.0f,DEFAULT_POSY ,0.0f };
		break;
	case 28:
		m_Pos = { 11.0f,DEFAULT_POSY ,0.0f };
		break;
	case 29:
		m_Pos = { 15.0f,DEFAULT_POSY ,0.0f };
		break;
	case 30:
		m_Pos = { 18.0f,DEFAULT_POSY ,0.0f };
		break;
	default:
		m_Pos = { 20.0f,DEFAULT_POSY ,0.0f };
		break;
	}
	
	m_TargetPos = m_Pos;
	//=========================================================
	
}


void StageSelectObject::Update()
{

	KeyEnter();
	SetPosID();
	// 回転処理
	if (m_IsCenter)
	{
		m_RotateY += m_RotateSpeed;

		if (m_RotateY >= 360.0f)
			m_RotateY -= 360.0f;
	}
	else
	{
		// 現在角度から徐々に0へ戻す
		float returnSpeed = 0.1f; 
		float Lerp = m_RotateY + (0.0f - m_RotateY) * returnSpeed;	//Leap=a+(b-a)*t
		
		m_RotateY = Lerp;

		// ぶれ防止
		if (std::fabs(m_RotateY) < 0.01f)
			m_RotateY = 0.0f;
	}


	GetGameObject()->GetTransform()->SetEulerAngle(0.0f, m_RotateY, 0.0f);

}

void StageSelectObject::KeyEnter()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (m_bCoolCount==false)
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

void StageSelectObject::SetStageID(int StageID)
{
	m_StageID = StageID;	//呼び出された順番にステージIDをセット
}

void StageSelectObject::SetPosID()
{

	float PosX = 0.0f;
	float PosY = DEFAULT_POSY;
	float PosZ = 0.0f;
	float Distance = 9.0f;

	m_TargetPos = m_Pos;

	int diff = m_StageID - m_selectIndex;
	if (diff > m_stageCount / 2)
		diff -= m_stageCount;
	else if (diff < -m_stageCount / 2)
		diff += m_stageCount;



	m_IsCenter = (diff == 0);

	// 表示範囲外
	if (diff <= -8 || diff >= 8)
	{
		m_Pos = { PosX + Distance * diff,PosY,PosZ };
		return;
	}
	else
	{
		if (m_IsCenter)
		{
			PosZ = CENTER_POSZ;
			PosY = DEFAULT_POSY + 1.0f;
			GetGameObject()->GetTransform()->SetScale(1.5f, 1.5f, 1.5f);
		}
		else
		{
			GetGameObject()->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
		}
		m_TargetPos = { PosX + Distance * diff,PosY,PosZ };
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




std::string StageSelectObject::SetModelID()
{
	std::string blockName = "tate3masu";
	switch (m_StageID)
	{
		case 0:
			blockName = "1masu";
			break;
		case 1:
			blockName = "Lji1";
			break;
		case 2:
			blockName = "tate2masu";
			break;
		case 3:
			blockName = "tate3masu";
			break;
		case 4:
			blockName = "yoko2masu";
			break;
		case 5:
			blockName = "yoko3masu";
			break;
		case 6:
			blockName = "1masu";
			break;
		case 7:
			blockName = "Lji1";
			break;
		case 8:
			blockName = "tate2masu";
			break;
		case 9:
			blockName = "tate3masu";
			break;
		case 10:
			blockName = "yoko2masu";
			break;
		case 11:
			blockName = "yoko3masu";
			break;
		case 12:
			blockName = "1masu";
			break;
		case 13:
			blockName = "Lji1";
			break;
		case 14:
			blockName = "tate2masu";
			break;
		case 15:
			blockName = "tate3masu";
			break;
		case 16:
			blockName = "yoko2masu";
			break;
		case 17:
			blockName = "yoko3masu";
			break;
		case 18:
			blockName = "1masu";
			break;
		case 19:
			blockName = "Lji1";
			break;
		case 20:
			blockName = "tate2masu";
			break;
		case 21:
			blockName = "tate3masu";
			break;
		case 22:
			blockName = "yoko2masu";
			break;
		case 23:
			blockName = "yoko3masu";
			break;
		case 24:
			blockName = "1masu";
			break;
		case 25:
			blockName = "Lji1";
			break;
		case 26:
			blockName = "tate2masu";
			break;
		case 27:
			blockName = "tate3masu";
			break;
		case 28:
			blockName = "yoko2masu";
			break;
		case 29:
			blockName = "yoko3masu";
			break;
		case 30:
			blockName = "1masu";
			break;
	default:
		    blockName = "tate3masu";
		break;
	}
	
	std::string blockPath = "Assets/Model/Blocks/FBX/" + blockName + ".fbx";
	return blockPath;
}



