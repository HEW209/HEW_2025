#pragma once

#include <DXGameFrame.h>

class TitleStage : public Component
{
public:
	TitleStage();
	virtual ~TitleStage() = default;

	void Awake() override;
	void Update() override;

private:

	ObjPtr<MeshRenderer> m_pConveyors;

	float m_time;
}; 