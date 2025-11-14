#include "GridDebugSceneManager.h"


GridDebugSceneManager::GridDebugSceneManager()
	: m_gridData(2, 2, 2)
{
}

void GridDebugSceneManager::Start()
{
	m_pCurrentBlock = SceneManager::GetActiveScene()->CreateGameObject();
	m_pCurrentBlockComponent = m_pCurrentBlock->AddComponent<BlockObject>();
	m_pCurrentBlock->GetTransform()->m_position = currentBlockPos;
}

void GridDebugSceneManager::Update()
{
	if (InputManager::GetKeyDown(Input::RIGHT))
		m_pCurrentBlock->GetTransform()->Rotate(0, 0, 90);
	if (InputManager::GetKeyDown(Input::LEFT))
		m_pCurrentBlock->GetTransform()->Rotate(0, 0, -90);
	if (InputManager::GetKeyDown(Input::UP))
		m_pCurrentBlock->GetTransform()->Rotate(90, 0, 0);
	if (InputManager::GetKeyDown(Input::DOWN))
		m_pCurrentBlock->GetTransform()->Rotate(-90, 0, 0);


	if (InputManager::GetKeyHold(Input::SPACE))
	{
		if (InputManager::GetKeyDown(Input::KEY_1)) {
			PlaceBlock({ 0, 0, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_2)) {
			PlaceBlock({ 1, 0, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_3)) {
			PlaceBlock({ 0, 0, 1 });
		}
		if (InputManager::GetKeyDown(Input::KEY_4)) {
			PlaceBlock({ 1, 0, 1 });
		}
		if (InputManager::GetKeyDown(Input::KEY_5)) {
			PlaceBlock({ 0, 1, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_6)) {
			PlaceBlock({ 1, 1, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_7)) {
			PlaceBlock({ 0, 1, 1 });
		}
		if (InputManager::GetKeyDown(Input::KEY_8)) {
			PlaceBlock({ 1, 1, 1 });
		}
	}
	else if (InputManager::GetKeyHold(Input::SHIFT))
	{
		if (InputManager::GetKeyDown(Input::KEY_1)) {
			RemoveBlock({ 0, 0, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_2)) {
			RemoveBlock({ 1, 0, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_3)) {
			RemoveBlock({ 0, 0, 1 });
		}
		if (InputManager::GetKeyDown(Input::KEY_4)) {
			RemoveBlock({ 1, 0, 1 });
		}
		if (InputManager::GetKeyDown(Input::KEY_5)) {
			RemoveBlock({ 0, 1, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_6)) {
			RemoveBlock({ 1, 1, 0 });
		}
		if (InputManager::GetKeyDown(Input::KEY_7)) {
			RemoveBlock({ 0, 1, 1 });
		}
		if (InputManager::GetKeyDown(Input::KEY_8)) {
			RemoveBlock({ 1, 1, 1 });
		}
	}
	else
	{
		if (InputManager::GetKeyDown(Input::KEY_1)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(1);
			data.blocks[0] = { 0, 0, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_2)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(2);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_3)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(3);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_4)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(4);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_5)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(4);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 0, 0, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_6)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(5);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			data.blocks[4] = { 0, 0, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_7)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(6);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			data.blocks[4] = { 0, 0, 1 };
			data.blocks[5] = { 1, 0, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_8)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(6);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			data.blocks[4] = { 0, 0, 1 };
			data.blocks[5] = { 1, 1, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_9)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(7);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			data.blocks[4] = { 0, 0, 1 };
			data.blocks[5] = { 1, 0, 1 };
			data.blocks[6] = { 0, 1, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (InputManager::GetKeyDown(Input::KEY_0)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(8);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			data.blocks[4] = { 0, 0, 1 };
			data.blocks[5] = { 1, 0, 1 };
			data.blocks[6] = { 0, 1, 1 };
			data.blocks[7] = { 1, 1, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
	}
}

void GridDebugSceneManager::PlaceBlock(Vec3Int position)
{
	auto blockId = m_gridData.PlaceBlock(m_pCurrentBlockComponent->GetBlockSet(), position, m_pCurrentBlock->GetTransform()->GetQuaternion());

	if (!blockId) {
		return;
	}

	Vector3 pos = gridFieldPos;
	pos.x += position.x;
	pos.y += position.y;
	pos.z += position.z;
	
	m_pCurrentBlock->GetTransform()->m_position = pos;

	if (m_pPlacedBlocks.size() < blockId) {
		m_pPlacedBlocks.resize(blockId);
	}

	m_pPlacedBlocks[blockId - 1] = m_pCurrentBlock;

	m_pCurrentBlock = SceneManager::GetActiveScene()->CreateGameObject();
	m_pCurrentBlockComponent = m_pCurrentBlock->AddComponent<BlockObject>();
	m_pCurrentBlock->GetTransform()->m_position = currentBlockPos;
}

void GridDebugSceneManager::RemoveBlock(Vec3Int position)
{
	auto blockId = m_gridData.RemoveBlock(position);

	if (!blockId) {
		return;
	}

	m_pCurrentBlock->Destroy();
	m_pCurrentBlock = m_pPlacedBlocks[blockId - 1];
	m_pCurrentBlockComponent = m_pCurrentBlock->GetComponent<BlockObject>();
	m_pPlacedBlocks[blockId - 1] = nullptr;

	m_pCurrentBlock->GetTransform()->m_position = currentBlockPos;
}