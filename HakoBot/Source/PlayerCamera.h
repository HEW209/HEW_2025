/*********************************************************************
 * @file   PlayerCamera.h
 * @brief  プレイヤーが動かすカメラを動かす
 *
 * @author 三品怜
 * @date   2025/12/04
 *********************************************************************/


#pragma once

#include <DXGameFrame.h>

class PlayerCamera : public Component
{
public:

	PlayerCamera();
	~PlayerCamera() = default;

	void Start() override;
	void LateUpdate() override;

	void SetPlayer(Transform* pPlayerTransform) { m_pPlayerTranform = pPlayerTransform; }
	void SetCameraDistance(float distance);

private:
	float m_cameraDistance;
	float m_rotateSpeed;

	float m_currentAngleY;
	float m_currentAngleX;

	Transform* m_pPlayerTranform;
	Vector3 m_posOffset;
};