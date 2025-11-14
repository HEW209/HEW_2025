/*****************************************************************//**
 * @file   Collider.h
 * @brief  当たり判定コンポーネント
 *
 * @author 三品怜
 * @date   2025/11/13
 *********************************************************************/
#pragma once

#include <DXGameFrame.h>
#include <vector>



class Collider :public Component
{
public:

	struct Axis3
	{
		Vector3 x, y, z;
	};

	Collider();
	~Collider();

	Quaternion GetQuaternion() { return m_rotateOffset; }

private:
	
	Vector3 m_positionOffset;
	Quaternion m_rotateOffset;
	Vector3 m_scale;
};

void CheckCollision();