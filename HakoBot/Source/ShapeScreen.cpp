#include "ShapeScreen.h"


void ShapeScreen::OnDestroy()
{
	for (auto&& block : m_pShapeBlocks) {
		if (block.pObj) {
			block.pObj->Destroy();
		}
	}
}

void ShapeScreen::SetClearShape(const ShapeType& shape, bool isHorogram)
{
	if (shape == m_clearShape) {
		return;
	}

	m_clearShape = shape;

	UpdateClearShapeBlocks(isHorogram);

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

void ShapeScreen::UpdateClearShapeBlocks(bool isHorogram)
{
	auto size = m_clearShape.GetSize();
	Vector3 sizeFloat = static_cast<Vector3>(size);
	Vector3 sizeHalf = sizeFloat * 0.5f;
	sizeHalf.z = 0.0f;

	for (auto&& block : m_pShapeBlocks) {
		if (block.pObj) {
			block.pObj->Destroy();
		}
	}
	m_pShapeBlocks.clear();
	m_pShapeBlocks.reserve(m_clearShape.GetTotalElements());

	for (auto it = m_clearShape.begin(); it != m_clearShape.end(); ++it) {
		auto coord = m_clearShape.GetCoordinates(it);
		Vector3 pos = static_cast<Vector3>(coord) - sizeHalf + Vector3{ 0.5f, 0.5f, 0.0f };

		bool isInside = m_clearShape(coord);

		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		renderer->SetTransparent(true);

		Material* mat = renderer->GetMaterial(0);
		mat->SetPixelShader("Assets/Shader/Hologram_PS.cso");
		mat->SetDepthStencilState(DepthStencilState::READ_ONLY);
		mat->SetBlendState(BlendState::ALPHA);
		mat->SetRasterizerState(RasterizerState::NONE);

		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetScale(1.0f, 1.0f, 0.2f);
		transform->SetPosition(pos,Space::LOCAL);

		Material* material = renderer->GetMaterial(0);
		Color currentColor;
		if (*it) {
			currentColor = Color(0.0f, 0.6f, 3.0f, 0.6f);
		}
		else {
			currentColor = Color(0.0f, 0.2f, 1.0f, 0.6f);
		}
		mat->SetParameter(&currentColor, sizeof(currentColor));

		ShapeBlock shapeBlock;
		shapeBlock.pObj = obj;
		shapeBlock.pRenderer = renderer;

		m_pShapeBlocks.push_back(shapeBlock);
	}
}

void ShapeScreen::UpdateCurrentShapeBlocks()
{
	auto size = m_currentShape.GetSize();
	Vector3 sizeFloat = static_cast<Vector3>(size);
	Vector3 sizeHalf = sizeFloat * 0.5f;
	sizeHalf.z = 0.0f;

	for (auto it = m_currentShape.begin(); it != m_currentShape.end(); ++it)
	{
		auto coord = m_currentShape.GetCoordinates(it);
		bool isInside = m_clearShape(coord);

		auto renderer = m_pShapeBlocks[coord.x + coord.y * size.x].pRenderer;
		Material* material = renderer->GetMaterial(0);
		Color currentColor;

		if (*it)
		{
			if (isInside) {
				currentColor = Color(0.0f, 2.0f, 0.0f, 0.6f);
			}
			else {
				currentColor = Color(2.0f, 0.0f, 0.0f, 0.6f);
			}
		}
		else
		{
			if (isInside) {
				currentColor = Color(0.0f, 0.6f, 3.0f, 0.6f);
			}
			else {
				currentColor = Color(0.0f, 0.2f, 1.0f, 0.6f);
			}
		}

		material->SetParameter(&currentColor, sizeof(currentColor));
	}
}
