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
	SpriteRenderer* GetSprite();

	int m_totalTime;
	int m_digit[6];
	bool m_b;
	SpriteRenderer* sprite[6];
	SpriteRenderer* sprite1[3];
private:
	Vector2 m_defaultOffPos;
	
  
};

