#pragma once

#include <DXGameFrame.h>
#include <cmath>
#include <algorithm>

class PlayerMove : public Component
{
public:
	PlayerMove();
	~PlayerMove() = default;

	void Start()override;
	void Update() override;

	void SetVelocityY(float y) { m_velocity_y = y; }

private:
	float m_moveSpeed;
	float m_jumpPower;
	float m_gravity;
	float m_velocity_y;
	float m_rotateSpeed ; 

};