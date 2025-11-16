#pragma once

#include <DXGameFrame.h>

class SamplePlayer : public Component
{
public:
	SamplePlayer();
	~SamplePlayer() = default;

	void Update() override;
	void Awake() override;

private:
	float m_moveSpeed;
	float m_jumpPower;
	float m_gravity;
	float m_velocity_y;
};