#include "BlockObject.h"

void BlockObject::OnDestroy()
{
	for (auto&& block : m_blocks) {
		if (block) {
			block->Destroy();
		}
	}
}

void BlockObject::SetBlockSet(BlockSetData& blockSet)
{
	m_blockSet = blockSet;

	for (auto&& block : m_blocks) {
		if (block) {
			block->Destroy();
		}
	}
	m_blocks.clear();
	m_blocks.reserve(m_blockSet.blocks.size());
	
	for (auto&& blockPos : blockSet.blocks) {
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto& pos = obj->GetTransform()->m_position;
		pos.x = blockPos.x;
		pos.y = blockPos.y;
		pos.z = blockPos.z;
		m_blocks.push_back(obj);
	}
}
