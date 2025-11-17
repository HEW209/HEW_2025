// PlaceCursor.cpp

#include "PlaceCursor.h"


void PlaceCursor::OnDestroy()
{
	for (auto&& block : m_pBlocks) {
		if (block) {
			block->Destroy();
		}
	}
}

void PlaceCursor::SetBlockSet(const BlockSetData& blockSet)
{
	if (m_blockSet.blocks == blockSet.blocks) {
		return;
	}
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
		transform->m_scale = Vector3{ 0.2f, 0.2f, 0.2f };
		auto& pos = transform->m_position;
		pos.x = blockPos.x;
		pos.y = blockPos.y;
		pos.z = blockPos.z;
		m_pBlocks.push_back(obj);
	}
}

void PlaceCursor::SetPlaceable(bool value)
{
	if (value) {
		for (auto&& pBlock : m_pBlocks)
		{
			pBlock->GetTransform()->m_scale = Vector3{ 0.3f, 0.3f, 0.3f };
		}
	}
	else {
		for (auto&& pBlock : m_pBlocks)
		{
			pBlock->GetTransform()->m_scale = Vector3{ 0.2f, 0.2f, 0.2f };
		}
	}
}
