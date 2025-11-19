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

	ShapeType clearShape(4, 4);
	clearShape.SetData({
		1,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		});
	m_pGridFieldComponent->SetClearShape(clearShape, clearShape, clearShape);
}

void GridDebugSceneManager::Update()
{
	{
		auto blockTransform = m_pCurrentBlock->GetTransform();
		if (Input::GetKeyDown(KeyCode::RIGHT)) {
			blockTransform->SetQuaternion(Quaternion::Euler( 0.0f, 0.0f, -90.0f) * blockTransform->GetQuaternion());
			if (m_isPlacing) {
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::LEFT)) {
			blockTransform->SetQuaternion(Quaternion::Euler(0.0f, 0.0f, 90.0f) * blockTransform->GetQuaternion());
			if (m_isPlacing) {
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::UP)) {
			blockTransform->SetQuaternion(Quaternion::Euler(90.0f, 0.0f, 0.0f) * blockTransform->GetQuaternion());
			if (m_isPlacing) {
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::DOWN)) {
			blockTransform->SetQuaternion(Quaternion::Euler(-90.0f, 0.0f, 0.0f) * blockTransform->GetQuaternion());
			if (m_isPlacing) {
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
	}

	if (Input::GetKeyDown(KeyCode::SPACE)) {
		if (!m_isPlacing) {
			if (m_isRemoving) {
				m_pGridFieldComponent->ResetRemoveCursor();
			}
			m_isPlacing = true;
			m_cursorPos = Vec3Int{0, 0, 0};
			auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
			m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
		}
	}
	else if (Input::GetKeyDown(KeyCode::SHIFT)) {
		if (!m_isPlacing && !m_isRemoving) {
			m_isRemoving = true;
			m_cursorPos = Vec3Int{ 0, 0, 0 };
			auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
			m_pGridFieldComponent->SetRemoveCursor(pos);
		}
	}

	if (Input::GetKeyUp(KeyCode::SPACE)) {
		if (m_isPlacing) {
			m_isPlacing = false;
			m_pGridFieldComponent->ResetPlaceCursor();
		}
	}
	else if (Input::GetKeyUp(KeyCode::SHIFT)) {
		if (m_isRemoving) {
			m_isRemoving = false;
			m_pGridFieldComponent->ResetRemoveCursor();
		}
	}

	if (m_isPlacing)
	{
		if (Input::GetKeyDown(KeyCode::A)) {
			if (m_cursorPos.x > 0) {
				m_cursorPos.x -= 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::D)) {
			if (m_cursorPos.x < m_pGridFieldComponent->GetSize().x - 1) {
				m_cursorPos.x += 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::Q)) {
			if (m_cursorPos.y > 0) {
				m_cursorPos.y -= 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::E)) {
			if (m_cursorPos.y < m_pGridFieldComponent->GetSize().y - 1) {
				m_cursorPos.y += 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::S)) {
			if (m_cursorPos.z > 0) {
				m_cursorPos.z -= 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}
		if (Input::GetKeyDown(KeyCode::W)) {
			if (m_cursorPos.z < m_pGridFieldComponent->GetSize().z - 1) {
				m_cursorPos.z += 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetPlaceCursor(m_pCurrentBlockComponent->GetBlockSet(), pos, m_pCurrentBlock->GetTransform()->GetQuaternion());
			}
		}

		if (Input::GetKeyDown(KeyCode::ENTER)) {
			PlaceBlock();
		}
	}
	else if (m_isRemoving)
	{
		if (Input::GetKeyDown(KeyCode::A)) {
			if (m_cursorPos.x > 0) {
				m_cursorPos.x -= 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetRemoveCursor(pos);
			}
		}
		if (Input::GetKeyDown(KeyCode::D)) {
			if (m_cursorPos.x < m_pGridFieldComponent->GetSize().x - 1) {
				m_cursorPos.x += 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetRemoveCursor(pos);
			}
		}
		if (Input::GetKeyDown(KeyCode::Q)) {
			if (m_cursorPos.y > 0) {
				m_cursorPos.y -= 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetRemoveCursor(pos);
			}
		}
		if (Input::GetKeyDown(KeyCode::E)) {
			if (m_cursorPos.y < m_pGridFieldComponent->GetSize().y - 1) {
				m_cursorPos.y += 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetRemoveCursor(pos);
			}
		}
		if (Input::GetKeyDown(KeyCode::S)) {
			if (m_cursorPos.z > 0) {
				m_cursorPos.z -= 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetRemoveCursor(pos);
			}
		}
		if (Input::GetKeyDown(KeyCode::W)) {
			if (m_cursorPos.z < m_pGridFieldComponent->GetSize().z - 1) {
				m_cursorPos.z += 1;
				auto pos = gridFieldPos + static_cast<Vector3>(m_cursorPos - Vec3Int(m_pGridFieldComponent->GetSize().x + 1, 0, m_pGridFieldComponent->GetSize().z + 1) / 2);
				m_pGridFieldComponent->SetRemoveCursor(pos);
			}
		}

		if (Input::GetKeyDown(KeyCode::ENTER)) {
			RemoveBlock();
		}
	}
	else
	{
		if (Input::GetKeyDown(KeyCode::KEY_1)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(1);
			data.blocks[0] = { 0, 0, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (Input::GetKeyDown(KeyCode::KEY_2)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(2);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (Input::GetKeyDown(KeyCode::KEY_3)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(3);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (Input::GetKeyDown(KeyCode::KEY_4)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(4);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 1, 1, 0 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (Input::GetKeyDown(KeyCode::KEY_5)) {
			m_pCurrentBlock->GetTransform()->SetEulerAngle(0, 0, 0);

			BlockSetData data;
			data.blocks.resize(4);
			data.blocks[0] = { 0, 0, 0 };
			data.blocks[1] = { 1, 0, 0 };
			data.blocks[2] = { 0, 1, 0 };
			data.blocks[3] = { 0, 0, 1 };
			m_pCurrentBlockComponent->SetBlockSet(data);
		}
		if (Input::GetKeyDown(KeyCode::KEY_6)) {
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
		if (Input::GetKeyDown(KeyCode::KEY_7)) {
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
		if (Input::GetKeyDown(KeyCode::KEY_8)) {
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
		if (Input::GetKeyDown(KeyCode::KEY_9)) {
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
		if (Input::GetKeyDown(KeyCode::KEY_0)) {
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