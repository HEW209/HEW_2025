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
	void LateUpdate() override;

	void SetVelocityY(float y) { m_velocity_y = y; }
	void SetCaterpillar(MeshRenderer* renderer);
	void SetHead(GameObject* pHead) { m_pHead = pHead; }

private:
	float m_moveSpeed;
	float m_jumpPower;
	float m_gravity;
	float m_velocity_y;
	float m_rotateSpeed ; 

	ObjPtr<MeshRenderer> m_pCaterpillar;
	Vector2 m_uvOffset;

	GameObject* m_pHead;
	bool m_IsDirLock;
	Quaternion m_HeadQuaternion;
};