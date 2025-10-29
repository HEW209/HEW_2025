/*****************************************************************//**
 * @file   Shader.h
 * @brief  シェーダーの基底クラス
 * 
 * @author 石田怜
 * @date   2025/09/18
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <string>

/**
 * @brief シェーダーの基底クラス
 */
class Shader
{
public:
	Shader() = default;
	virtual ~Shader() = default;

	/**
	 * @brief シェーダーを読み込む
	 * @param filePath シェーダー(.cso)へのファイルパス
	 */
	void Load(const std::string& filePath);

	/**
	 * @brief シェーダーをセットする
	 */
	virtual void Bind() = 0;

private:
	/**
	 * @brief csoファイルからシェーダーデータを作成
	 * @param filePath シェーダー(.cso)へのファイルパス
	 * @return 成功したかを返す
	 */
	virtual HRESULT LoadFromCSOFile(const std::string& filePath) = 0;
};
