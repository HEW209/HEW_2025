#pragma once

#include <DXGameFrame.h>

class Manual : public Component
{
public:
	Manual();

	void Start() override;
	void Update() override;

	enum State
	{
		DEFAULT,
		DOWN,
		UP,
		DISPLAY
	};

public:

	void ChangeState(State state);

private:

	State m_currentState;
	SpriteRenderer* m_manual;
	float m_time;

	void UpdateDefault();
	void UpdateDown();
	void UpdateUp();
	void UpdateDisplay();
};