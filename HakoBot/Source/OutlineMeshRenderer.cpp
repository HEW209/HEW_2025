#include "OutlineMeshRenderer.h"

OutlineMeshRenderer::OutlineMeshRenderer()
	: m_outlineThickness(5.0f)
	, m_shouldDrawOutline(true)
	, m_outlineColor(1.0f, 0.0f, 0.0f, 1.0f)
{
}

void OutlineMeshRenderer::Draw()
{
	if (m_pModel == nullptr)
		return;

	// Transformからワールド行列をセット
	DirectX::XMMATRIX matrix;
	matrix = GetTransform()->GetWorldMatrix();
	ConstantBufferManager::Instance().SetWorld(matrix);

	// モデル描画処理
	m_pModel->Draw(m_materials);

	if (m_shouldDrawOutline) {
		Direct3D::Instance().ClearStencilView();
		std::vector<Material> meshMaterials, outlineMaterials;
		meshMaterials = m_materials;
		outlineMaterials.resize(m_materials.size());
		for (auto itMeshMat = meshMaterials.begin(), itOutlineMat = outlineMaterials.begin(); itMeshMat != meshMaterials.end(); ++itMeshMat, ++itOutlineMat) {
			itMeshMat->SetDepthStencilState(DepthStencilState::DISABLE_STENCIL);
			itMeshMat->SetBlendState(BlendState::DISABLE);
			itMeshMat->SetRasterizerState(RasterizerState::NONE);
			//itMeshMat->ClearPixelShader();
			itOutlineMat->SetVertexShader("Assets/Shader/Outline_VS.cso");
			itOutlineMat->SetPixelShader("Assets/Shader/Outline_PS.cso");
			struct OutlineParams {
				Color outlineColor;
				float outlineThickness;
				Vector2 viewportSize;
				float transparency;
			} params;
			params.outlineColor = m_outlineColor;
			params.outlineThickness = m_outlineThickness;
			params.viewportSize.x = Direct3D::Instance().GetViewportSizeW();
			params.viewportSize.y = Direct3D::Instance().GetViewportSizeH();
			params.transparency = 1.0f;
			itOutlineMat->SetParameter(&params, sizeof(params));
			if (IsTransparent()) {
				itOutlineMat->SetDepthStencilState(DepthStencilState::OUTLINE_READONLY);
				itOutlineMat->SetBlendState(BlendState::ALPHA);
			}
			else {
				itOutlineMat->SetDepthStencilState(DepthStencilState::OUTLINE);
				itOutlineMat->SetBlendState(BlendState::DEFAULT);
			}
			itOutlineMat->SetRasterizerState(RasterizerState::DEFAULT);
		}
		m_pModel->Draw(meshMaterials);
		ShaderManager::Instance().LoadGeometryShader("Assets/Shader/Outline_GS.cso")->Bind();
		m_pModel->Draw(outlineMaterials);
		ShaderManager::Instance().SetGeometryShader(nullptr);
	}
}

void OutlineMeshRenderer::DrawDepth()
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
		//itDepthMat->ClearPixelShader();
	}

	// モデル描画処理
	m_pModel->Draw(depthMaterials);

	//if (m_shouldDrawOutline) {
	//	Direct3D::Instance().ClearStencilView();
	//	std::vector<Material> meshMaterials, outlineMaterials;
	//	meshMaterials = m_materials;
	//	outlineMaterials.resize(m_materials.size());
	//	for (auto itMeshMat = meshMaterials.begin(), itOutlineMat = outlineMaterials.begin(); itMeshMat != meshMaterials.end(); ++itMeshMat, ++itOutlineMat) {
	//		itMeshMat->SetDepthStencilState(DepthStencilState::DISABLE_STENCIL);
	//		itMeshMat->SetBlendState(BlendState::DISABLE);
	//		itMeshMat->SetRasterizerState(RasterizerState::NONE);
	//		itMeshMat->ClearPixelShader();
	//		itOutlineMat->SetVertexShader("Assets/Shader/Outline_VS.cso");
	//		itOutlineMat->ClearPixelShader();
	//		struct OutlineParams {
	//			Color outlineColor;
	//			float outlineThickness;
	//			Vector2 viewportSize;
	//			float transparency;
	//		} params;
	//		params.outlineColor = m_outlineColor;
	//		params.outlineThickness = m_outlineThickness;
	//		params.viewportSize.x = Direct3D::Instance().GetViewportSizeW();
	//		params.viewportSize.y = Direct3D::Instance().GetViewportSizeH();
	//		params.transparency = 1.0f;
	//		itOutlineMat->SetParameter(&params, sizeof(params));
	//		itOutlineMat->SetDepthStencilState(DepthStencilState::OUTLINE);
	//		itOutlineMat->SetBlendState(BlendState::DISABLE);
	//		itOutlineMat->SetRasterizerState(RasterizerState::DEFAULT);
	//	}
	//	m_pModel->Draw(meshMaterials);
	//	ShaderManager::Instance().LoadGeometryShader("Assets/Shader/Outline_GS.cso")->Bind();
	//	m_pModel->Draw(outlineMaterials);
	//	ShaderManager::Instance().SetGeometryShader(nullptr);
	//}
}

void OutlineMeshRenderer::LoadModel(const std::string& filePath)
{
	// モデル読み込み
	m_pModel = ModelManager::Instance().Load(filePath, ModelLoadType::OUTLINE);

	// マテリアルをコピー
	m_materials = m_pModel->GetMaterials();
}