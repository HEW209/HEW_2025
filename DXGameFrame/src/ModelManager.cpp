// ModelManager.cpp
#include <DirectX/ModelManager.h>

std::shared_ptr<Model> ModelManager::Load(const std::string& filePath)
{
	// すでに読み込まれているかチェック
	auto it = m_models.find(filePath);
	if (it != m_models.end())
	{
		// 読み込まれていれば再利用
		return it->second;
	}

	// モデルデータ作成
	auto model = std::make_shared<Model>();
	model->Load(filePath);
	m_models[filePath] = model;

	return model;
}

void ModelManager::CollectGarbage()
{
	for (auto it = m_models.begin(); it != m_models.end();)
	{
		// 使用中チェック
		if (it->second.use_count() > 1)
		{
			++it;
			continue;
		}

		it = m_models.erase(it);
	}
}

void ModelManager::Clear()
{
	m_models.clear();
}
