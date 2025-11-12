#include "SampleCamera.h"

SampleCamera::SampleCamera():
	m_cameraAngle(0.0f),
	m_cameraDistance(10.0f),
	m_rotateSpeed(2.0f)
{
}

void SampleCamera::Update()
{
	float input = 0.0f;

	if (InputManager::GetKeyHold(Input::MOUSE_LEFT))
		input -= 1.0f;
	if (InputManager::GetKeyHold(Input::MOUSE_RIGHT))
		input += 1.0f;

	GetTransform()->Rotate(0.0f, input * m_rotateSpeed, 0.0f);

	Vector3 cameraPos;
	cameraPos = GetTransform()->GetQuaternion() * Vector3::back;
	cameraPos *= m_cameraDistance;
	cameraPos.y += 4;

	GetTransform()->m_position = cameraPos;
}
