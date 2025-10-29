//MeshRenderer.cpp
#include <Component/MeshRenderer.h>
#include <DirectX/DefaultMaterial.h>
#include <DirectX/Geometry.h>
#include <DirectX/MatrixUtil.h>
#include <DirectX/ConstantBuffer.h>

MeshRenderer::MeshRenderer()
{
	m_pMaterial = std::make_shared<DefaultMaterial>();
	m_pMesh = Geometry::Instance().GetBox();
}

void MeshRenderer::Draw()
{
	if (m_pMaterial == nullptr || m_pMesh == nullptr)
		return;

	m_pMaterial->Bind();

	//Transformからワールド行列をセット
	ConstantBuffer::Instance().SetWorld(
		MatrixUtil::CreateWorldMatrix(GetTransform())
	);

	m_pMesh->Draw();
}