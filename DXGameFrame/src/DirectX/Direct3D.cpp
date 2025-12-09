//Direct3D.cpp
#include <DirectX/Direct3D.h>
#include <DirectX/PipelineState.h>
#include <DirectX/ConstantBuffer.h>
#include <DirectX/Geometry.h>

Direct3D::Direct3D() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRTV(nullptr),
	m_pDSV(nullptr),
	m_pDepthBuffer(nullptr)
{
}

HRESULT Direct3D::Init(HWND hWnd, UINT width, UINT height, BOOL fullScreen)
{
	HRESULT hr;		//関数の結果

	//デバイス・スワップチェインを作成
	hr = CreateDeviceAndSwapChain(hWnd, width, height, fullScreen);
	if (FAILED(hr)) { return hr; }

	//レンダーターゲットビューを作成
	hr = CreateRenderTargetView();
	if (FAILED(hr)) { return hr; }

	//深度ステンシルビューを作成
	hr = CreateDepthStencilView(width, height);
	if (FAILED(hr)) { return hr; }

	//GPUの出力先設定
	m_pContext->OMSetRenderTargets(
		1,
		m_pRTV.GetAddressOf(),
		m_pDSV.Get()
	);

	//ビューポートの設定
	SetViewPort(width, height);

	//パイプラインステートの初期化
	hr = PipelineState::Instance().Init(m_pDevice.Get(), m_pContext.Get());
	if (FAILED(hr)) { return hr; }

	//定数バッファの初期化
	hr = ConstantBuffer::Instance().Init(m_pDevice.Get(), m_pContext.Get());
	if (FAILED(hr)) { return hr; }

	//ジオメトリの初期化
	hr = Geometry::Instance().Init();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Direct3D::Uninit()
{
	Geometry::Instance().Uninit();
	ConstantBuffer::Instance().Uninit();
	PipelineState::Instance().Uninit();
}

void Direct3D::BeginDraw(Color clearColor)
{
	//画面クリア
	float color[4] = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), color);

	//ステンシルビュークリア
	m_pContext->ClearDepthStencilView(m_pDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Direct3D::EndDraw()
{
	//描画内容を画面に表示
	m_pSwapChain->Present(1, 0);
}

HRESULT Direct3D::CreateDeviceAndSwapChain(HWND hWnd, UINT width, UINT height, BOOL fullScreen)
{
	HRESULT hr;		//関数の結果

	//スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC sd = {};		//スワップチェイン設定
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width;							
	sd.BufferDesc.Height = height;							
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		
	sd.BufferDesc.RefreshRate.Numerator = 60;				
	sd.BufferDesc.RefreshRate.Denominator = 1;				
	sd.SampleDesc.Count = 1;								
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		
	sd.BufferCount = 1;										
	sd.OutputWindow = hWnd;									
	sd.Windowed = fullScreen ? FALSE : TRUE;				
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//ドライバの種類を設定
	D3D_DRIVER_TYPE driverType;				//ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] = {		//ドライバの種類を列挙
		D3D_DRIVER_TYPE_HARDWARE,	
		D3D_DRIVER_TYPE_WARP,		
		D3D_DRIVER_TYPE_REFERENCE	
	};
	UINT numDriverTypes =					//ドライバの種類数
		ARRAYSIZE(driverTypes);

	//機能レベルを設定
	D3D_FEATURE_LEVEL featureLevel;			//機能レベル
	D3D_FEATURE_LEVEL featureLevels[] = {	//機能レベルを列挙
		D3D_FEATURE_LEVEL_11_1,	
		D3D_FEATURE_LEVEL_11_0,	
		D3D_FEATURE_LEVEL_10_1,	
		D3D_FEATURE_LEVEL_10_0,	
		D3D_FEATURE_LEVEL_9_3,	
		D3D_FEATURE_LEVEL_9_2,	
		D3D_FEATURE_LEVEL_9_1,	
	};
	UINT numFeatureLevels =					//機能レベルの種類数
		ARRAYSIZE(featureLevels);

	//デバイス作成時の動作フラグを設定
	UINT createDeviceFlag = 0;				//デバイス作成時の動作フラグ (デフォルト)
#ifdef _DEBUG
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;		//デバッグレイヤーをサポート
#endif

	//作成可能な設定でDirectXの機能を有効化 
	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		//ドライバの種類を一つずつ試す
		driverType = driverTypes[i];

		//ドライバとスワップチェインの作成 (作成可能な範囲で最も性能が良いものを作成)
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,					
			driverType,				
			NULL,			
			createDeviceFlag,		
			featureLevels,			
			numFeatureLevels,		
			D3D11_SDK_VERSION,		
			&sd,					
			&m_pSwapChain,			
			&m_pDevice,				
			&featureLevel,			
			&m_pContext				
		);

		//作成に成功したら終了
		if (SUCCEEDED(hr)) { break; }
	}

	return hr;
}

HRESULT Direct3D::CreateRenderTargetView()
{
	HRESULT hr;									//関数の結果

	//レンダーターゲットの作成
	ComPtr<ID3D11Texture2D> pBackBuffer;		//フレームバッファへのポインタ
	pBackBuffer = nullptr;

	//フレームバッファへのポインタを取得
	hr = m_pSwapChain->GetBuffer(
		0,							
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer		
	);
	if (FAILED(hr)) { return hr; }

	//フレームバッファへのポインタで紐づけて、レンダーターゲットビューを作成
	hr = m_pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		NULL,			
		&m_pRTV			
	);

	return hr;
}

HRESULT Direct3D::CreateDepthStencilView(UINT width, UINT height)
{
	//深度ステンシルバッファの設定
	D3D11_TEXTURE2D_DESC depthDesc = {};		//深度ステンシルバッファの設定情報
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	//深度ステンシルバッファを作成
	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	HRESULT hr = m_pDevice->CreateTexture2D(&depthDesc, nullptr, &depthStencilBuffer);
	if (FAILED(hr)) return hr;

	//深度ステンシルビューの設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};		//深度ステンシルビューの設定情報
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	//深度ステンシルビューを作成
	hr = m_pDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, m_pDSV.GetAddressOf());
	if (FAILED(hr)) return hr;

	return hr;
}

void Direct3D::SetViewPort(UINT width, UINT height)
{
	//ビューポートの設定
	D3D11_VIEWPORT vp = {};			//ビューポート設定情報
	vp.TopLeftX = 0.0f;				
	vp.TopLeftY = 0.0f;				
	vp.Width = (float)width;		
	vp.Height = (float)height;		
	vp.MinDepth = 0.0f;				
	vp.MaxDepth = 1.0f;				

	//ビューポートを設定
	m_pContext->RSSetViewports(1, &vp);
}

//HRESULT Direct3D::CreateSpriteVertexBuffer()
//{
//	HRESULT hr;		//関数の結果
//
//	SpriteVertex vertex[] =
//	{
//		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } }, // 左下
//		{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } }, // 左上
//		{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } }, // 右下
//		{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } }  // 右上
//	};
//
//	//頂点バッファの設定
//	D3D11_BUFFER_DESC bd = {};		//頂点バッファ設定情報
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(vertex);
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//
//	//初期化用データの設定
//	D3D11_SUBRESOURCE_DATA initData = {};
//	initData.pSysMem = vertex;
//
//	hr = m_pDevice->CreateBuffer(&bd, &initData, m_pSpriteVertexBuffer.GetAddressOf());
//	if (FAILED(hr)) return hr;
//
//	return hr;
//}
