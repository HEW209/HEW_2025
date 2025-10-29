/*****************************************************************//**
 * @file   ModelManager.h
 * @brief  モデルデータの管理を行う
 * 
 * @author 石田怜
 * @date   2025/10/18
 *********************************************************************/
#pragma once

#include <DirectX/Mesh.h>
#include <unordered_map>
#include <string>
#include <memory>

/**
 * @brief モデルデータの管理を行う
 */
class ModelManager
{
public:
	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータ(.fbx)へのファイルパス
	 * @return モデルのメッシュデータ
	 */
	std::shared_ptr<Mesh> Load(const std::string& filePath);

	/**
	 * @brief 使用していないアセットを解放する
	 */
	void CollectGarbage();

	/**
	 * @brief 全てのテクスチャを解放する
	 */
	void Clear();

private:
	ModelManager();

	///ファイルパスとモデルデータのマップ
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_models;
};