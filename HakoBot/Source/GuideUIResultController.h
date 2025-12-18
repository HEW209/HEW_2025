#pragma once
//GuideUIResultController.h

#include <DXGameFrame.h>

class GuideUIResultController : public Component
{
public:
	void Start() override;
	void Update() override;
	bool GetMenuBool();

	Vector3 m_defaultPosition;
	Vector3 m_defaultScale;
	bool m_menu;
private:
	float m_value = 0;
	
	bool m_menu2;
	SpriteRenderer* m_rend;
	SpriteRenderer* m_rend2;
};

