/******************************************************************//**
 * @file   Direct3D.h
 * @brief  Direct3Dを扱う
 * 
 * @author 石田怜
 * @date   2025/11/21
 * 
 * @date   2025/12/28 [芝晃佑]	シャドウ描画対応
 *********************************************************************/
#pragma once

#include "DirectXInclude.h"

constexpr UINT SHADOW_MAP_SIZE = 4096;	// シャドウマップの解像度

/**
 * @brief Direct3Dを扱う
 */
class Direct3D
{
public:
	/**
	 * @brief DirectXの初期化を行う
	 * @param hWnd ウィンドウハンドル
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT Init(HWND hWnd, UINT width, UINT height);

	/**
	 * @brief DirectXの終了処理
	 */
	void Uninit();

	/**
	 * @brief ウィンドウサイズが変更時に呼ぶ処理
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT Resize(UINT width, UINT height);

	/**
	 * @brief RTV, DSV, ShadowDSVをクリアする
	 * @param clearColor 画面クリア色
	 */
	void ClearView(const float clearColor[4]);

	/**
	 * @brief 描画を開始する
	 * @param clearColor 画面クリア色
	 */
	void BeginDraw();

	/**
	 * @brief シャドウの描画を開始する
	 */
	void BeginDrawShadow();

	/**
	 * @brief グループ透過オブジェクトの深度描画を開始する
	 */
	void BeginDrawTransparentDepth();

	/**
	 * @brief 描画を終了する
	 */
	void Present();

	/**
	 * @brief ステンシルビューをクリアする
	 */
	void ClearStencilView();

	/**
	 * @brief Direct3Dデバイスを取得する
	 * @return デバイスへのポインタ
	 */
	ID3D11Device* GetDevice() const;

	/**
	 * @brief Direct3Dデバイスコンテクストを取得する
	 * @return デバイスコンテクストへのポインタ
	 */
	ID3D11DeviceContext* GetContext() const;

	/**
	 * @brief ビューポートの幅を取得する
	 * @return ビューポートの幅
	 */
	UINT GetViewportSizeW() const { return m_viewportSizeW; }

	/**
	 * @brief ビューポートの高さを取得する
	 * @return ビューポートの高さ
	 */
	UINT GetViewportSizeH() const { return m_viewportSizeH; }

	/**
	 * @brief シャドウ用のシェーダーリソースビューを取得する
	 * @return シャドウ用のシェーダーリソースビューへのポインタ
	 */
	ID3D11ShaderResourceView* GetShadowSRV() const { return m_pShadowSRV.Get(); }

	/**
	 * @brief シャドウマップをセットする
	 */
	void SetShadowMap();

	/**
	 * @brief グループ透過用の深度マップをセットする
	 */
	void SetTransparentDepthMap();

private:
	Direct3D();
	~Direct3D() = default;

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

	///	シャドウ用の深度ステンシルビュー
	ComPtr<ID3D11DepthStencilView> m_pShadowDSV;

	///	シャドウ用のシェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_pShadowSRV;

	/// シャドウマップ
	ComPtr<ID3D11Texture2D> m_pShadowMap;

	///	グループ透過用の深度ステンシルビュー
	ComPtr<ID3D11DepthStencilView> m_pTransparentDSV;

	///	グループ透過用のシェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_pTransparentSRV;

	/// グループ透過用の深度ステンシルバッファ
	ComPtr<ID3D11Texture2D> m_pTransparentDepthBuffer;

	/// ビューポートの幅
	UINT m_viewportSizeW;
	/// ビューポートの高さ
	UINT m_viewportSizeH;

	/**
	 * @brief デバイス・デバイスコンテキスト・スワップチェインを作成する
	 * @param hWnd ウィンドウハンドル
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT CreateDeviceAndSwapChain(HWND hWnd, UINT width, UINT height);

	/**
	 * @brief レンダーターゲットビュー・深度ステンシルビューを作成しセットする
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT CreateRenderTargets(UINT width, UINT height);

	/**
	 * @brief レンダーターゲットビューを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateRenderTargetView();

	/**
	 * @brief 深度ステンシルビューを作成する
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT CreateDepthStencilView(UINT width, UINT height);

	/**
	 * @brief シャドウマップを作成する
	 * @return 成功したかを返す
	 */
	HRESULT CreateShadowMap();

	/**
	 * @brief グループ透過用の深度ステンシルビューを作成する
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT CreateTransparentDepthStencilView(UINT width, UINT height);

	/**
	 * @brief ビューポート設定を行う
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 */
	void SetViewport(UINT width, UINT height, bool isTemp = false);

	/**
	 * @brief スワップチェインのサイズを再設定する
	 * @param width クライアント領域の幅
	 * @param height クライアント領域の高さ
	 * @return 成功したかを返す
	 */
	HRESULT ResizeSwapChain(UINT width, UINT height);

	/**
	 * @brief DirectX描画関連クラスをまとめて初期化する
	 * @return 成功したかを返す
	 */
	HRESULT InitAllRenderSystems();

	/**
	 * @brief DirectX描画関連クラスをまとめて解放する
	 */
	void UninitAllRenderSystems();

public:
	/**
	 * @brief 唯一のインスタンスを取得する
	 * @return Direct3Dインスタンスへの参照
	 */
	static Direct3D& Instance();
};
