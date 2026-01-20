#pragma once
//GuideUITimeController.h
#include <DXGameFrame.h>
#include <Vector>
#include "Easing.h"


class GuideUITimeController : public Component
{
public:
	void Start() override;			
	void Update() override;			
	void SetDigitUV();					//UV座標を設定
	void SetTimer();					//時間を求めてメンバ変数にそれぞれの桁に値を格納

	int m_totalTime;					//毎フレーム足される
	int m_digit[6];						//各桁のディジットを格納(SetDigitUVで使います)
	bool m_b;							//リザルト出す用　falseにすると時が止まる
	SpriteRenderer* sprite[6];			//各桁の描画用
	SpriteRenderer* sprite1[3];			//時計とコロン描画用
private:
	int m_time;
	bool m_b2;
	ObjPtr<GameObject> m_root;
};

