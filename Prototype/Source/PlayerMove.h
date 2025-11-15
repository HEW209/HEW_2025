#pragma once

#include <DXGameFrame.h>

class PlayerMove : public Component
{
public:
	PlayerMove();
	~PlayerMove() = default;

	void Update() override;

private:
	float m_moveSpeed;
	float m_jumpPower;
	float m_gravity;
	float m_velocity_y;
};