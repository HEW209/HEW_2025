#pragma once
//GuideUITimeController.h
#include <DXGameFrame.h>
#include <Vector>
#include "Easing.h"
#include "GuideUIController.h"



class GuideUITimeController : public Component
{
public:
	void Start() override;
	void Update() override;
	void SetDigitUV();
	void SetTimer();


	int m_totalTime;
	int m_digit[6];

private:
	Vector2 m_defaultOffPos;
	SpriteRenderer* sprite[6];
  
};

