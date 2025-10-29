/*****************************************************************//**
 * @file   Direct3D.h
 * @brief  Direct3Dを扱う
 * 
 * @author 石田怜
 * @date   2025/09/15
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"
#include <Utility/Color.h>

/**
 * @brief Direct3Dを扱う
 */
class Direct3D
{
public:	
	/**
	 * @brief DirectXの初期化を行う
	 * @param hWnd ウィンドウハンドル
	 * @param width 画面の幅
	 * @param height 画面の高さ
	 * @param fullScreen フルスクリーンモード
	 * @return 初期化が成功したかを返す
	 */
	HRESULT Init(HWND hWnd, UINT width, UINT height, BOOL fullScreen);

	/**
	 * @brief 終了処理を行う
	 */
	void Uninit();
	
	/**
	 * @brief 描画を開始する
	 * @param clearColor 画面クリア色
	 */
	void BeginDraw(Color clearColor);

	/**
	 * @brief 描画を終了する
	 */
	void EndDraw();
	 
	/**
	 * @brief Direct3Dデバイスを取得する
	 * @return Direct3Dデバイスへのポインタ
	 */
	ID3D11Device* GetDevice()
	{
		return m_pDevice.Get();
	}

	/**
	 * @brief デバイスコンテキストを取得する
	 * @return デバイスコンテキストへのポインタ
	 */
	ID3D11DeviceContext* GetContext()
	{
		return m_pContext.Get();
	}

private:
	Direct3D();

	/// Direct3Dデバイス
	ComPtr<ID3D11Device> m_pDevice;

	/// Direct3Dデバイスコンテキスト
	ComPtr<ID3D11DeviceContext> m_pContext;

	/// スワップチェイン
	ComPtr<IDXGISwapChain> m_pSwapChain;

	/// バックバッファのRT(描画対象)ビュー
	ComPtr<ID3D11RenderTargetView> m_pRTV;

	///	深度ステンシルビュー
	ComPtr<ID3D11DepthStencilView> m_pDSV;

	/// 深度ステンシルバッファ
	ComPtr<ID3D11Texture2D> m_pDepthBuffer;

	/**
	 * @brief デバイス・デバイスコンテキスト・スワップチェインを作成する
	 * @param hWnd ウィンドウハンドル
	 * @param width 画面の幅
	 * @param height 画面の高さ
	 * @param fullScreen フルスクリーンモード
	 * @return 成功したかを返す
	 */
	HRESULT CreateDeviceAndSwapChain(HWND hWnd, UINT width, UINT height, BOOL fullScreen);

	/**
	 * @brief レンダーターゲットビューを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateRenderTargetView();

	/**
	 * @brief 深度ステンシルビューを作成する
	 * @param width 画面の幅
	 * @param height 画面の高さ
	 * @return 成功したかを返す
	 */
	HRESULT CreateDepthStencilView(UINT width, UINT height);
	
	/**
	 * @brief ビューポートの設定を行う
	 * @param width 画面の幅
	 * @param height 画面の高さ
	 */
	void SetViewPort(UINT width, UINT height);

public:	
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return Direct3Dインスタンスへの参照
	 */
	static Direct3D& Instance()
	{
		static Direct3D s_instance;
		return s_instance;
	}
};