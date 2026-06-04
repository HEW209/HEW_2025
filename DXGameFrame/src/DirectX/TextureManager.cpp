// TextureManager.cpp
#include <DirectX/TextureManager.h>

std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& filePath)
{
	// テクスチャ検索
	auto it = m_textures.find(filePath);
	if (it != m_textures.end())
	{
		// 既存のテクスチャを返す
		return it->second;
	}

	// 新規テクスチャ読み込み・追加
	auto newTex = std::make_shared<Texture>();
	if (FAILED(newTex->Load(filePath)))
	{
		return nullptr;
	}
	m_textures[filePath] = newTex;
	return newTex;
}

void TextureManager::CollectGarbage()
{
	for (auto it = m_textures.begin(); it != m_textures.end();)
	{
		// 使用中チェック
		if (it->second.use_count() > 1)
		{
			++it;
			continue;
		}

		it = m_textures.erase(it);
	}
}

void TextureManager::Clear()
{
	m_textures.clear();
}

TextureManager& TextureManager::Instance()
{
	static TextureManager s_instance;
	return s_instance;
}
