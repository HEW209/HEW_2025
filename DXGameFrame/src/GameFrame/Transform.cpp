//Transform.cpp
#include <GameFrame/Transform.h>

Transform::Transform(GameObject* owner) :
	m_localPosition(Vector3::zero),
	m_localScale(Vector3::one),
	m_localEuler(Vector3::zero),
	m_localQuaternion(Quaternion::identity),
	m_pParent(nullptr),
	m_pChildren{}
{
}

Transform::~Transform()
{
	
}

Vector3 Transform::GetPosition(Space space)
{
	if (space == Space::LOCAL)
		return m_localPosition;

	if (space == Space::WORLD)
	{
		if (m_pParent == nullptr)
			return m_localPosition;

		// 親のワールド行列を取得
		DirectX::XMMATRIX parentMatrix = m_pParent->GetWorldMatrix();

		// 自身のワールド座標を求める
		DirectX::XMVECTOR worldPos;
		worldPos = DirectX::XMVector3Transform(
			m_localPosition.ToXMVector(),
			parentMatrix
		);

		// Vector3に変換
		DirectX::XMFLOAT3 temp;
		DirectX::XMStoreFloat3(&temp, worldPos);
		return Vector3(temp.x, temp.y, temp.z);
	}

	return Vector3();
}

Vector3 Transform::GetScale()
{
	return m_localPosition;
}

Vector3 Transform::GetEulerAngle(Space space)
{
	if (space == Space::LOCAL)
		return m_localEuler;

	if (space == Space::WORLD)
		return GetQuaternion(Space::WORLD).ToEuler();

	return Vector3();
}

Quaternion Transform::GetQuaternion(Space space)
{
	if (space == Space::LOCAL)
		return m_localQuaternion;

	if (space == Space::WORLD)
	{
		if (m_pParent != nullptr)
		{
			// 再帰的に回転を求める
			return m_localQuaternion * m_pParent->GetQuaternion(Space::WORLD);
		}
		else
		{
			// 親がなければ終了
			return m_localQuaternion;
		}
	}

	return Quaternion();
}

void Transform::SetPosition(Vector3 position)
{
	m_localPosition = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(x, y, z);
}

void Transform::SetScale(Vector3 scale)
{
	m_localScale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(x, y, z);
}

void Transform::SetEulerAngle(Vector3 euler)
{
	m_localEuler = euler;
	m_localQuaternion = Quaternion::Euler(m_localEuler);
}

void Transform::SetEulerAngle(float x, float y, float z)
{
	SetEulerAngle(Vector3(x, y, z));
}

void Transform::SetQuaternion(Quaternion quaternion)
{
	m_localQuaternion = quaternion;
	m_localEuler = m_localQuaternion.ToEuler();
}

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX T;	// 移動行列
	DirectX::XMMATRIX S;	// スケール行列
	DirectX::XMMATRIX R;	// 回転行列

	// それぞれの変換行列を求める
	T = DirectX::XMMatrixTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);
	S = DirectX::XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);
	R = DirectX::XMMatrixRotationQuaternion(m_localQuaternion.ToXMVector());

	// 変換行列の合成
	DirectX::XMMATRIX localMatrix = S * R * T;

	if (m_pParent != nullptr)
	{
		// 再帰的にワールド変換行列を求める
		return localMatrix * m_pParent->GetWorldMatrix();
	}
	else
	{
		// 親がなければ終了
		return localMatrix;
	}
}