#include "SampleCamera.h"

#include "InputSystem.h"

SampleCamera::SampleCamera():
	m_cameraAngle(0.0f),
	m_cameraDistance(10.0f),
	m_rotateSpeed(2.0f)
{
}

void SampleCamera::Update()
{
	float input = 0.0f;

	if (InputSystem::GetButton("CameraRight"))
		input -= 1.0f;
	if (InputSystem::GetButton("CameraLeft"))
		input += 1.0f;

	GetTransform()->Rotate(0.0f, input * m_rotateSpeed, 0.0f);
}
