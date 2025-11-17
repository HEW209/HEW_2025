#include "PlayerBlockHandler.h"
#include "PlayerBlockDebugSceneManager.h"


void PlayerBlockHandler::Start()
{
	
}

void PlayerBlockHandler::Update()
{
	auto blockTransform = m_pBlockObject->GetTransform();
	auto playerTransform = GetTransform();

	Vector3 blockOffset{0.0f, 2.0f, 0.0f};
	blockTransform->SetPosition(playerTransform->GetPosition() + blockOffset);

	

	if (InputManager::GetKeyDown(Input::RIGHT)) {
		blockTransform->SetQuaternion(Quaternion::Euler(0.0f, 0.0f, -90.0f) * blockTransform->GetQuaternion());

	}
	if (InputManager::GetKeyDown(Input::LEFT)) {
		blockTransform->SetQuaternion(Quaternion::Euler(0.0f, 0.0f, 90.0f) * blockTransform->GetQuaternion());

	}
	if (InputManager::GetKeyDown(Input::UP)) {
		blockTransform->SetQuaternion(Quaternion::Euler(90.0f, 0.0f, 0.0f) * blockTransform->GetQuaternion());

	}
	if (InputManager::GetKeyDown(Input::DOWN)) {
		blockTransform->SetQuaternion(Quaternion::Euler(-90.0f, 0.0f, 0.0f) * blockTransform->GetQuaternion());

	}


	if (InputManager::GetKeyDown(Input::KEY_1)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(1);
		data.blocks[0] = { 0, 0, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_2)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(2);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_3)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(3);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_4)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(4);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_5)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(4);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 0, 0, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_6)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(5);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_7)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(6);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_8)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(6);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 1, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_9)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

		BlockSetData data;
		data.blocks.resize(7);
		data.blocks[0] = { 0, 0, 0 };
		data.blocks[1] = { 1, 0, 0 };
		data.blocks[2] = { 0, 1, 0 };
		data.blocks[3] = { 1, 1, 0 };
		data.blocks[4] = { 0, 0, 1 };
		data.blocks[5] = { 1, 0, 1 };
		data.blocks[6] = { 0, 1, 1 };
		m_pBlockObject->SetBlockSet(data);
	}
	if (InputManager::GetKeyDown(Input::KEY_0)) {
		m_pBlockObject->GetTransform()->SetEulerAngle(0, 0, 0);

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
		m_pBlockObject->SetBlockSet(data);
	}
	//プレイヤーからのブロックを置く相対座標
	Vector3 placeCursorOffset{ 0.0f, 0.5f, -2.0f };
	
	Vector3 placeCursorPos = playerTransform->GetPosition() + playerTransform->GetQuaternion() * placeCursorOffset; 

	PlayerBlockDebugSceneManager::GetInstance()->GetGridField()->
		SetPlaceCursor(m_pBlockObject->GetBlockSet(), playerTransform->GetPosition(), blockTransform->GetQuaternion());

}

void PlayerBlockHandler::TryPlaceBlock()
{

	CheckPlaceBlock();
}

void PlayerBlockHandler::SetBlockObject(BlockObject* pBlockObject)
{
	m_pBlockObject = pBlockObject;
}

bool PlayerBlockHandler::CheckPlaceBlock()
{
	
	return false;
}
