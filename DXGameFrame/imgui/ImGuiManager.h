/******************************************************************//**
 * @file   ImGuiManager.h
 * @brief  ImGuiを扱う
 * 
 * @author 石田怜
 * @date   2025/11/24
 *********************************************************************/
#pragma once

#include <DirectX/DirectXInclude.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class ImGuiManager
{
public:
	/**
	 * @brief ImGuiの使用準備を行う
	 * @param hWnd ウィンドウハンドル
	 * @param device D3Dデバイスへのポインタ
	 * @param context D3Dデバイスコンテキストへのポインタ
	 */
	void Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);

	/**
	 * @brief ImGuiの終了処理
	 */
	void Uninit();

	/**
	 * @brief ImGuiのフレーム開始処理
	 */
	void BeginFrame();

	/**
	 * @brief ImGuiの描画処理
	 */
	void Draw();

private:
	ImGuiManager();

	/// 初期化済みフラグ
	bool m_initialized;

public:
	static ImGuiManager& Instance();
};