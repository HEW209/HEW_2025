//ShaderManager.cpp
#include <DirectX/ShaderManager.h>
#include <DirectX/Direct3D.h>

ShaderManager::ShaderManager() :
	m_pCurrentVS(nullptr),
	m_pCurrentPS(nullptr)
{

}

std::shared_ptr<VertexShader> ShaderManager::LoadVertexShader(const std::string& filePath)
{
	//頂点シェーダー検索
	auto it = m_vertexShaders.find(filePath);
	if (it != m_vertexShaders.end())
	{
		//既存のシェーダーを返す
		return it->second;		
	}

	//新規シェーダー読み込み・追加
	auto newVS = std::make_shared<VertexShader>();
	newVS->Load(filePath);
	m_vertexShaders[filePath] = newVS;
	return newVS;
}

std::shared_ptr<PixelShader> ShaderManager::LoadPixelShader(const std::string& filePath)
{
	//ピクセルシェーダー検索
	auto it = m_pixelShaders.find(filePath);
	if (it != m_pixelShaders.end())
	{
		//既存のシェーダーを返す
		return it->second;
	}

	//新規シェーダー読み込み・追加
	auto newPS = std::make_shared<PixelShader>();
	newPS->Load(filePath);
	m_pixelShaders[filePath] = newPS;
	return newPS;
}

void ShaderManager::SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pInputLayout)
{
	if (pVS == nullptr)
		return;

	if (m_pCurrentVS == pVS)
		return;

	//シェーダーをセット
	m_pCurrentVS = pVS;
	Direct3D::Instance().GetContext()->VSSetShader(pVS, nullptr, 0);
	Direct3D::Instance().GetContext()->IASetInputLayout(pInputLayout);
}

void ShaderManager::SetPixelShader(ID3D11PixelShader* pPS)
{
	if (pPS == nullptr)
		return;

	if (m_pCurrentPS == pPS)
		return;

	//シェーダーをセット
	m_pCurrentPS = pPS;
	Direct3D::Instance().GetContext()->PSSetShader(pPS, nullptr, 0);
}

void ShaderManager::Clear()
{
	m_vertexShaders.clear();
	m_pixelShaders.clear();
	m_pCurrentVS = nullptr;
	m_pCurrentPS = nullptr;
}