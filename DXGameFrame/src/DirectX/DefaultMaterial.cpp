#include <DirectX/DefaultMaterial.h>
#include <DirectX/ShaderManager.h>
#include <DirectX/PipelineState.h>
#include <DirectX/TextureManager.h>

const static std::string g_VSPath = "Assets/Default/Default_VS.cso";
const static std::string g_PSPath = "Assets/Default/Default_PS.cso";
const static std::string g_texPath = "Assets/Default/Default.png";

DefaultMaterial::DefaultMaterial()
{
	//アセット読み込み
	m_pVS = ShaderManager::Instance().LoadVertexShader(g_VSPath);
	m_pPS = ShaderManager::Instance().LoadPixelShader(g_PSPath);
	m_pMainTexture = TextureManager::Instance().LoadTexture(g_texPath);
}

void DefaultMaterial::Bind()
{
	PipelineState::Instance().SetPipelineMode(
		RasterizerState::Mode::DEFAULT,
		DepthStencilState::Mode::DEFAULT,
		BlendState::Mode::DEFAULT,
		SamplerState::Mode::DEFAULT
	);

	m_pMainTexture->Bind();
	m_pPS->Bind();
	m_pVS->Bind();
}