//Material.cpp
#include <DirectX/Material.h>
#include <DirectX/TextureManager.h>
#include <DirectX/ShaderManager.h>

Material::Material()
{
	m_pTextures[0] = TextureManager::Instance().LoadTexture("Assets/Default/Default.png");
	m_pVS = ShaderManager::Instance().LoadVertexShader("Assets/Default/Default_VS.cso");
	m_pPS = ShaderManager::Instance().LoadPixelShader("Assets/Default/Default_PS.cso");
}

void Material::Bind() const
{
	// テクスチャをセット
	for (int i = 0; i < MaxTextureSlot; ++i)
	{
		if (m_pTextures[i] == nullptr)
			continue;

		m_pTextures[i]->Bind();
	}

	// シェーダーをセット
	m_pVS->Bind();
	m_pPS->Bind();

	// 定数バッファをセット


	// パイプラインステートをセット
	PipelineState::Instance().SetPipelineMode(m_pipelineMode);
}

void Material::SetVertexShader(const std::string& filePath)
{
	m_pVS = ShaderManager::Instance().LoadVertexShader(filePath);
}

void Material::SetPixelShader(const std::string& filePath)
{
	m_pPS = ShaderManager::Instance().LoadPixelShader(filePath);
}

void Material::SetTexture(const std::string& filePath, UINT slot)
{
	m_pTextures[slot] = TextureManager::Instance().LoadTexture(filePath);
}

void Material::SetParameter(const void* data, UINT size)
{
	if (data == nullptr)
		return;

	// データコピー
	m_customParameter.clear();
	m_customParameter.resize(size);
	memcpy(m_customParameter.data(), data, size);
}