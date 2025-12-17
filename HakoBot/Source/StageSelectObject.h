#pragma once
#include <DXGameFrame.h>

class StegeSelectKeyEnter;

class StageSelectObject : public Component
{
public:
	StageSelectObject() = default;
	~StageSelectObject() = default;

	void Start() override;
	void Update()override;


	void LoadGame(int StageID);
	void KeyEnter();
	void SetStageID(int StageID);
	Vector2 SetPosID(int StageID);
	
private:

	//========================================ステージの変数=================================================
	int m_selectIndex;					//小面(1-X)インデックス変数
	const int m_stageCount = 30;		//m_UP_DOWN_selectIndexの最大数（小ステージ数）
	//=======================================================================================================	

	int m_StageID=0;


};