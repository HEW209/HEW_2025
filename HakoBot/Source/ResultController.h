//ResultController.h
//2026 01 15 佐藤蒼也
//リザルト画面を作ります
//あけましておめでとうございます
#pragma once

#include <DXGameFrame.h>
#include "GameFrame/Component.h"

class ResultController : public Component
{
public:
	ResultController();
	void Start() override;
	void Update() override;
private:
	ObjPtr<SpriteRenderer> m_screenFade;
	ObjPtr<SpriteRenderer> m_kuroDown;
	ObjPtr<SpriteRenderer> m_kuroUp;
	std::array<ObjPtr<GameObject>, 4> m_resultTexts;
	ObjPtr<GameObject> m_resultTextRoot;
	ObjPtr<SpriteRenderer> m_selectText[3];
	ObjPtr<SpriteRenderer> m_illust;
	ObjPtr<GameObject> m_clearTime;
	Vector2 m_resultPos;
	Vector2 m_charaPos;
	Vector2 m_charaScale;
	float m_resultTime;
	float m_animTime;

	enum class ResultState
	{
		MOVE,SELECT,END
	};

	ResultState m_state;
	void MoveUpdate();
	void SelectUpdate();
	void EndUpdate();

	enum Select
	{
		NEXT,STAGE_SELECT,RESTART,COUNT
	};

	int m_currentSelect;

	enum class BGMState
	{
		NONE,START,LOOP
	};

	BGMState m_bgmState;

	float m_cursorWheelTimer;
};