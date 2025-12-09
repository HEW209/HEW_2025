// Material.cpp
#include <DirectX/Material.h>
#include <DirectX/ShaderManager.h>
#include <DirectX/TextureManager.h>
#include <DirectX/ConstantBufferManager.h>
#include <DirectX/ConfigManager.h>

Material::Material()
{
	// デフォルトのシェーダーとテクスチャをセット
	m_pVS = ShaderManager::Instance().LoadVertexShader(
		ConfigManager::Instance().GetString(ConfigKey::DefaultVS));

	m_pPS = ShaderManager::Instance().LoadPixelShader(
		ConfigManager::Instance().GetString(ConfigKey::DefaultPS));

	m_pTextures[0] = TextureManager::Instance().LoadTexture(
		ConfigManager::Instance().GetString(ConfigKey::DefaultTexture));
}

void Material::Bind() const
{
	// テクスチャをセット
	for (int i = 0; i < TextureSlot::Count; ++i)
	{
		if (m_pTextures[i] == nullptr)
			continue;

		m_pTextures[i]->Bind(i);
	}

	// シェーダーをセット
	if (m_pVS)
	{
		m_pVS->Bind();
	}
	if (m_pPS)
	{
		m_pPS->Bind();
	}

	// 定数バッファをセット
	ConstantBufferManager::Instance().SetCustomData(m_customParameter);

	// パイプラインステートをセット
	PipelineStateManager::Instance().SetPipelineState(m_pipelineState);
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
	if (slot >= TextureSlot::Count)
		return;

	m_pTextures[slot] = TextureManager::Instance().LoadTexture(filePath);
}

void Material::SetParameter(const void* data, UINT size)
{
	if (data == nullptr || size > CustomCBSize)
		return;

	// データコピー
	m_customParameter.clear();
	m_customParameter.resize(size);
	memcpy(m_customParameter.data(), data, size);
}

void Material::SetPipelineState(PipelineState pipelineState)
{
	m_pipelineState = pipelineState;
}
void Material::SetRasterizerState(RasterizerState rasterizerState)
{
	m_pipelineState.rasterizerState = rasterizerState;
}

void Material::SetDepthStencilState(DepthStencilState depthStencilState)
{
	m_pipelineState.depthStencilState = depthStencilState;
}

void Material::SetSamplerState(SamplerState samplerState)
{
	m_pipelineState.samplerState = samplerState;
}

void Material::SetBlendState(BlendState blendState)
{
	m_pipelineState.blendState = blendState;
}

Texture* Material::GetTexture(UINT slot)
{
	return m_pTextures[slot].get();
}
