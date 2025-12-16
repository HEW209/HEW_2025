#pragma once
//GuideUITimeController2.h
#include <DXGameFrame.h>
#include "GuideUITimeController.h"

class GuideUITimeController2 : public Component
{
	void Start() override;
	void Update() override;

private:
	int m_time2;
	bool m_b3;
	SpriteRenderer* m_restart;
	SpriteRenderer* m_stageSelect;
	int m_curcirPos;
	bool isActive;
};

