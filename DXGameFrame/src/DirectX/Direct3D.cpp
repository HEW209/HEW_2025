// Direct3D.cpp
#include <DirectX/Direct3D.h>
#include <DirectX/PipelineStateManager.h>
#include <DirectX/ConstantBufferManager.h>
#include <DirectX/Geometry.h>
#include <DirectX/SpriteDrawer.h>
#include <DirectX/ShaderManager.h>
#include <DirectX/TextureManager.h>
#include <DirectX/ModelManager.h>

Direct3D::Direct3D() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRTV(nullptr),
	m_pDSV(nullptr),
	m_pDepthBuffer(nullptr),
	m_pShadowDSV(nullptr),
	m_pShadowSRV(nullptr),
	m_viewportSizeH(0),
	m_viewportSizeW(0)
{
}

HRESULT Direct3D::Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr = S_OK;		// 関数の結果

	// デバイス・スワップチェインを作成
	hr = CreateDeviceAndSwapChain(hWnd, width, height);
	if (FAILED(hr)) { return hr; }

	// レンダーターゲットビュー・深度ステンシルビューを作成
	hr = CreateRenderTargets(width, height);
	if (FAILED(hr)) { return hr; }

	// ビューポートの設定
	SetViewport(width, height);

	// その他の描画関連機能の初期化
	hr = InitAllRenderSystems();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Direct3D::Uninit()
{
	// 描画関連クラスの解放処理呼び出し
	UninitAllRenderSystems();

	// リソースの解放
	m_pDSV.Reset();
	m_pDepthBuffer.Reset();
	m_pRTV.Reset();
	m_pSwapChain.Reset();
	m_pContext.Reset();
	m_pDevice.Reset();
}

HRESULT Direct3D::Resize(UINT width, UINT height)
{
	HRESULT hr = S_OK;		// 関数の結果

	// 描画リソースのリセット
	m_pDSV.Reset();
	m_pDepthBuffer.Reset();
	m_pRTV.Reset();

	// スワップチェインのサイズを変更
	ResizeSwapChain(width, height);

	// ビューポート再設定
	SetViewport(width, height);

	// レンダーターゲットビュー・深度ステンシルビューを作成
	hr = CreateRenderTargets(width, height);
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Direct3D::ClearView(const float clearColor[4])
{
	// 画面クリア
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), clearColor);

	// ステンシルビュークリア
	m_pContext->ClearDepthStencilView(m_pDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// シャドウ用DSVクリア
	m_pContext->ClearDepthStencilView(m_pShadowDSV.Get(),
		D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::BeginDraw()
{
	// レンダーターゲットをセット
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());

	// ビューポートをセット
	SetViewport(m_viewportSizeW, m_viewportSizeH);
}

void Direct3D::BeginDrawShadow()
{
	// シャドウ用レンダーターゲットをセット
	m_pContext->OMSetRenderTargets(0, nullptr, m_pShadowDSV.Get());

	// シャドウ用ビューポートをセット
	SetViewport(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, true);
}

void Direct3D::Present()
{
	//描画内容を画面に表示
	m_pSwapChain->Present(0, 0);
}

void Direct3D::ClearStencilView()
{
	m_pContext->ClearDepthStencilView(m_pDSV.Get(),
		D3D11_CLEAR_STENCIL, 1.0f, 0);
}

ID3D11Device* Direct3D::GetDevice() const
{
	return m_pDevice.Get();
}

ID3D11DeviceContext* Direct3D::GetContext() const
{
	return m_pContext.Get();
}

void Direct3D::SetShadowMap()
{
	// ピクセルシェーダーのシェーダーリソースビューにシャドウマップをセット
	m_pContext->PSSetShaderResources(
		TextureSlot::ShadowMap,
		1,
		m_pShadowSRV.GetAddressOf()
	);
}

HRESULT Direct3D::CreateDeviceAndSwapChain(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr = S_OK;		// 関数の結果

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC sd = {};		// スワップチェイン設定情報
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ドライバの種類を設定
	D3D_DRIVER_TYPE driverType;				// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] = {		// ドライバの種類を列挙
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes =					// ドライバの種類数
		ARRAYSIZE(driverTypes);

	// 機能レベルを設定
	D3D_FEATURE_LEVEL featureLevel;			// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] = {	// 機能レベルを列挙
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	UINT numFeatureLevels =					// 機能レベルの種類数
		ARRAYSIZE(featureLevels);

	// デバイス作成時の動作フラグを設定
	UINT createDeviceFlag = 0;				// デバイス作成時の動作フラグ (デフォルト)
#ifdef _DEBUG
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;		// デバッグレイヤーをサポート
#endif

	// 作成可能な設定でDirectXの機能を有効化 
	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		// ドライバの種類を一つずつ試す
		driverType = driverTypes[i];

		// ドライバとスワップチェインの作成 (作成可能な範囲で最も性能が良いものを作成)
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlag,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			m_pSwapChain.GetAddressOf(),
			m_pDevice.GetAddressOf(),
			&featureLevel,
			m_pContext.GetAddressOf()
		);

		// 作成に成功したら終了
		if (SUCCEEDED(hr)) { break; }
	}

	return hr;
}

