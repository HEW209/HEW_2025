#pragma once
#include <DXGameFrame.h>


class StageSelectConveyors : public Component
{
public:
	StageSelectConveyors() = default;
	~StageSelectConveyors() = default;

	void Start() override;
	void Update()override;



	void KeyEnter();
	void SetConveyorsID(int ConveyorsID);
	void SetPosConveyors();

private:

	//========================================ステージの変数=================================================
	int m_selectIndex = 0;					//小面(1-X)インデックス変数
	const int m_stageCount = 30;		//m_UP_DOWN_selectIndexの最大数（小ステージ数）
	//=======================================================================================================	
	Vector3 m_Pos;        // 現在位置
	Vector3 m_TargetPos; // 目的位置
	float   m_MoveSpeed = 0.5f; // 移動速度


	bool m_farst = true;
	bool m_bCoolCount = false;
	float CountUPTimer = 0.0f;
	int m_ConveyorsID = 0;

	SpriteRenderer* m_ObjectSprite;			//各桁の描画用



};