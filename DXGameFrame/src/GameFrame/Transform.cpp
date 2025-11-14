//Transform.cpp
#include <GameFrame/Transform.h>

Transform::Transform(GameObject* owner) :
	m_position(Vector3::zero),
	m_scale(Vector3::one),
	m_euler(Vector3::zero),
	m_quaternion(Quaternion::identity),
	m_pParent(nullptr),
	m_pChildren{}
{
}

Vector3 Transform::GetWorldPosition()
{
	if (m_pParent == nullptr)
		return m_position;

	DirectX::XMVECTOR localPos;
	DirectX::XMVECTOR worldPos;
	localPos = DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.0f);
	worldPos = DirectX::XMVector3Transform(localPos, m_pParent->GetWorldMatrix());
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, worldPos);

	Vector3 result(temp.x, temp.y, temp.z);
	return result;
}

Vector3 Transform::GetWorldScale()
{
	if (m_pParent == nullptr)
		return m_scale;

	DirectX::XMVECTOR localScale;
	DirectX::XMVECTOR worldScale;
	localScale = DirectX::XMVectorSet(m_scale.x, m_scale.y, m_scale.z, 1.0f);
	worldScale = DirectX::XMVector3Transform(localScale, m_pParent->GetWorldMatrix());
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, worldScale);

	Vector3 result(temp.x, temp.y, temp.z);
	return result;
}

Quaternion Transform::GetWorldQuaternion()
{
	if (m_pParent == nullptr)
		return m_quaternion;

	return m_quaternion * m_pParent->m_quaternion;
}

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX T;	// 移動行列
	DirectX::XMMATRIX S;	// スケール行列
	DirectX::XMMATRIX R;	// 回転行列

	Vector3 pos = m_position;
	Vector3 scale = m_scale;
	Quaternion qua = GetQuaternion();

	// それぞれの変換行列を求める
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	R = DirectX::XMMatrixRotationQuaternion(
		DirectX::XMVectorSet(qua.x, qua.y, qua.z, qua.w)
	);

	// 変換行列の合成
	DirectX::XMMATRIX matrix = S * R * T;
	if (m_pParent == nullptr)
		return matrix;

	return matrix * m_pParent->GetWorldMatrix();
}

