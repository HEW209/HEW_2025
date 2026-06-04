// Camera.cpp
#include <Component/Camera.h>
#include <GameFrame/Transform.h>
#include <DirectX/ConfigManager.h>

Camera* Camera::s_pMainCamera = nullptr;

Camera::Camera()
{
	if (s_pMainCamera == nullptr)
	{
		s_pMainCamera = this;
	}
}

Camera::~Camera()
{
	if (s_pMainCamera == this)
	{
		s_pMainCamera = nullptr;
	}
}

void Camera::SetConfig(Config config)
{
	m_config = config;
}

Camera::Config Camera::GetConfig()
{
	return m_config;
}

void Camera::SetMain()
{
	s_pMainCamera = this;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	DirectX::XMMATRIX view;		// 計算用ビュー行列

	// ビュー行列を求める
	view = GetTransform()->GetWorldMatrix();
	view = DirectX::XMMatrixInverse(nullptr, view);
	return view;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	DirectX::XMMATRIX projection;	// 計算用プロジェクション行列

	// プロジェクション行列を求める
	projection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_config.fovAngle),
		m_config.screenSize.x / m_config.screenSize.y, m_config.nearZ, m_config.farZ
	);
	return projection;
}

DirectX::XMMATRIX Camera::GetShadowProjectionMatrix(float shadowDistance)
{
	DirectX::XMMATRIX projection;	// 計算用プロジェクション行列

	// プロジェクション行列を求める
	projection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_config.fovAngle),
		m_config.screenSize.x / m_config.screenSize.y, m_config.nearZ, shadowDistance
	);
	return projection;
}

DirectX::XMMATRIX Camera::GetOrthographicProjectionMatrix()
{
	DirectX::XMMATRIX projection;	// 計算用プロジェクション行列
	int pixelPerUnit = ConfigManager::Instance().GetInt(ConfigKey::PixelPerUnit);

	// 投影サイズを求める
	float halfWidth = m_config.screenSize.x / (float)pixelPerUnit;
	float halfHeight = m_config.screenSize.y / (float)pixelPerUnit;
	halfWidth *= m_config.cameraScale * 0.5f;
	halfHeight *= m_config.cameraScale * 0.5f;

	// プロジェクション行列を求める
	projection = DirectX::XMMatrixOrthographicOffCenterLH(
		-halfWidth, halfWidth, -halfHeight, halfHeight,
		0.0f, m_config.farZ
	);
	return projection;
}

Camera* Camera::GetMain()
{
	return s_pMainCamera;
}

DirectX::XMMATRIX Camera::GetDefaultViewMatrix()
{
	DirectX::XMMATRIX view;		// 計算用ビュー行列

	// ビュー行列を求める
	view = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	view = DirectX::XMMatrixInverse(nullptr, view);
	return view;
}
