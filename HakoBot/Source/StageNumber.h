#pragma once
#include <DXGameFrame.h>



class StageNumber : public Component
{
public:
	StageNumber() = default;
	~StageNumber() = default;

	void Start() override;
	void Update()override;


	void LoadGame(int StageID);					//ステージ読み出し
	void KeyEnter_Number();							//キー入力
	void AutoCount();

	
	void SetDigitUV();					//UV座標を設定
	void SetStegeNumberdigit();					//時間を求めてメンバ変数にそれぞれの桁に値を格納


	int digit[6];						//各桁のディジットを格納(SetDigitUVで使います)

	

private:

	SpriteRenderer* m_NumberSprite[6];			//各桁の描画用
	
	//========================================ステージの変数=================================================
	int m_selectIndex;					//小面(1-X)インデックス変数
	const int m_stageCount = 30;		//m_UP_DOWN_selectIndexの最大数（小ステージ数）
	//=======================================================================================================	
	
	float m_totalTime;
	bool m_bAutoCountUp=false;
	bool m_bAutoCountDown = false;
	int m_StegeNumberdigit =0;


};