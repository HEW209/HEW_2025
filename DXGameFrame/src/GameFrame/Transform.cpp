// Transform.cpp
#include <GameFrame/Transform.h>
#include <GameFrame/GameObject.h>

Transform::Transform() :
	m_localPosition(0.0f, 0.0f, 0.0f),
	m_localScale(1.0f, 1.0f, 1.0f),
	m_localQuaternion(Quaternion::identity),
	m_pParent(nullptr),
	m_pChildren{}
{
}

Transform::~Transform()
{
	// 親から自身を削除
	if (m_pParent != nullptr)
		m_pParent->DetachChild(this);

	// 子をすべて切り離す
	for (auto child : m_pChildren)
	{
		child->m_pParent = nullptr;
	}
	m_pChildren.clear();
}

Vector3 Transform::GetPosition(Space space) const
{
	if (space == Space::LOCAL || m_pParent == nullptr)
	{
		// ローカル座標をそのまま返す
		return m_localPosition;
	}
	else
	{
		// ワールド座標に変換
		DirectX::XMVECTOR worldPos = DirectX::XMVector3TransformCoord(
			m_localPosition.ToXMVector(),
			m_pParent->GetWorldMatrix()
		);

		// Vector3に変換して返す
		DirectX::XMFLOAT3 result;
		DirectX::XMStoreFloat3(&result, worldPos);
		return Vector3(result.x, result.y, result.z);
	}
}

Vector3 Transform::GetScale() const
{
	return m_localScale;
}

Vector3 Transform::GetEulerAngle(Space space) const
{
	// クォータニオンで求めてから変換する
	return GetQuaternion().ToEuler();
}

Quaternion Transform::GetQuaternion(Space space) const
{
	if (space == Space::LOCAL || m_pParent == nullptr)
	{
		// ローカル回転をそのまま返す
		return m_localQuaternion;
	}
	else
	{
		// 再帰的にワールド回転を求める
		return m_pParent->GetQuaternion(Space::WORLD) * m_localQuaternion;
	}
}

void Transform::SetPosition(Vector3 position, Space space)
{
	if (space == Space::LOCAL || m_pParent == nullptr)
	{
		// ローカル座標にそのままセット
		m_localPosition = position;
	}
	else
	{
		// ローカル座標に変換
		DirectX::XMMATRIX parentMatrix = m_pParent->GetWorldMatrix();
		DirectX::XMVECTOR localPos = DirectX::XMVector3TransformCoord(
			position.ToXMVector(),
			DirectX::XMMatrixInverse(nullptr, parentMatrix)
		);

		// Vector3に変換してセット
		DirectX::XMFLOAT3 result;
		DirectX::XMStoreFloat3(&result, localPos);
		m_localPosition = Vector3(result.x, result.y, result.z);
	}
}

void Transform::SetPosition(float x, float y, float z, Space space)
{
	SetPosition(Vector3(x, y, z), space);
}

void Transform::SetScale(Vector3 scale)
{
	m_localScale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(Vector3(x, y, z));
}

void Transform::SetEulerAngle(Vector3 euler, Space space)
{
	SetQuaternion(Quaternion::Euler(euler), space);
}

void Transform::SetEulerAngle(float x, float y, float z, Space space)
{
	SetQuaternion(Quaternion::Euler(x, y, z), space);
}

void Transform::SetQuaternion(Quaternion quaternion, Space space)
{
	if (space == Space::LOCAL || m_pParent == nullptr)
	{
		m_localQuaternion = quaternion;
	}
	else
	{
		// ローカル回転に変換してセット
		Quaternion parentQuat = m_pParent->GetQuaternion(Space::WORLD);
		m_localQuaternion = parentQuat.Inverse() * quaternion;
	}
}

void Transform::SetParent(GameObject* pParent)
{
	if (pParent == nullptr)
	{
		SetParent((Transform*)nullptr);
		return;
	}

	SetParent(pParent->GetTransform());
}

void Transform::SetParent(Transform* pParent)
{
	if (m_pParent == pParent)
		return;

	// 現在の親から切り離す
	if (m_pParent != nullptr)
		m_pParent->DetachChild(this);

	m_pParent = pParent;

	// 新しい親に追加
	if (m_pParent)
		m_pParent->m_pChildren.push_back(this);
}

Transform* Transform::GetParent() const
{
	return m_pParent;
}

Transform* Transform::GetRoot()
{
	if (m_pParent == nullptr)
	{
		return this;
	}
	else
	{
		return m_pParent->GetRoot();
	}
}

std::vector<Transform*> Transform::GetChildren() const
{
	return m_pChildren;
}

void Transform::DetachChild(Transform* child)
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(), child);
	if (it != m_pChildren.end())
		m_pChildren.erase(it);
}

void Transform::Translate(Vector3 translation, Space space)
{
	if (space == Space::LOCAL)
	{
		m_localPosition += m_localQuaternion * translation;
	}
	else
	{
		if (m_pParent == nullptr)
		{
			m_localPosition += translation;
		}
		else
		{
			m_localPosition += m_pParent->GetQuaternion(Space::WORLD).Inverse() * translation;
		}
	}
}

void Transform::Translate(float x, float y, float z, Space space)
{
	Translate(Vector3(x, y, z), space);
}

void Transform::Rotate(Vector3 euler, Space space)
{
	Rotate(euler.x, euler.y, euler.z, space);
}

void Transform::Rotate(float x, float y, float z, Space space)
{
	// 回転量を求める
	Quaternion delta = Quaternion::Euler(x, y, z);

	if (space == Space::LOCAL)
	{
		m_localQuaternion = m_localQuaternion * delta;
	}
	if (space == Space::WORLD)
	{
		if (m_pParent == nullptr)
		{
			m_localQuaternion = delta * m_localQuaternion;
		}
		else
		{
			// ローカル回転量を求めて回転
			Quaternion parentWorld = m_pParent->GetQuaternion(Space::WORLD);
			Quaternion localDelta = parentWorld.Inverse() * delta * parentWorld;
			m_localQuaternion = localDelta * m_localQuaternion;
		}
	}
}

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	DirectX::XMMATRIX T;	// 移動行列
	DirectX::XMMATRIX S;	// スケール行列
	DirectX::XMMATRIX R;	// 回転行列

	// それぞれの変換行列を求める
	T = DirectX::XMMatrixTranslationFromVector(m_localPosition.ToXMVector());
	S = DirectX::XMMatrixScalingFromVector(m_localScale.ToXMVector());
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
