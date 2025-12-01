/******************************************************************//**
 * @file   ShaderManager.h
 * @brief  シェーダーを管理する
 * 
 * @author 石田怜
 * @date   2025/11/22
 *********************************************************************/
#pragma once

#include "VertexShader.h"
#include "PixelShader.h"
#include <unordered_map>
#include <memory>

/**
 * @brief シェーダーを管理する
 */
class ShaderManager
{
public:
	/**
	 * @brief 多重読み込みを防ぎつつ頂点シェーダーを読み込む
	 * @param filePath 頂点シェーダー(.cso)へのファイルパス
	 * @return 頂点シェーダーへのポインタ(shared_ptr)
	 */
	std::shared_ptr<VertexShader> LoadVertexShader(const std::string& filePath);

	/**
	 * @brief 多重読み込みを防ぎつつピクセルシェーダーを読み込む
	 * @param filePath 頂点シェーダー(.cso)へのファイルパス
	 * @return ピクセルシェーダーへのポインタ(shared_ptr)
	*/
	std::shared_ptr<PixelShader> LoadPixelShader(const std::string& filePath);

	/**
	 * @brief 頂点シェーダーをセットする
	 * @param pVS 実際の頂点シェーダーへのポインタ
	 * @param pInputLayout 入力レイアウトへのポインタ
	 */
	void SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pInputLayout);

	/**
	 * @brief ピクセルシェーダーをセットする
	 * @param pPixelShader 実際のピクセルシェーダーへのポインタ
	 */
	void SetPixelShader(ID3D11PixelShader* pPS);

	/**
	 * @brief 全てのシェーダーを解放する
	 */
	void Clear();

private:
	ShaderManager();

	/// 頂点シェーダーとファイルパスのマップ
	std::unordered_map<std::string, std::shared_ptr<VertexShader>> m_pVertexShaders;

	/// ピクセルシェーダーとファイルパスのマップ
	std::unordered_map<std::string, std::shared_ptr<PixelShader>> m_pPixelShaders;

	/// 現在セットされている頂点シェーダー
	ID3D11VertexShader* m_pCurrentVS;

	/// 現在セットされているピクセルシェーダー
	ID3D11PixelShader* m_pCurrentPS;

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return ShaderManagerインスタンスへの参照
	 */
	static ShaderManager& Instance();
};
