//ResultController.h
//2026 01 15 ç≤ì°ëìñÁ
//ÉäÉUÉãÉgâÊñ ÇçÏÇËÇ‹Ç∑
//Ç†ÇØÇ‹ÇµÇƒÇ®ÇﬂÇ≈Ç∆Ç§Ç≤Ç¥Ç¢Ç‹Ç∑
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
	ObjPtr<SpriteRenderer> m_result;
	ObjPtr<SpriteRenderer> m_selectText[3];
	ObjPtr<SpriteRenderer> m_illust;
	ObjPtr<GameObject> m_clearTime;

	float m_resultTime;

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
};