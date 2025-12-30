#include "ShapeScreen.h"

static const Color g_clearShapeColor(0.2f, 0.8f, 4.0f, 1.0f);
static const Color g_defaultColor(0.0f, 0.2f, 1.0f, 1.0f);
static const Color g_redColor(2.0f, 0.0f, 0.0f, 1.0f);
static const Color g_greenColor(0.0f, 2.0f, 0.0f, 1.0f);

ShapeScreen::ShapeScreen()
	: m_isHologram(true)
{
}

void ShapeScreen::OnDestroy()
{
	for (auto&& block : m_pShapeBlocks) {
		if (block.pObj) {
			block.pObj->Destroy();
		}
	}
}

void ShapeScreen::SetClearShape(const ShapeType& shape, bool isFlip, bool isHorogram)
{
	if (shape == m_clearShape) {
		return;
	}

	m_clearShape = shape;

	UpdateClearShapeBlocks(isFlip, isHorogram);

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

void ShapeScreen::SetTransparent(bool isHologram)
{
	m_isHologram = isHologram;
	for (auto&& block : m_pShapeBlocks) {
		if (block.pRenderer) {
			block.pRenderer->SetTransparent(isHologram);
			for (auto&& material : *block.pRenderer->GetMaterials()) {
				material.SetBlendState(isHologram ? BlendState::ALPHA : BlendState::DEFAULT);
				material.SetDepthStencilState(isHologram ? DepthStencilState::READ_ONLY : DepthStencilState::DEFAULT);
				material.SetPixelShader(isHologram ? "Assets/Shader/Hologram_PS.cso" : "Assets/Shader/Screen_PS.cso");
			}
		}
	}
}

void ShapeScreen::UpdateClearShapeBlocks(bool isFlip, bool isHorogram)
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
		renderer->SetGeometry(Geometry::Type::PLANE);
		renderer->SetTransparent(m_isHologram);

		Material* mat = renderer->GetMaterial(0);
		mat->SetPixelShader(m_isHologram ? "Assets/Shader/Hologram_PS.cso" : "Assets/Shader/Screen_PS.cso");
		mat->SetDepthStencilState(m_isHologram ? DepthStencilState::READ_ONLY : DepthStencilState::DEFAULT);
		mat->SetBlendState(m_isHologram ? BlendState::ALPHA : BlendState::DEFAULT);
		mat->SetRasterizerState(m_isHologram ? RasterizerState::NONE : RasterizerState::DEFAULT);

		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetScale(1.0f, 1.0f, 1.0f);
		transform->SetEulerAngle(90.0f, 0.0f, 0.0f, Space::LOCAL);
		if (isFlip)
		{
			transform->Rotate(0.0f, 180.0f, 0.0f);
		}
		transform->SetPosition(pos,Space::LOCAL);

		Material* material = renderer->GetMaterial(0);
		Color currentColor;
		if (*it) {
			currentColor = g_clearShapeColor;
		}
		else {
			currentColor = g_defaultColor;
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
				currentColor = g_greenColor;
			}
			else {
				currentColor = g_redColor;
			}
		}
		else
		{
			if (isInside) {
				currentColor = g_clearShapeColor;
			}
			else {
				currentColor = g_defaultColor;
			}
		}

		material->SetParameter(&currentColor, sizeof(currentColor));
	}
}
