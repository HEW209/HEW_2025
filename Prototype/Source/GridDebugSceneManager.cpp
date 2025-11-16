// GridDebugSceneManager.cpp

#include "GridDebugSceneManager.h"


GridDebugSceneManager::GridDebugSceneManager()
	: m_isPlacing(false)
	, m_isRemoving(false)
{
}

void GridDebugSceneManager::Start()
{
	m_pCurrentBlock = SceneManager::GetActiveScene()->CreateGameObject();
	m_pCurrentBlock->GetTransform()->m_position = currentBlockPos;
	m_pCurrentBlockComponent = m_pCurrentBlock->AddComponent<BlockObject>();

	m_pGridField = SceneManager::GetActiveScene()->CreateGameObject();
	m_pGridField->GetTransform()->m_position = gridFieldPos;
	m_pGridFieldComponent = m_pGridField->AddComponent<GridField>();
	m_pGridFieldComponent->SetSize({ 4, 4, 4 });
}

void GridDebugSceneManager::Update()
{
	{
		auto blockTransform = m_pCurrentBlock->GetTransform();
		if (InputManager::GetKeyDown(Input::RIGHT)) {
			blockTransform->SetQuaternion(Quaternion::Euler( 0.0f, 0.0f, -90.0f) * blockTransform->GetQuaternion());
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
	}

	if (InputManager::GetKeyDown(Input::SPACE)) {
		if (!m_isPlacing) {
			if (m_isRemoving) {
				m_pGridFieldComponent->ResetRemoveCursor();
			}
			m_isPlacing = true;
			m_cursorPos = gridFieldPos;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
	}
	else if (InputManager::GetKeyDown(Input::SHIFT)) {
		if (!m_isPlacing && !m_isRemoving) {
			m_isRemoving = true;
			m_cursorPos = gridFieldPos;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}
	}

	if (InputManager::GetKeyUp(Input::SPACE)) {
		if (m_isPlacing) {
			m_isPlacing = false;
			m_pGridFieldComponent->ResetPlaceCursor();
		}
	}
	else if (InputManager::GetKeyUp(Input::SHIFT)) {
		if (m_isRemoving) {
			m_isRemoving = false;
			m_pGridFieldComponent->ResetRemoveCursor();
		}
	}

	if (m_isPlacing)
	{
		if (InputManager::GetKeyDown(Input::A)) {
			m_cursorPos.x -= 1.0f;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
		if (InputManager::GetKeyDown(Input::D)) {
			m_cursorPos.x += 1.0f;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
		if (InputManager::GetKeyDown(Input::Q)) {
			m_cursorPos.y -= 1.0f;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
		if (InputManager::GetKeyDown(Input::E)) {
			m_cursorPos.y += 1.0f;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
		if (InputManager::GetKeyDown(Input::S)) {
			m_cursorPos.z -= 1.0f;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
		if (InputManager::GetKeyDown(Input::W)) {
			m_cursorPos.z += 1.0f;
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), m_cursorPos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}

		if (InputManager::GetKeyDown(Input::ENTER)) {
			PlaceBlock();
		}
	}
	else if (m_isRemoving)
	{
		if (InputManager::GetKeyDown(Input::A)) {
			m_cursorPos.x -= 1.0f;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}
		if (InputManager::GetKeyDown(Input::D)) {
			m_cursorPos.x += 1.0f;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}
		if (InputManager::GetKeyDown(Input::Q)) {
			m_cursorPos.y -= 1.0f;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}
		if (InputManager::GetKeyDown(Input::E)) {
			m_cursorPos.y += 1.0f;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}
		if (InputManager::GetKeyDown(Input::S)) {
			m_cursorPos.z -= 1.0f;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}
		if (InputManager::GetKeyDown(Input::W)) {
			m_cursorPos.z += 1.0f;
			m_pGridFieldComponent->SetRemoveCursor(m_cursorPos);
		}

		if (InputManager::GetKeyDown(Input::ENTER)) {
			RemoveBlock();
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

void GridDebugSceneManager::PlaceBlock()
{
	if (m_pGridFieldComponent->PlaceBlock()) {
		m_pCurrentBlockComponent->SetBlockSet(BlockSetData{});
		m_pCurrentBlock->GetTransform()->SetEulerAngle(0.0f, 0.0f, 0.0f);
	}
}

void GridDebugSceneManager::RemoveBlock()
{
	auto data = m_pGridFieldComponent->RemoveBlock();

	if (data) {
		m_pCurrentBlockComponent->SetBlockSet(data->blockSet);
		m_pCurrentBlock->GetTransform()->SetQuaternion(data->rotation);
	}
}