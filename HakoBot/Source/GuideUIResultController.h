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
	bool m_menu;
	bool m_menu2;
	SpriteRenderer* m_rend;
	SpriteRenderer* m_rend2;
};

