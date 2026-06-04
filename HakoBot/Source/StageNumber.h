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
	std::string GetStageFilePath();
	bool IsStageStart()
	{
		return m_isSceneChange && m_targetScene == TargetScene::GAME;
	}

private:
	enum TargetScene
	{
		GAME,
		TITLE
	};

	SpriteRenderer* m_NumberSprite[3];			//各桁の描画用
	SpriteRenderer* m_line;
	int m_selectIndex;
	int m_lastInput;			// 左で-1 右で1
	bool m_keyHold;				// 長押しフラグ
	float m_holdTimer;			// 長押し時間
	float m_repeatTimer;		// 長押し移動用タイマー
	TargetScene m_targetScene;	// 遷移先シーン
	bool m_isSceneChange;		// シーン遷移中フラグ

	void StageSelect();
	void LoadGame();					//ステージ読み出し	
	void SetDigitUV();					//UV座標を設定
};