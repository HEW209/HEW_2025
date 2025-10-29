//MatrixUtil.cpp
#include <DirectX/MatrixUtil.h>
#include <DirectX/DirectXInclude.h>

DirectX::XMFLOAT4X4 MatrixUtil::CreateWorldMatrix(Transform* pTransform)
{
	DirectX::XMMATRIX T;	//移動行列
	DirectX::XMMATRIX S;	//スケール行列
	DirectX::XMMATRIX R;	//回転行列

	Vector3 pos = pTransform->m_position;
	Vector3 scale = pTransform->m_scale;
	Quaternion qua = pTransform->GetQuaternion();

	//それぞれの変換行列を求める
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	R = DirectX::XMMatrixRotationQuaternion(
		DirectX::XMVectorSet(qua.x, qua.y, qua.z, qua.w)
	);

	//変換行列の合成・転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(S * R * T)
	);

	return result;
}

DirectX::XMFLOAT4X4 MatrixUtil::CreateViewMatrix(Camera* pCamera)
{
	DirectX::XMMATRIX T;	//移動行列
	DirectX::XMMATRIX R;	//スケール行列

	Vector3 pos = pCamera->GetTransform()->m_position;
	Quaternion qua = pCamera->GetTransform()->GetQuaternion();

	//それぞれの変換行列を求める
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	R = DirectX::XMMatrixRotationQuaternion(
		DirectX::XMVectorSet(qua.x, qua.y, qua.z, qua.w)
	);

	//変換行列の合成・転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixInverse(nullptr, R * T)
		)
	);

	return result;
}

DirectX::XMFLOAT4X4 MatrixUtil::CreateProjectionMatrix(Camera* pCamera)
{
	Camera::Config config = pCamera->GetConfig();

	//プロジェクション行列を求める
	DirectX::XMMATRIX matrix;
	matrix = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(config.fovAngle),
		config.screenRatio, config.nearZ, config.farZ
	);

	//変換行列の転置
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(
		&result, DirectX::XMMatrixTranspose(matrix)
	);

	return result;
}
