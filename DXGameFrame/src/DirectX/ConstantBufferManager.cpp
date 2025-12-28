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
	m_frameCB.view = identity;
	m_frameCB.projection = identity;

	m_frameCB.time = 0.0f;
	m_frameCB.cameraPos = { 0.0f, 0.0f, 0.0f };
	m_frameCB.lightCB = {};
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
	m_pWorldBuffer.Reset();
	m_pFrameBuffer.Reset();
	m_pBoneBuffer.Reset();
	m_pCustomBuffer.Reset();
}

void ConstantBufferManager::SetWorld(const DirectX::XMMATRIX& world)
{
	if (m_pContext == nullptr)
		return;

	WorldConstantBuffer worldCB;		// 転送用データ
	DirectX::XMMATRIX worldMat;			// 計算用ワールド行列
	DirectX::XMMATRIX invWorldMat;		// 計算用ワールド逆行列

	// ワールド行列をセット
	worldMat = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&worldCB.world, worldMat);

	// ワールド逆行列をセット
	invWorldMat = DirectX::XMMatrixInverse(nullptr, world);
	invWorldMat = DirectX::XMMatrixTranspose(invWorldMat);
	DirectX::XMStoreFloat4x4(&worldCB.invWorld, invWorldMat);

	// 定数バッファを更新
	m_pContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &worldCB, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::WORLD, 1, m_pWorldBuffer.GetAddressOf());
	m_pContext->GSSetConstantBuffers((UINT)SlotNum::WORLD, 1, m_pWorldBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers((UINT)SlotNum::WORLD, 1, m_pWorldBuffer.GetAddressOf());
}

void ConstantBufferManager::SetView(const DirectX::XMMATRIX& view)
{
	if (m_pContext == nullptr)
		return;

	DirectX::XMMATRIX viewMat;			// 計算用ビュー行列
	DirectX::XMMATRIX invViewMat;		// 計算用ビュー逆行列

	// ビュー行列をセット
	viewMat = DirectX::XMMatrixTranspose(view);
	DirectX::XMStoreFloat4x4(&m_frameCB.view, viewMat);

	// カメラ座標をセット
	invViewMat = DirectX::XMMatrixInverse(nullptr, view);
	m_frameCB.cameraPos.x = DirectX::XMVectorGetX(invViewMat.r[3]);
	m_frameCB.cameraPos.y = DirectX::XMVectorGetY(invViewMat.r[3]);
	m_frameCB.cameraPos.z = DirectX::XMVectorGetZ(invViewMat.r[3]);
}

void ConstantBufferManager::SetProjection(const DirectX::XMMATRIX& projection)
{
	if (m_pContext == nullptr)
		return;

	// プロジェクション行列をセット
	DirectX::XMStoreFloat4x4(
		&m_frameCB.projection,
		DirectX::XMMatrixTranspose(projection)
	);
}

void ConstantBufferManager::SetLight(const LightConstantBuffer& light)
{
	if (m_pContext == nullptr)
		return;

	// ライト定数バッファをセット
	m_frameCB.lightCB = light;
}

void ConstantBufferManager::SetLightViewProj(const DirectX::XMMATRIX& lightViewProj)
{
	if (m_pContext == nullptr)
		return;

	// ライトビュー・プロジェクション行列をセット
	DirectX::XMStoreFloat4x4(
		&m_frameCB.lightViewProj,
		DirectX::XMMatrixTranspose(lightViewProj)
	);
}

void ConstantBufferManager::SetTime(float time)
{
	m_frameCB.time = time;
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

	if (data.size() == 0 || data.size() > CustomCBSize)
		return;

	// 固定長配列にデータコピー
	BYTE customData[CustomCBSize] = {};
	size_t dataSize = (size_t)(sizeof(BYTE) * data.size());
	memcpy(customData, data.data(), dataSize);

	// カスタム定数バッファの更新
	m_pContext->UpdateSubresource(m_pCustomBuffer.Get(), 0, nullptr, customData, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::CUSTOM, 1, m_pCustomBuffer.GetAddressOf());
	m_pContext->GSSetConstantBuffers((UINT)SlotNum::CUSTOM, 1, m_pCustomBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers((UINT)SlotNum::CUSTOM, 1, m_pCustomBuffer.GetAddressOf());
}

void ConstantBufferManager::UpdateFrameConstantBuffer()
{
	// フレーム更新定数バッファの更新
	m_pContext->UpdateSubresource(m_pFrameBuffer.Get(), 0, nullptr, &m_frameCB, 0, 0);
	m_pContext->VSSetConstantBuffers((UINT)SlotNum::FRAME, 1, m_pFrameBuffer.GetAddressOf());
	m_pContext->GSSetConstantBuffers((UINT)SlotNum::FRAME, 1, m_pFrameBuffer.GetAddressOf());
	m_pContext->PSSetConstantBuffers((UINT)SlotNum::FRAME, 1, m_pFrameBuffer.GetAddressOf());
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
		case ConstantBufferManager::SlotNum::WORLD:
			// WVP定数バッファの作成
			cbDesc.ByteWidth = sizeof(WorldConstantBuffer);
			hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_pWorldBuffer.GetAddressOf());
			break;

		case ConstantBufferManager::SlotNum::FRAME:
			// フレーム更新定数バッファの作成
			cbDesc.ByteWidth = sizeof(FrameConstantBuffer);
			hr = m_pDevice->CreateBuffer(&cbDesc, nullptr, m_pFrameBuffer.GetAddressOf());
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
