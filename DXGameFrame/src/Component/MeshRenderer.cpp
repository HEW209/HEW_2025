// MeshRenderer.cpp
#include <Component/MeshRenderer.h>
#include <DirectX/ModelManager.h>
#include <DirectX/ConstantBufferManager.h>
#include <GameFrame/Transform.h>
#include <DirectX/Geometry.h>

MeshRenderer::MeshRenderer()
{
	m_pModel = Geometry::Instance().GetModel(Geometry::Type::BOX);
	m_materials = m_pModel->GetMaterials();
}

void MeshRenderer::Draw()
{
	if (m_pModel == nullptr)
		return;

	// Transformからワールド行列をセット
	DirectX::XMMATRIX matrix;
	matrix = GetTransform()->GetWorldMatrix();
	ConstantBufferManager::Instance().SetWorld(matrix);

	// モデル描画処理
	m_pModel->Draw(m_materials);
}

void MeshRenderer::LoadModel(const std::string& filePath)
{
	// モデル読み込み
	m_pModel = ModelManager::Instance().Load(filePath);

	// マテリアルをコピー
	m_materials = m_pModel->GetMaterials();
}

UINT MeshRenderer::GetMaterialNum()
{
	return (UINT)m_materials.size();
}

Material* MeshRenderer::GetMaterial(UINT slot)
{
	if (slot < 0 || slot >= m_materials.size())
		return nullptr;

	return &m_materials[slot];
}

std::vector<Material>* MeshRenderer::GetMaterials()
{
	return &m_materials;
}