// MeshRenderer.cpp
#include <Component/MeshRenderer.h>
#include <DirectX/ModelManager.h>
#include <DirectX/ConstantBufferManager.h>
#include <GameFrame/Transform.h>
#include <GameFrame/Time.h>
#include <GameFrame/RenderSystem.h>

MeshRenderer::MeshRenderer()
	: m_shouldDrawShadow(false)
{
	m_pModel = Geometry::Instance().GetModel(Geometry::Type::BOX);
	m_materials = m_pModel->GetMaterials();
}

MeshRenderer::~MeshRenderer()
{
	if (m_shouldDrawShadow) {
		RenderSystem::Instance().UnregisterShadow(this);
	}
}

void MeshRenderer::Update()
{
	m_pModel->StepAnime(Time::GetDeltaTime());
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

void MeshRenderer::DrawShadow()
{
	if (m_pModel == nullptr)
		return;

	// Transformからワールド行列をセット
	DirectX::XMMATRIX matrix;
	matrix = GetTransform()->GetWorldMatrix();
	ConstantBufferManager::Instance().SetWorld(matrix);

	std::vector<Material> shadowMaterials;
	shadowMaterials = m_materials;
	for (auto itShadowMat = shadowMaterials.begin(); itShadowMat != shadowMaterials.end(); ++itShadowMat) {
		itShadowMat->SetBlendState(BlendState::DISABLE);
		itShadowMat->SetRasterizerState(RasterizerState::SHADOW);
		itShadowMat->ClearPixelShader();
	}

	// モデル描画処理
	m_pModel->Draw(shadowMaterials);
}

void MeshRenderer::DrawDepth()
{
	if (m_pModel == nullptr)
		return;

	// Transformからワールド行列をセット
	DirectX::XMMATRIX matrix;
	matrix = GetTransform()->GetWorldMatrix();
	ConstantBufferManager::Instance().SetWorld(matrix);

	std::vector<Material> depthMaterials;
	depthMaterials = m_materials;
	for (auto itDepthMat = depthMaterials.begin(); itDepthMat != depthMaterials.end(); ++itDepthMat) {
		itDepthMat->SetBlendState(BlendState::DISABLE);
		itDepthMat->SetDepthStencilState(DepthStencilState::DEFAULT);
		itDepthMat->ClearPixelShader();
	}

	// モデル描画処理
	m_pModel->Draw(depthMaterials);
}

void MeshRenderer::LoadModel(const std::string& filePath)
{
	// モデル読み込み
	m_pModel = ModelManager::Instance().Load(filePath);

	// マテリアルをコピー
	m_materials = m_pModel->GetMaterials();
}

void MeshRenderer::SetGeometry(Geometry::Type type)
{
	m_pModel = Geometry::Instance().GetModel(type);
	m_materials = m_pModel->GetMaterials();
}

MeshGroup::AnimeNo MeshRenderer::LoadAnimation(const std::string& filePath)
{
	return m_pModel->LoadAnimation(filePath);
}

void MeshRenderer::PlayAnime(MeshGroup::AnimeNo no, bool loop, float speed)
{
	m_pModel->PlayAnime(no, loop, speed);
}

void MeshRenderer::PlayBlend(MeshGroup::AnimeNo no, float blendTime, bool loop, float speed)
{
	m_pModel->PlayBlend(no, blendTime, loop, speed);
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

void MeshRenderer::SetShouldDrawShadow(bool shouldDrawShadow)
{
	if (m_shouldDrawShadow == shouldDrawShadow) {
		return;
	}

	if (shouldDrawShadow)
		RenderSystem::Instance().RegisterShadow(this);
	else
		RenderSystem::Instance().UnregisterShadow(this);

	m_shouldDrawShadow = shouldDrawShadow;
}
