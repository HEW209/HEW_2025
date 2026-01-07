#pragma once
#include <DXGameFrame.h>

constexpr int StageCount = 30;		// ステージ数

class StageNumber : public Component
{
public:
	StageNumber();
	~StageNumber() = default;

	void Start() override;
	void Update() override;

	int GetSelectIndex();

private:
	SpriteRenderer* m_NumberSprite[2];			//各桁の描画用
	int m_selectIndex;
	int m_lastInput;		// 左で-1 右で1
	bool m_keyHold;			// 長押しフラグ
	float m_holdTimer;		// 長押し時間
	float m_repeatTimer;	// 長押し移動用タイマー

	void StageSelect();
	void LoadGame(int StageID);			//ステージ読み出し	
	void SetDigitUV();					//UV座標を設定
};