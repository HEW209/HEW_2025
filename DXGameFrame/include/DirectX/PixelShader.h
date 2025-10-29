/*****************************************************************//**
 * @file   PixelShader.h
 * @brief  ピクセルシェーダーを扱う
 *
 * @author 石田怜
 * @date   2025/09/17
 *********************************************************************/
#pragma once

#include "Shader.h"

/**
 * @brief ピクセルシェーダーを扱う
 */
class PixelShader : public Shader
{
public:
	PixelShader();
	~PixelShader() = default;

	/**
	 * @brief ピクセルシェーダーをデバイスコンテキストにセットする
	 */
	void Bind() override;

private:
	/// ピクセルシェーダー
	ComPtr<ID3D11PixelShader> m_pPS;

	/**
	 * @brief csoファイルからシェーダーデータを作成
	 * @param filePath ピクセルシェーダー(.cso)へのファイルパス
	 * @return 成功したかを返す
	 */
	HRESULT LoadFromCSOFile(const std::string& filePath) override;

	/**
	 * @brief ピクセルシェーダーを作成する
	 * @param data ピクセルシェーダーのコンパイル済みデータ
	 * @param dataSize ピクセルシェーダーデータのサイズ
	 * @return 成功したかを返す
	 */
	HRESULT CreatePixelShader(const char* data, int dataSize);
};