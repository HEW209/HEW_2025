#pragma once
//GuideUITimeController.h
#include <DXGameFrame.h>
#include <Vector>

class GuideUITimeController : public Component
{
	void Start() override;
	void Update() override;

	SpriteRenderer* m_pRenderer1;
private:
};

