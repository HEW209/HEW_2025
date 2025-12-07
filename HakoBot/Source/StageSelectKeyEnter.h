#pragma once
#include <DXGameFrame.h>

class StegeSelectKeyEnter : public Component
{
public:
	StegeSelectKeyEnter()=default;
	~StegeSelectKeyEnter()=default;

	void Start() override;
	void Update()override;

	void SetStageID();
private:
	//========================================ステージの変数=================================================
	int m_UP_DOWN_selectIndex;				//小面(1-X)インデックス変数
	int m_LEFT_RIGHT_selectIndex;			//大面(X-1)インデックス変数
	const int m_SMALL_stageCount = 10;		//m_UP_DOWN_selectIndexの最大数（小ステージ数）
	const int m_BIG_stageCount = 3;			//m_LEFT_RIGHT_selectIndexの最大数(大ステージ数)
	//=======================================================================================================	

};