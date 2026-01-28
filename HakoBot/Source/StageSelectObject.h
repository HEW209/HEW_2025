#pragma once
#include <DXGameFrame.h>
#include "StageNumber.h"
#include "OutlineMeshRenderer.h"

class StageSelectObject : public Component
{
public:
	StageSelectObject();
	~StageSelectObject() = default;

	void Start() override;
	void Update()override;

	void SetStageNumber(StageNumber* stageNumber);

private:
	ObjPtr<StageNumber> m_stageNumber;
	ObjPtr<EffectRenderer> m_effect;
	GameObject* m_blocks[StageCount];
	std::vector<OutlineMeshRenderer*> m_blockRenderers;
	bool m_easeStartFlag;
	float m_easeStartScale;
	float m_scaleEaseTimer;

	void Move();
	void BlockMove();
	void BlockRotate();
	void BlockScaling();
};