//StageArrow.h
#pragma once

#include <DXGameFrame.h>
#include "StageNumber.h"

class StageArrow : public Component
{
public:
	
	void Start();
	void Update();
	void SetStageNumber(StageNumber* stageNumber) { m_stageNumber = stageNumber; }
private:
	ObjPtr<SpriteRenderer> m_ArrowLeft;
	ObjPtr<SpriteRenderer> m_ArrowRight;
	ObjPtr<StageNumber> m_stageNumber;
	float m_ease;
};