// PlaceCursor.cpp

#include "PlaceCursor.h"


void PlaceCursor::OnDestroy()
{
	for (auto&& block : m_pBlocks) {
		if (block.pObj) {
			block.pObj->Destroy();
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
		if (block.pObj) {
			block.pObj->Destroy();
		}
	}
	m_pBlocks.clear();
	m_pBlocks.reserve(m_blockSet.blocks.size());

	for (auto&& blockPos : blockSet.blocks) {
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		renderer->LoadModel("Assets/Model/SelectGrid/SelectGrid.fbx");
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->m_scale = Vector3{ 0.9f, 0.9f, 0.9f };
		auto& pos = transform->m_position;
		pos.x = blockPos.x;
		pos.y = blockPos.y;
		pos.z = blockPos.z;

		BlockObj blockObj;
		blockObj.pObj = obj;
		blockObj.pRenderer = renderer;
		m_pBlocks.push_back(blockObj);
	}
}

void PlaceCursor::SetPlaceable(bool value)
{
	if (value) {
		for (auto&& pBlock : m_pBlocks)
		{
			pBlock.pRenderer->GetMaterial(0)->SetTexture("Assets/Model/SelectGrid/Texture.png");
		}
	}
	else {
		for (auto&& pBlock : m_pBlocks)
		{
			pBlock.pRenderer->GetMaterial(0)->SetTexture("Assets/Model/SelectGrid/Texture_Out.png");
		}
	}
}
