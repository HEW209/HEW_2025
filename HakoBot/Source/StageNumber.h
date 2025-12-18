#pragma once
#include <DXGameFrame.h>



class StageNumber : public Component
{
public:
	StageNumber() = default;
	~StageNumber() = default;

	void Start() override;
	void Update()override;


	void LoadGame(int StageID);
	void KeyEnter();

	void Set_StegeNumberdigit(int CallNum);
	Vector2 SetPos_StegeNumber(int CallNum);
	
private:

	//========================================ステージの変数=================================================
	int m_selectIndex;					//小面(1-X)インデックス変数
	const int m_stageCount = 30;		//m_UP_DOWN_selectIndexの最大数（小ステージ数）
	//=======================================================================================================	

	
	int m_StegeNumberdigit =0;


};