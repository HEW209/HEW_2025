#pragma once

#include <DXGameFrame.h>
#include <cmath>
#include <algorithm>

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
	float m_rotateSpeed ; 

};