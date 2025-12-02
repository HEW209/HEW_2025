#include "ShapeScreen.h"


void ShapeScreen::OnDestroy()
{
	for (auto&& block : m_pClearShapeBlocks) {
		if (block) {
			block->Destroy();
		}
	}

	for (auto&& block : m_pCurrentShapeBlocks) {
		if (block) {
			block->Destroy();
		}
	}
}

void ShapeScreen::SetClearShape(const ShapeType& shape)
{
	if (shape == m_clearShape) {
		return;
	}

	m_clearShape = shape;

	UpdateClearShapeBlocks();

	if (m_clearShape.GetSize() != m_currentShape.GetSize()) {
		m_currentShape = ShapeType(m_clearShape.GetSize());
		UpdateCurrentShapeBlocks();
	}
}

void ShapeScreen::SetCurrentShape(const ShapeType& shape)
{
	if (shape.GetSize() != m_clearShape.GetSize()) {
		return;
	}

	if (shape == m_currentShape) {
		return;
	}

	m_currentShape = shape;

	UpdateCurrentShapeBlocks();
}

bool ShapeScreen::IsClear()
{
	return m_clearShape == m_currentShape;
}

void ShapeScreen::UpdateClearShapeBlocks()
{
	auto size = m_clearShape.GetSize();
	Vector3 sizeFloat = static_cast<Vector3>(size);
	Vector3 sizeHalf = sizeFloat * 0.5f;
	sizeHalf.z = 0.0f;

	for (auto&& block : m_pClearShapeBlocks) {
		if (block) {
			block->Destroy();
		}
	}
	m_pClearShapeBlocks.clear();
	m_pClearShapeBlocks.reserve(m_clearShape.GetTotalElements());

	for (auto it = m_clearShape.begin(); it != m_clearShape.end(); ++it) {
		auto coord = m_clearShape.GetCoordinates(it);
		Vector3 pos = static_cast<Vector3>(coord) - sizeHalf + Vector3{ 0.5f, 0.5f, 0.0f };

		bool isInside = m_clearShape(coord);

		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();

		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetScale(1.0f, 1.0f, 0.1f);
		transform->SetPosition(pos,Space::LOCAL);

		Material material = renderer->GetMaterial(0);

		if (*it) {
			material.SetTexture("Assets/Textures/White.png");
		}
		else {
			material.SetTexture("Assets/Textures/Gray.png");
		}

		renderer->SetMaterial(material, 0);

		m_pClearShapeBlocks.push_back(obj);
	}
}

void ShapeScreen::UpdateCurrentShapeBlocks()
{
	auto size = m_currentShape.GetSize();
	Vector3 sizeFloat = static_cast<Vector3>(size);
	Vector3 sizeHalf = sizeFloat * 0.5f;
	sizeHalf.z = 0.0f;
	
	bool isClear = IsClear();

	for (auto&& block : m_pCurrentShapeBlocks) {
		if (block) {
			block->Destroy();
		}
	}
	m_pCurrentShapeBlocks.clear();
	m_pCurrentShapeBlocks.reserve(m_currentShape.GetTotalElements());

	for (auto it = m_currentShape.begin(); it != m_currentShape.end(); ++it) {
		if (*it) {
			auto coord = m_currentShape.GetCoordinates(it);
			Vector3 pos = static_cast<Vector3>(coord) - sizeHalf + Vector3{ 0.5f, 0.5f, 0.0f };

			bool isInside = m_clearShape(coord);
			
			auto obj = SceneManager::GetActiveScene()->CreateGameObject();
			auto renderer = obj->AddComponent<MeshRenderer>();

			Material material = renderer->GetMaterial(0);

			if (isClear) {
				material.SetTexture("Assets/Textures/Green.png");
			}
			else {
				if (isInside) {
					material.SetTexture("Assets/Textures/Cyan.png");
				}
				else {
					material.SetTexture("Assets/Textures/Red.png");
				}
			}
			renderer->SetMaterial(material, 0);

			auto transform = obj->GetTransform();
			transform->SetParent(GetTransform());
			transform->SetScale(0.8f, 0.8f, 0.2f);
			transform->SetPosition(pos,Space::LOCAL);

			m_pCurrentShapeBlocks.push_back(obj);
		}
	}
}
