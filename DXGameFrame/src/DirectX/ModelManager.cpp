// ModelManager.cpp
#include <DirectX/ModelManager.h>

std::shared_ptr<Model> ModelManager::Load(const std::string& filePath, ModelLoadType loadType)
{
	if (loadType == ModelLoadType::COUNT) {
		loadType = ModelLoadType::DEFAULT;
	}

	// モデルデータを格納するマップのキー
	// ファイルパス|読み込みタイプ
	std::string mapKey = filePath + "|" + std::to_string(static_cast<int>(loadType));

	// すでに読み込まれているかチェック
	auto it = m_models.find(mapKey);
	if (it != m_models.end())
	{
		// 読み込まれていれば再利用
		return it->second;
	}

	// モデルデータ作成
	auto model = std::make_shared<Model>();
	model->Load(filePath, loadType);
	m_models[mapKey] = model;

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

ModelManager& ModelManager::Instance()
{
	static ModelManager s_instance;
	return s_instance;
}
