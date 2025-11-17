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
