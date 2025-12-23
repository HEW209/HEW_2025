#pragma once
#include <DXGameFrame.h>


class StageSelectObject : public Component
{
public:
	StageSelectObject() = default;
	~StageSelectObject() = default;

	void Start() override;
	void Update()override;


	
	void KeyEnter();
	void SetStageID(int StageID);
	void SetPosID();
	std::string SetModelID();
private:

	//========================================ステージの変数=================================================
	int m_selectIndex=0;					//小面(1-X)インデックス変数
	const int m_stageCount = 30;		//m_UP_DOWN_selectIndexの最大数（小ステージ数）
	//=======================================================================================================	
	Vector3 m_Pos;        // 現在位置
	Vector3 m_TargetPos; // 目的位置
	float   m_MoveSpeed = 0.5f; // 移動速度

	float m_RotateY = 0.0f;          // 現在のY回転角
	float m_RotateSpeed = 0.4f;     // 回転supeed
	bool  m_IsCenter = false;        // 中央かどうか

	int m_StageID = 0;
	SpriteRenderer* m_ObjectSprite;			//各桁の描画用
	


};