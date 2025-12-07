#pragma once
//GuideUITimeController.h
#include <DXGameFrame.h>
#include <Vector>
#include "Easing.h"

class GuideUITimeController : public Component
{
	void Start() override;
	void Update() override;

	SpriteRenderer* m_pRenderer1;
	Vector3 m_defaultPosition;
	Vector3 m_defaultScale;
private:
	float m_value = 0;
};

