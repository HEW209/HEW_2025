#pragma once
//GuideUIController.h

#include <DXGameFrame.h>


class GuideUIController2 : public Component
{
public:

	void Start() override;
	void Update() override;

	bool Complete;
private:
	float m_value;
	Vector3 m_defaultPosition;
	Vector3 m_defaultScale;
};

