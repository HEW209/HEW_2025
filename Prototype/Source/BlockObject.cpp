// BlockObject.cpp

#include "BlockObject.h"

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
	
	for (auto&& blockPos : blockSet.blocks) {
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		auto& pos = transform->m_position;
		pos.x = blockPos.x;
		pos.y = blockPos.y;
		pos.z = blockPos.z;
		m_pBlocks.push_back(obj);
	}
}

void BlockObject::SetSelect(bool value)
{
	if (value) {
		for (auto && pBlock : m_pBlocks)
		{
			pBlock->GetTransform()->m_scale = Vector3{ 0.9f, 0.9f, 0.9f };
		}
	}
	else {
		for (auto&& pBlock : m_pBlocks)
		{
			pBlock->GetTransform()->m_scale = Vector3{ 1.0f, 1.0f, 1.0f };
		}
	}
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