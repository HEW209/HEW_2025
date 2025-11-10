//MeshRenderer.cpp
#include <Component/MeshRenderer.h>
#include <DirectX/ModelManager.h>
#include <DirectX/ConstantBuffer.h>
#include <DirectX/MatrixUtil.h>

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::Draw()
{
	if (m_pModel == nullptr)
		return;

	// Transformからワールド行列をセット
	ConstantBuffer::Instance().SetWorld(
		MatrixUtil::CreateWorldMatrix(GetTransform())
	);

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

Material* MeshRenderer::GetMaterial(UINT slot)
{
	if (slot < 0 || slot >= m_materials.size())
		return nullptr;

	return &m_materials[slot];
}

void MeshRenderer::SetMaterial(Material* pMaterial, UINT slot)
{
	if (pMaterial == nullptr)
		return;

	if (slot < 0 || slot >= m_materials.size())
		return;

	// マテリアルをコピー
	m_materials[slot] = *pMaterial;
}