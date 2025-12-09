/*****************************************************************//**
 * @file   ModelManager.h
 * @brief  モデルデータの管理を行う
 * 
 * @author 石田怜
 * @date   2025/10/18
 *********************************************************************/
#pragma once

#include "Model.h"
#include <Utility/Common.h>

/**
 * @brief モデルデータの管理を行う
 */
class ModelManager
{
public:
	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータへのファイルパス
	 * @return モデルへのポインタ
	 */
	std::shared_ptr<Model> Load(const std::string& filePath);

	/**
	 * @brief 使用していないモデルデータを解放する
	 */
	void CollectGarbage();

	/**
	 * @brief 全てのモデルデータを解放する
	 */
	void Clear();

private:
	ModelManager() = default;

	/// ファイルパスとモデルデータのマップ
	std::unordered_map<std::string, std::shared_ptr<Model>> m_models;

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return ModelManagerへの参照
	 */
	static ModelManager& Instance()
	{
		static ModelManager s_instance;
		return s_instance;
	}
};