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
		transform->SetScale(0.9f, 0.9f, 0.9f);
		transform->SetPosition(blockPos.x, blockPos.y, blockPos.z,Space::LOCAL);

		BlockObj blockObj;
		blockObj.pObj = obj;
		blockObj.pRenderer = renderer;
		m_pBlocks.push_back(blockObj);
	}
}

void PlaceCursor::SetPlaceable(bool value)
{
	Material material;
	if (value) {
		for (auto&& pBlock : m_pBlocks)
		{
			material = pBlock.pRenderer->GetMaterial(0);
			material.SetTexture("Assets/Model/SelectGrid/Texture.png");
			pBlock.pRenderer->SetMaterial(material,0);
		}
	}
	else {
		for (auto&& pBlock : m_pBlocks)
		{
			material = pBlock.pRenderer->GetMaterial(0);
			material.SetTexture("Assets/Model/SelectGrid/Texture_Out.png");
			pBlock.pRenderer->SetMaterial(material, 0);
		}
	}
}
