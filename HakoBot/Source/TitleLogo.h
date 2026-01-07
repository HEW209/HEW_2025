// TitleLogo.h
#pragma once
#include <DXGameFrame.h>

class TitleLogo : public Component
{
public:
	TitleLogo();

	void Start() override;
	void Update() override;

private:
	GameObject* m_TitleLeft;
	GameObject* m_TitleRight;

	float m_leftMoveTimer;
	float m_RightMoveTimer;
};
