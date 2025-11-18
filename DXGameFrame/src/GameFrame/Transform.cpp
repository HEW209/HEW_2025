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

Transform::~Transform()
{
	if (m_pParent != nullptr)
		m_pParent->DeleteChild(this);

	for (auto child : m_pChildren)
	{
		child->SetParent(nullptr);
	}
}

Vector3 Transform::GetPosition(Space space)
{
	if (space == Space::LOCAL)
		return m_position;

	if (space == Space::WORLD)
	{
		if (m_pParent == nullptr)
			return m_position;

		// 親のワールド行列を取得
		DirectX::XMMATRIX parentMatrix = m_pParent->GetWorldMatrix();

		// 自身のワールド座標を求める
		DirectX::XMVECTOR worldPos;
		worldPos = DirectX::XMVector3Transform(
			m_position.ToXMVector(),
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
	return m_position;
}

Vector3 Transform::GetEulerAngle(Space space)
{
	if (space == Space::LOCAL)
		return m_euler;

	if (space == Space::WORLD)
	{
		if (m_pParent != nullptr)
		{
			// ワールドクォータニオンから変換
			return GetQuaternion(Space::WORLD).ToEuler();
		}
		else
		{
			// 親がなければ保持しているオイラー角を返す
			return m_euler;
		}
	}

	return Vector3();
}

Quaternion Transform::GetQuaternion(Space space)
{
	if (space == Space::LOCAL)
		return m_quaternion;

	if (space == Space::WORLD)
	{
		if (m_pParent != nullptr)
		{
			// 再帰的に回転を求める
			return m_quaternion * m_pParent->GetQuaternion(Space::WORLD);
		}
		else
		{
			// 親がなければ終了
			return m_quaternion;
		}
	}

	return Quaternion();
}

void Transform::SetPosition(Vector3 position)
{
	m_position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

void Transform::SetScale(Vector3 scale)
{
	m_scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(Vector3(x, y, z));
}

void Transform::SetEulerAngle(Vector3 euler)
{
	m_euler = euler;
	m_quaternion = Quaternion::Euler(m_euler);
}

void Transform::SetEulerAngle(float x, float y, float z)
{
	SetEulerAngle(Vector3(x, y, z));
}

void Transform::SetQuaternion(Quaternion quaternion)
{
	m_quaternion = quaternion;
	m_euler = m_quaternion.ToEuler();
}

void Transform::SetParent(Transform* pParent)
{
	// 元の親から自身の登録削除
	if (m_pParent != nullptr)
		m_pParent->DeleteChild(this);

	// 新しい親子関係を構築
	m_pParent = pParent;
	if (m_pParent != nullptr)
		m_pParent->m_pChildren.emplace_back(this);
}

Transform* Transform::GetParent()
{
	return m_pParent;
}

std::vector<Transform*> Transform::GetChildren()
{
	return m_pChildren;
}

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX T;	// 移動行列
	DirectX::XMMATRIX S;	// スケール行列
	DirectX::XMMATRIX R;	// 回転行列

	// それぞれの変換行列を求める
	T = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	R = DirectX::XMMatrixRotationQuaternion(m_quaternion.ToXMVector());

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

void Transform::DeleteChild(Transform* child)
{
	if (m_pChildren.empty())
		return;

	auto it = std::remove(m_pChildren.begin(), m_pChildren.end(), child);
	m_pChildren.erase(it, m_pChildren.end());
}