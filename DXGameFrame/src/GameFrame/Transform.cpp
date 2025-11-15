//Transform.cpp
#include <GameFrame/Transform.h>

Transform::Transform(GameObject* owner) :
	m_position(Vector3::zero),
	m_scale(Vector3::one),
	m_euler(Vector3::zero),
	m_quaternion(Quaternion::identity)
{
}
