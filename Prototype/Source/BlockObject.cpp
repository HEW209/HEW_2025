#include "BlockObject.h"

void BlockObject::Update()
{
	if (InputManager::GetKeyDown(Input::RIGHT))
		GetTransform()->Rotate(0, 90, 0);
	if (InputManager::GetKeyDown(Input::LEFT))
		GetTransform()->Rotate(0, -90, 0);
	if (InputManager::GetKeyDown(Input::UP))
		GetTransform()->Rotate(90, 0, 0);
	if (InputManager::GetKeyDown(Input::DOWN))
		GetTransform()->Rotate(-90, 0, 0);

	if (InputManager::GetKeyDown(Input::KEY_1)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(1);
		data.blocks[0] = { 0, 0, 0 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_2)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(2);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_3)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(3);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_4)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(4);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_5)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(4);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 0, 0, 1 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_6)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(5);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_7)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(6);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_8)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(6);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 1, 1 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_9)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(7);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		data.blocks[6] = { 0, 1, 1 };
		SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_0)) {
		GetTransform()->SetEulerAngle(0, 0, 0);

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
		SetBlockSet(data);
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
