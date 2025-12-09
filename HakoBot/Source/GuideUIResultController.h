#pragma once
//GuideUIResultController.h

#include <DXGameFrame.h>

class GuideUIResultController : public Component
{
	void Start() override;
	void Update() override;

	Vector3 m_defaultPosition;
	Vector3 m_defaultScale;
private:
	float m_value = 0;
};

