/******************************************************************//**
 * @file   GeometryShader.h
 * @brief  ジオメトリシェーダーを扱う
 *
 * @author 芝晃佑
 * @date   2025/12/20
 *********************************************************************/
#pragma once

#include "Shader.h"

 /**
  * @brief ジオメトリシェーダーを扱う
  */
class GeometryShader : public Shader
{
public:
	GeometryShader();
	~GeometryShader() = default;

	/**
	 * @brief ジオメトリシェーダーをデバイスコンテキストにセットする
	 */
	void Bind() override;

private:
	/// ジオメトリシェーダー
	ComPtr<ID3D11GeometryShader> m_pGS;

	/**
	 * @brief csoファイルからシェーダーデータを作成
	 * @param filePath ジオメトリシェーダー(.cso)へのファイルパス
	 * @return 成功したかを返す
	 */
	HRESULT LoadFromCSOFile(const std::string& filePath) override;

	/**
	 * @brief ジオメトリシェーダーを作成する
	 * @param data ジオメトリシェーダーのコンパイル済みデータ
	 * @param dataSize ジオメトリシェーダーデータのサイズ
	 * @return 成功したかを返す
	 */
	HRESULT CreateGeometryShader(const char* data, int dataSize);
};