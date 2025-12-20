// ShaderManager.cpp
#include <DirectX/ShaderManager.h>
#include <DirectX/Direct3D.h>

ShaderManager::ShaderManager() :
	m_pCurrentVS(nullptr),
	m_pCurrentPS(nullptr)
{

}

std::shared_ptr<VertexShader> ShaderManager::LoadVertexShader(const std::string& filePath)
{
	// 頂点シェーダー検索
	auto it = m_pVertexShaders.find(filePath);
	if (it != m_pVertexShaders.end())
	{
		// 既存のシェーダーを返す
		return it->second;
	}

	// 新規シェーダー読み込み・追加
	auto newVS = std::make_shared<VertexShader>();
	newVS->Load(filePath);
	m_pVertexShaders[filePath] = newVS;
	return newVS;
}

std::shared_ptr<PixelShader> ShaderManager::LoadPixelShader(const std::string& filePath)
{
	// ピクセルシェーダー検索
	auto it = m_pPixelShaders.find(filePath);
	if (it != m_pPixelShaders.end())
	{
		// 既存のシェーダーを返す
		return it->second;
	}

	// 新規シェーダー読み込み・追加
	auto newPS = std::make_shared<PixelShader>();
	newPS->Load(filePath);
	m_pPixelShaders[filePath] = newPS;
	return newPS;
}

std::shared_ptr<GeometryShader> ShaderManager::LoadGeometryShader(const std::string& filePath)
{
	// ジオメトリシェーダー検索
	auto it = m_pGeometryShaders.find(filePath);
	if (it != m_pGeometryShaders.end())
	{
		// 既存のシェーダーを返す
		return it->second;
	}

	// 新規シェーダー読み込み・追加
	auto newGS = std::make_shared<GeometryShader>();
	newGS->Load(filePath);
	m_pGeometryShaders[filePath] = newGS;
	return newGS;
}

void ShaderManager::SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pInputLayout)
{
	if (pVS == nullptr || pInputLayout == nullptr || pVS == m_pCurrentVS)
		return;

	// シェーダーをセット
	m_pCurrentVS = pVS;
	Direct3D::Instance().GetContext()->VSSetShader(pVS, nullptr, 0);
	Direct3D::Instance().GetContext()->IASetInputLayout(pInputLayout);
}

void ShaderManager::SetPixelShader(ID3D11PixelShader* pPS)
{
	if (pPS == nullptr || pPS == m_pCurrentPS)
		return;

	// シェーダーをセット
	m_pCurrentPS = pPS;
	Direct3D::Instance().GetContext()->PSSetShader(pPS, nullptr, 0);
}

void ShaderManager::SetGeometryShader(ID3D11GeometryShader* pGS)
{
	if (pGS == m_pCurrentGS)
		return;

	// シェーダーをセット
	m_pCurrentGS = pGS;
	Direct3D::Instance().GetContext()->GSSetShader(pGS, nullptr, 0);
}

void ShaderManager::Clear()
{
	m_pVertexShaders.clear();
	m_pPixelShaders.clear();
	m_pCurrentVS = nullptr;
	m_pCurrentPS = nullptr;
}

ShaderManager& ShaderManager::Instance()
{
	static ShaderManager s_instance;
	return s_instance;
}
