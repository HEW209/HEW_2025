/******************************************************************//**
 * @file   VertexShader.h
 * @brief  頂点シェーダーを扱う
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "Shader.h"

/**
 * @brief 頂点シェーダーを扱う
 */
class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader() = default;

	/**
	 * @brief 頂点シェーダーをデバイスコンテキストにセットする
	 */
	void Bind() override;

private:
	/// 頂点シェーダー
	ComPtr<ID3D11VertexShader> m_pVS;

	/// 頂点入力レイアウト
	ComPtr<ID3D11InputLayout> m_pInputLayout;

	/**
	 * @brief csoファイルからシェーダーデータを作成
	 * @param filePath 頂点シェーダー(.cso)へのファイルパス
	 * @return 成功したかを返す
	 */
	HRESULT LoadFromCSOFile(const std::string& filePath) override;

	/**
	 * @brief 頂点シェーダーを作成する
	 * @param data 頂点シェーダーのコンパイル済みデータ
	 * @param dataSize 頂点シェーダーデータのサイズ
	 * @return 成功したかを返す
	 */
	HRESULT CreateVertexShader(const char* data, int dataSize);

	/**
	 * @brief 頂点入力レイアウトを作成する
	 * @param data 頂点シェーダーのコンパイル済みデータ
	 * @param dataSize 頂点シェーダーデータのサイズ
	 * @return 成功したかを返す
	 */
	HRESULT CreateInputLayout(const char* data, int dataSize);
};