HRESULT Direct3D::CreateRenderTargets(UINT width, UINT height)
{
	HRESULT hr = S_OK;		// 関数の結果

	// レンダーターゲットビューを作成
	hr = CreateRenderTargetView();
	if (FAILED(hr)) { return hr; }

	// 深度ステンシルビューを作成
	hr = CreateDepthStencilView(width, height);
	if (FAILED(hr)) { return hr; }

	// シャドウマップを作成
	hr = CreateShadowMap();
	if (FAILED(hr)) { return hr; }

	// レンダーターゲットビューと深度ステンシルビューをセット
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());

	return hr;
}

HRESULT Direct3D::CreateRenderTargetView()
{
	HRESULT hr = S_OK;		// 関数の結果

	// レンダーターゲットの作成
	ComPtr<ID3D11Texture2D> pBackBuffer;		// フレームバッファへのポインタ

	// フレームバッファへのポインタを取得 (IID_PPV_ARGSはGetAddressOf()不要)
	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hr)) { return hr; }

	// フレームバッファへのポインタで紐づけて、レンダーターゲットビューを作成
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, m_pRTV.GetAddressOf());

	return hr;
}

HRESULT Direct3D::CreateDepthStencilView(UINT width, UINT height)
{
	HRESULT hr = S_OK;		// 関数の結果

	// 深度ステンシルバッファの設定
	D3D11_TEXTURE2D_DESC depthDesc = {};		// 深度ステンシルバッファの設定情報
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// 深度ステンシルバッファを作成
	hr = m_pDevice->CreateTexture2D(&depthDesc, nullptr, m_pDepthBuffer.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	// 深度ステンシルビューの設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};		// 深度ステンシルビューの設定情報
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// 深度ステンシルビューを作成
	hr = m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), &dsvDesc, m_pDSV.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	return hr;
}

HRESULT Direct3D::CreateShadowMap()
{
	HRESULT hr = S_OK;		// 関数の結果

	// シャドウマップの定義
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = SHADOW_MAP_SIZE;
	texDesc.Height = SHADOW_MAP_SIZE;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	hr = m_pDevice->CreateTexture2D(&texDesc, nullptr, m_pShadowMap.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	// シャドウ用DSV (書き込み用)
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = m_pDevice->CreateDepthStencilView(m_pShadowMap.Get(), &dsvDesc, m_pShadowDSV.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	// シャドウ用SRV (読み込み用)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	hr = m_pDevice->CreateShaderResourceView(m_pShadowMap.Get(), &srvDesc, m_pShadowSRV.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Direct3D::SetViewport(UINT width, UINT height, bool isTemp)
{
	// ビューポートの設定
	D3D11_VIEWPORT vp = {};			// ビューポート設定情報
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	// ビューポートを設定
	m_pContext->RSSetViewports(1, &vp);

	if (!isTemp) {
		// ビューポートの幅・高さを保存
		m_viewportSizeW = width;
		m_viewportSizeH = height;
	}
}

HRESULT Direct3D::ResizeSwapChain(UINT width, UINT height)
{
	HRESULT hr = S_OK;		// 関数の結果

	if (m_pSwapChain == nullptr)
		return S_FALSE;

	hr = m_pSwapChain->ResizeBuffers(
		0, width, height,
		DXGI_FORMAT_UNKNOWN,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);
	if (FAILED(hr)) { return hr; }

	return hr;
}

HRESULT Direct3D::InitAllRenderSystems()
{
	HRESULT hr = S_OK;		// 関数の結果

	// パイプラインステートの初期化
	hr = PipelineStateManager::Instance().Init(m_pDevice.Get(), m_pContext.Get());
	if (FAILED(hr)) { return hr; }

	// 定数バッファマネージャーの初期化
	ConstantBufferManager::Instance().Init(m_pDevice.Get(), m_pContext.Get());
	if (FAILED(hr)) { return hr; }

	// ジオメトリの初期化
	hr = Geometry::Instance().Init();
	if (FAILED(hr)) { return hr; }

	// スプライトの描画機能の初期化
	hr = SpriteDrawer::Instance().Init(m_pDevice.Get(), m_pContext.Get());
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Direct3D::UninitAllRenderSystems()
{
	SpriteDrawer::Instance().Uninit();
	Geometry::Instance().Uninit();
	ConstantBufferManager::Instance().Uninit();
	PipelineStateManager::Instance().Uninit();

	// リソースの解放
	ShaderManager::Instance().Clear();
	TextureManager::Instance().Clear();
	ModelManager::Instance().Clear();
}

Direct3D& Direct3D::Instance()
{
	static Direct3D s_instance;
	return s_instance;
}