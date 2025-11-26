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

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	DirectX::XMMATRIX world;	// カメラのワールド行列
	world = GetTransform()->GetWorldMatrix();

	// 変換行列の合成・転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixInverse(nullptr, world)
		)
	);

	return result;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	// プロジェクション行列を求める
	DirectX::XMMATRIX matrix;
	matrix = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_config.fovAngle),
		m_config.screenSize.x / m_config.screenSize.y, m_config.nearZ, m_config.farZ
	);

	// 変換行列の転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(matrix)
	);

	return result;
}

DirectX::XMFLOAT4X4 Camera::GetOrthographicProjectionMatrix()
{
	// プロジェクション行列を求める
	DirectX::XMMATRIX matrix;
	int pixelPerUnit = ConfigManager::Instance().GetInt(ConfigKey::PixelPerUnit);
	float halfWidth = m_config.screenSize.x * 0.5f / (float)pixelPerUnit;
	float halfHeight = m_config.screenSize.y * 0.5f / (float)pixelPerUnit;
	matrix = DirectX::XMMatrixOrthographicOffCenterLH(
		-halfWidth, halfWidth, -halfHeight, halfHeight,
		0.0f, m_config.farZ
	);

	// 変換行列の転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(matrix)
	);

	return result;
}

Camera* Camera::GetMain()
{
	return s_pMainCamera;
}

DirectX::XMFLOAT4X4 Camera::GetDefaultViewMatrix()
{
	DirectX::XMMATRIX world;	// カメラのワールド行列
	world = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	// 変換行列の合成・転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixInverse(nullptr, world)
		)
	);

	return result;
}
