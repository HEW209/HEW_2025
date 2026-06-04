#pragma once
//GuideUIController.h

#include <DXGameFrame.h>


class GuideUIController2 : public Component
{
public:
	GuideUIController2();

	void Start() override;
	void Update() override;

private:
	SpriteRenderer* m_renderer;
	float m_easeTime;
};

