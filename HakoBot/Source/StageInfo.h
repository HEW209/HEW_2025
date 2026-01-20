// StageInfo.h
#pragma once
#include <DXGameFrame.h>
#include "StageNumber.h"

class StageInfo : public Component
{
public:
	StageInfo();

	void Start() override;
	void Update() override;
	
	void SetStageNumber(StageNumber* stageNumber);

private:
	using ClearShape = std::vector<std::vector<ObjPtr<SpriteRenderer>>>;
	std::vector<ClearShape> m_clearShapes;
	ObjPtr<SpriteRenderer> m_pTitle;

	StageNumber* m_stageNumber;
	int m_currentStage;
	Vector2 m_clearShapePos[3];

	void CreateClearShape();
};
