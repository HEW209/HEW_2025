// Camera.cpp
#include <Component/Camera.h>
#include <GameFrame/Transform.h>

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

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	DirectX::XMMATRIX world;	// カメラのワールド列
	world = Camera::GetMain()->GetTransform()->GetWorldMatrix();

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
		m_config.screenRatio, m_config.nearZ, m_config.farZ
	);

	// 変換行列の転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(matrix)
	);

	return result;
}
