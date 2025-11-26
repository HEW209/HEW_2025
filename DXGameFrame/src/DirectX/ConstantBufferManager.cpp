// ConstantBuffer.cpp
#include <DirectX/ConstantBufferManager.h>

ConstantBufferManager::ConstantBufferManager() :
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

HRESULT ConstantBufferManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	// 定数バッファを作成
	hr = CreateAllBuffer();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void ConstantBufferManager::Uninit()
{
	m_pWVPBuffer.Reset();
	m_pLightBuffer.Reset();
	m_pBoneBuffer.Reset();
	m_pCustomBuffer.Reset();
}

void ConstantBufferManager::SetWorld(const DirectX::XMFLOAT4X4& world, bool bufferUpdate)
{
	m_WVP.world = world;

	if (bufferUpdate)
		UpdateWVPBuffer();
}

void ConstantBufferManager::SetView(const DirectX::XMFLOAT4X4& view, bool bufferUpdate)
{
	m_WVP.view = view;

	if (bufferUpdate)
		UpdateWVPBuffer();
}

void ConstantBufferManager::SetProjection(const DirectX::XMFLOAT4X4& projection, bool bufferUpdate)
{
	m_WVP.projection = projection;

	if (bufferUpdate)
		UpdateWVPBuffer();
}

void ConstantBufferManager::SetLight(const LightConstantBuffer& light)
{
	if (m_pContext == nullptr)
		return;

	// ライト定数バッファの更新
	m_pContext->UpdateSubresource(m_pLightBuffer.Get(), 0, nullptr, &light, 0, 0);
	m_pContext->PSSetConstantBuffers((UINT)SlotNum::LIGHT, 1, m_pLightBuffer.GetAddressOf());
}

void ConstantBufferManager::SetBone(DirectX::XMFLOAT4X4 bones[MaxBone])
{
	if (m_pContext == nullptr)
		return;

	// ボーン定数バッファの更新
	m_pContext->UpdateSubresource(m_pBoneBuffer.Get(), 0, nullptr, bones, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::BONE, 1, m_pBoneBuffer.GetAddressOf());
}

void ConstantBufferManager::SetCustomData(const std::vector<BYTE>& data)
{
	if (m_pContext == nullptr)
		return;

	if (data.size() > CustomCBSize)
		return;

	// 固定長配列にデータコピー
	BYTE customData[CustomCBSize] = {};
	size_t dataSize = (size_t)(sizeof(BYTE) * data.size());
	memcpy(customData, data.data(), dataSize);

	// カスタム定数バッファの更新
	m_pContext->UpdateSubresource(m_pCustomBuffer.Get(), 0, nullptr, customData, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::CUSTOM, 1, m_pCustomBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers((UINT)SlotNum::CUSTOM, 1, m_pCustomBuffer.GetAddressOf());
}

void ConstantBufferManager::UpdateWVPBuffer()
{
	if (m_pContext == nullptr)
		return;

	// WVP定数バッファの更新
	m_pContext->UpdateSubresource(m_pWVPBuffer.Get(), 0, nullptr, &m_WVP, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::WVP, 1, m_pWVPBuffer.GetAddressOf());
}

HRESULT ConstantBufferManager::CreateAllBuffer()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr = S_OK;		// 関数の結果

	for (int i = 0; i < (UINT)SlotNum::COUNT; ++i)
	{
		// 定数バッファの設定
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.Usage = D3D11_USAGE_DEFAULT;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = 0;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// 定数バッファを作成する
		switch ((SlotNum)i)
		{
		case ConstantBufferManager::SlotNum::WVP:
			// WVP定数バッファの作成
			cbDesc.ByteWidth = sizeof(WVP);
			hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_pWVPBuffer.GetAddressOf());
			break;

		case ConstantBufferManager::SlotNum::LIGHT:
			// ライト定数バッファの作成
			cbDesc.ByteWidth = sizeof(LightConstantBuffer);
			hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_pLightBuffer.GetAddressOf());
			break;

		case ConstantBufferManager::SlotNum::BONE:
			// ボーン定数バッファの作成
			cbDesc.ByteWidth = sizeof(BoneConstantBuffer);
			hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_pBoneBuffer.GetAddressOf());
			break;

		case ConstantBufferManager::SlotNum::CUSTOM:
			// カスタム定数バッファの作成
			cbDesc.ByteWidth = sizeof(CustomConstantBuffer);
			hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_pCustomBuffer.GetAddressOf());
			break;
		}
		if (FAILED(hr)) { return hr; }
	}

	return hr;
}

ConstantBufferManager& ConstantBufferManager::Instance()
{
	static ConstantBufferManager s_instance;
	return s_instance;
}
