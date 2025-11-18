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
	~Collider() = default;

	void Update()override;
	void Awake()override;
	void OnDestroy()override;

	static void CheckCollision();

private:

	Transform m_transform;

	static std::vector<Collider*> s_colliders;
	static std::vector<Axis3> s_axis;

};