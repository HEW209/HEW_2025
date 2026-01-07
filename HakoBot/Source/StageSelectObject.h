#pragma once
#include <DXGameFrame.h>
#include "StageNumber.h"
#include "OutlineMeshRenderer.h"

class StageSelectObject : public Component
{
public:
	StageSelectObject() = default;
	~StageSelectObject() = default;

	void Start() override;
	void Update()override;

	void SetStageNumber(StageNumber* stageNumber);

private:
	ObjPtr<StageNumber> m_stageNumber;
	GameObject* m_blocks[StageCount];
	std::vector<OutlineMeshRenderer*> m_blockRenderers;

	void Move();
	void BlockMove();
	void BlockRotate();
};