//Material.cpp
#include <DirectX/Material.h>
#include <DirectX/TextureManager.h>

void Material::SetTexture(const std::string& filePath)
{
	m_pMainTexture = TextureManager::Instance().LoadTexture(filePath);
}
