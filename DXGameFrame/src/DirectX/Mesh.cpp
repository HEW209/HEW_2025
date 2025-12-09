// Mesh.cpp
#include <DirectX/Mesh.h>
#include <DirectX/Direct3D.h>

Mesh::Mesh() :
	m_desc{},
	m_pVtxBuffer(nullptr),
	m_pIdxBuffer(nullptr)
{
}

HRESULT Mesh::Create(const Description& desc)
{
	HRESULT hr = S_OK;		// 関数の結果

	// 頂点バッファ作成
	hr = CreateVertexBuffer(desc.vtx, desc.isWrite);
	if (FAILED(hr)) { return hr; }

	// インデックスバッファ作成
	if (!desc.idx.empty()) {
		hr = CreateIndexBuffer(desc.idx);
		if (FAILED(hr)) { return hr; }
	}

	// メッシュ情報のコピー
	m_desc = desc;

	return hr;
}

void Mesh::Draw()
{
	ID3D11DeviceContext* pContext = Direct3D::Instance().GetContext();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// 描画用情報をセット
	pContext->IASetPrimitiveTopology(m_desc.topology);
	pContext->IASetVertexBuffers(0, 1, m_pVtxBuffer.GetAddressOf(), &stride, &offset);

	// 描画
	if (!m_desc.idx.empty())
	{
		// インデックスバッファを使用して描画
		DXGI_FORMAT format = {};
		switch (sizeof(UINT))
		{
		case 2: format = DXGI_FORMAT_R16_UINT;
			break;
		case 4: format = DXGI_FORMAT_R32_UINT;
			break;
		}
		pContext->IASetIndexBuffer(m_pIdxBuffer.Get(), format, 0);
		pContext->DrawIndexed((UINT)m_desc.idx.size(), 0, 0);
	}
	else
	{
		// 頂点バッファのみで描画
		pContext->Draw((UINT)m_desc.vtx.size(), 0);
	}
}

const Mesh::Description& Mesh::GetDesc() const
{
	return m_desc;
}

UINT Mesh::GetMaterialID() const
{
	return m_desc.materialID;
}

HRESULT Mesh::CreateVertexBuffer(const std::vector<Vertex>& vtx, bool isWrite)
{
	HRESULT hr = S_OK;		// 関数の結果

	// 頂点バッファの設定
	D3D11_BUFFER_DESC bufDesc = {};		// 頂点バッファ設定情報
	bufDesc.ByteWidth = (UINT)(sizeof(Vertex) * vtx.size());
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	// 初期化用データ設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = vtx.data();

	// 頂点バッファの作成
	ID3D11Device* pDevice = Direct3D::Instance().GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, m_pVtxBuffer.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	return hr;
}

HRESULT Mesh::CreateIndexBuffer(const std::vector<UINT>& idx)
{
	HRESULT hr = S_OK;		// 関数の結果

	// インデックスバッファの設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = sizeof(UINT) * (UINT)idx.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// 初期化用データ設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = idx.data();

	// インデックスバッファの作成
	ID3D11Device* pDevice = Direct3D::Instance().GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, m_pIdxBuffer.GetAddressOf());

	return hr;
}