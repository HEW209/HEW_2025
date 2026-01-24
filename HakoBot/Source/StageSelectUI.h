//StageSelectUI.h
#pragma once

#include <DXGameFrame.h>
#include "StageNumber.h"

class StageSelectUI : public Component
{
public:
	StageSelectUI();

	void Start();
	void Update();
	void SetStageNumber(StageNumber* stageNumber) { m_stageNumber = stageNumber; }

private:
	ObjPtr<SpriteRenderer> m_arrowLeft;
	ObjPtr<SpriteRenderer> m_arrowRight;
	ObjPtr<SpriteRenderer> m_startText;
	ObjPtr<SpriteRenderer> m_button;
	ObjPtr<StageNumber> m_stageNumber;
	int m_lastSelectNumber;
	float m_ease;
	float m_hideTimer;
};