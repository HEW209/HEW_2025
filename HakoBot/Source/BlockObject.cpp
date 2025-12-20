// BlockObject.cpp

#include "BlockObject.h"
#include <Component/Collider.h>

void BlockObject::Awake()
{
	m_pBlockMeshRenderer = GetGameObject()->AddComponent<OutlineMeshRenderer>();
	m_pBlockMeshRenderer->SetEnabled(false);
	m_pBlockMeshRenderer->SetShouldDrawOutline(false);
	m_pBlockMeshRenderer->SetOutlineColor(Color{1.0f, 1.0f, 0.0f});
	m_pBlockMeshRenderer->SetOutlineThickness(5.0f);
}

void BlockObject::OnDestroy()
{
	for (auto&& block : m_pBlocks) {
		if (block) {
			block->Destroy();
		}
	}
}

void BlockObject::SetBlockSet(const BlockSetData& blockSet)
{
	m_blockSet = blockSet;

	for (auto&& block : m_pBlocks) {
		if (block) {
			block->Destroy();
		}
	}
	m_pBlocks.clear();
	m_pBlocks.reserve(m_blockSet.blocks.size());

	Vector3 min{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	Vector3 max{ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
	for (auto&& blockPos : blockSet.blocks) {
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		if (m_shouldUseCollider)
		{
			obj->AddComponent<Collider>();
		}

		// ブロックの最小位置を記録
		min.x = std::min(min.x, (float)blockPos.x);
		min.y = std::min(min.y, (float)blockPos.y);
		min.z = std::min(min.z, (float)blockPos.z);

		// ブロックの最大位置を記録
		max.x = std::max(max.x, (float)blockPos.x);
		max.y = std::max(max.y, (float)blockPos.y);
		max.z = std::max(max.z, (float)blockPos.z);

		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetPosition(blockPos.x, blockPos.y, blockPos.z, Space::LOCAL);
	
		m_pBlocks.push_back(obj);
	}
	m_size = max - min + Vector3(1.0f, 1.0f, 1.0f);
	m_center = (min + max) * 0.5f;
}

void BlockObject::SetModel(const std::string& modelPath)
{
	m_modelPath = modelPath;
	if (modelPath.empty()) {
		m_pBlockMeshRenderer->SetEnabled(false);
	}
	else {
		m_pBlockMeshRenderer->LoadModel(modelPath);
		m_pBlockMeshRenderer->SetEnabled(true);
	}
}

void BlockObject::SetSelect(bool isSelected)
{
	m_pBlockMeshRenderer->SetShouldDrawOutline(isSelected);
}

float BlockObject::GetGroundYOffset()
{
	if (m_blockSet.blocks.empty()) {
		return 0.0f;
	}

	// ブロック（立方体）の中心から見た8つの頂点へのオフセット
	const std::vector<Vector3> cornerOffsets = {
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f }
	};

	float minY = (std::numeric_limits<float>::max)();

	const Quaternion rotation = GetTransform()->GetQuaternion();

	for (const auto& blockPos : m_blockSet.blocks) {
		// ブロックの中心座標
		const Vector3 center(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z));

		for (const auto& offset : cornerOffsets) {
			// ブロックの頂点座標
			Vector3 corner = center + offset;

			Vector3 rotatedCorner = rotation * corner;

			if (rotatedCorner.y < minY) {
				minY = rotatedCorner.y;
			}
		}
	}

	return (minY == (std::numeric_limits<float>::max)()) ? 0.0f : -minY;
}

Vector3 BlockObject::GetGroundOffset()
{
	const float yOffset = GetGroundYOffset();

	return Vector3(0.0f, yOffset, 0.0f);
}

bool BlockObject::IsInside(const Vector3& worldPosition)
{
	Transform* pTransform = GetTransform();
	if (!pTransform)
	{
		return false;
	}

	// このBlockObjectの逆ワールド行列を取得する
	DirectX::XMMATRIX worldMatrix = pTransform->GetWorldMatrix();
	DirectX::XMMATRIX inverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);

	DirectX::XMVECTOR worldPosVec = worldPosition.ToXMVector();

	// 座標をローカル空間に変換する
	DirectX::XMVECTOR localPosVec = DirectX::XMVector3TransformCoord(worldPosVec, inverseWorldMatrix);

	DirectX::XMFLOAT3 localPosFloat3;
	DirectX::XMStoreFloat3(&localPosFloat3, localPosVec);
	Vector3 localPosition(localPosFloat3.x, localPosFloat3.y, localPosFloat3.z);

	// ローカル座標で、各ブロックのAABBと当たり判定を行う
	for (const auto& blockPos : m_blockSet.blocks)
	{
		const float centerX = static_cast<float>(blockPos.x);
		const float centerY = static_cast<float>(blockPos.y);
		const float centerZ = static_cast<float>(blockPos.z);

		// AABB
		const float minX = centerX - 0.5f;
		const float maxX = centerX + 0.5f;
		const float minY = centerY - 0.5f;
		const float maxY = centerY + 0.5f;
		const float minZ = centerZ - 0.5f;
		const float maxZ = centerZ + 0.5f;

		// ローカル座標がAABBの内側にあるかチェック
		if (localPosition.x >= minX && localPosition.x <= maxX &&
			localPosition.y >= minY && localPosition.y <= maxY &&
			localPosition.z >= minZ && localPosition.z <= maxZ)
		{
			return true;
		}
	}

	return false;
}
