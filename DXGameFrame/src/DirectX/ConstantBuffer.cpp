//ConstantBuffer.cpp
#include <DirectX/ConstantBuffer.h>

ConstantBuffer::ConstantBuffer() :
	m_pDevice(nullptr),
	m_pContext(nullptr)
{
	DirectX::XMFLOAT4X4 identity;	// 行列初期値
	DirectX::XMStoreFloat4x4(
		&identity,
		DirectX::XMMatrixIdentity()
	);

	// 行列の初期化
	m_WVP.world = identity;
	m_WVP.view = identity;
	m_WVP.projection = identity;
}

HRESULT ConstantBuffer::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	hr = CreateWVPBuffer();
	if (FAILED(hr)) { return hr; }

	hr = CreateLightBuffer();
	if (FAILED(hr)) { return hr; }

	hr = CreateBoneBuffer();
	if (FAILED(hr)) { return hr; }

	hr = CreateCustomBuffer();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void ConstantBuffer::Uninit()
{
	m_WVPBuffer.Reset();
}

void ConstantBuffer::SetWorld(const DirectX::XMFLOAT4X4& world, bool bufferUpdate)
{
	m_WVP.world = world;

	if (bufferUpdate)
		UpdateWVPBuffer();
}

void ConstantBuffer::SetView(const DirectX::XMFLOAT4X4& view, bool bufferUpdate)
{
	m_WVP.view = view;

	if (bufferUpdate)
		UpdateWVPBuffer();
}

void ConstantBuffer::SetProjection(const DirectX::XMFLOAT4X4& projection, bool bufferUpdate)
{
	m_WVP.projection = projection;

	if (bufferUpdate)
		UpdateWVPBuffer();
}

void ConstantBuffer::UpdateWVPBuffer()
{
	if (m_pContext == nullptr)
		return;

	m_pContext->UpdateSubresource(m_WVPBuffer.Get(), 0, nullptr, &m_WVP, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::WVP, 1, m_WVPBuffer.GetAddressOf());
}

HRESULT ConstantBuffer::CreateWVPBuffer()
{
	HRESULT hr = S_OK;		//関数の結果

	//WVP定数バッファの設定
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(WVP);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	//WVP定数バッファの作成
	hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_WVPBuffer.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	return hr;
}

HRESULT ConstantBuffer::CreateLightBuffer()
{
	return S_OK;
}

HRESULT ConstantBuffer::CreateBoneBuffer()
{
	return S_OK;
}

HRESULT ConstantBuffer::CreateCustomBuffer()
{
	return S_OK;
}
