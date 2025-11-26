// SpriteDrawer.cpp
#include <DirectX/SpriteDrawer.h>

SpriteDrawer::SpriteDrawer() :
	m_pDevice(nullptr),
	m_pContext(nullptr)
{
}

HRESULT SpriteDrawer::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	hr = CreateVertexBuffer();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void SpriteDrawer::Uninit()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;
	m_pVertexBuffer.Reset();
}

void SpriteDrawer:: Draw(const Material& material)
{
	if (m_pContext == nullptr)
		return;

	// マテリアルをバインド
	material.Bind();

	UINT stride = sizeof(SpriteVertex);
	UINT offset = 0;

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	m_pContext->Draw(4, 0);
}

HRESULT SpriteDrawer::CreateVertexBuffer()
{
	HRESULT hr = S_OK;		// 関数の結果
	
	if (m_pDevice == nullptr)
		return S_FALSE;

	// 頂点情報を設定
	SpriteVertex vtx[] =
	{
		{{-0.5f,  0.5f}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {1.0f, 1.0f}},
	};

	// 頂点バッファの設定
	D3D11_BUFFER_DESC bufDesc = {};		// 頂点バッファ設定情報
	bufDesc.ByteWidth = (UINT)sizeof(vtx);
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 初期化用データ設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = vtx;

	// 頂点バッファの作成
	hr = m_pDevice->CreateBuffer(&bufDesc, &subResource, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	return hr;
}

SpriteDrawer& SpriteDrawer::Instance()
{
	static SpriteDrawer s_instance;
	return s_instance;
}