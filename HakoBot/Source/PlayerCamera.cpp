#include "PlayerCamera.h"

#include "InputSystem.h"

constexpr float X_ANGLE_MAX = 80.0f;
constexpr float X_ANGLE_MIN = -0.0f;

PlayerCamera::PlayerCamera() :
	m_cameraDistance(10.0f),
	m_rotateSpeed(2.0f),
	m_pPlayerTranform(nullptr),
	m_currentAngleY(0.0f),
	m_currentAngleX(20.0f),
	m_posOffset(0.0f, 1.0f, 0.0f)
{
}

void PlayerCamera::Start()
{
	auto obj = SceneManager::GetActiveScene()->CreateGameObject();
	auto camera = obj->AddComponent<Camera>();
	Camera::Config cameraConfig;
	cameraConfig.fovAngle = 30.0f;
	camera->SetConfig(cameraConfig);
	auto transform = obj->GetTransform();
	transform->SetParent(GetTransform());
	transform->SetPosition(0.0f, 0.0f, -m_cameraDistance,Space::LOCAL);
	GetTransform()->SetEulerAngle(m_currentAngleX, m_currentAngleY, 0.0f);
}

void PlayerCamera::LateUpdate()
{
	Vector2 input = InputSystem::GetAxis("CameraMove"_hash);

	m_currentAngleY += input.x * m_rotateSpeed;

	// x軸は反転
	m_currentAngleX += -(input.y) * m_rotateSpeed;

	// 角度制限
	if (X_ANGLE_MAX < m_currentAngleX)
	{
		m_currentAngleX = X_ANGLE_MAX;
	}
	else if(m_currentAngleX < X_ANGLE_MIN)
	{
		m_currentAngleX = X_ANGLE_MIN;
	}

	GetTransform()->SetEulerAngle(m_currentAngleX, m_currentAngleY, 0.0f);
	
	// プレイヤーに付随させる
	if (m_pPlayerTranform)
		GetTransform()->SetPosition(m_pPlayerTranform->GetPosition() + m_posOffset);
}

void PlayerCamera::SetCameraDistance(float distance)
{
	const float distanceToOffsetY = 0.05f;

	m_cameraDistance = distance;
	m_posOffset.y = distance * distanceToOffsetY + 0.5f;
}
