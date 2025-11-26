// SamplerStateManager.cpp
#include <DirectX/SamplerStateManager.h>

SamplerStateManager::SamplerStateManager() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_currentState(SamplerState::COUNT)
{
}

HRESULT SamplerStateManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	// 全てのステートを作成
	hr = CreateAllState();
	if (FAILED(hr)) { return hr; }

	// デフォルトステートをセット
	SetState(SamplerState::DEFAULT);

	return hr;
}

void SamplerStateManager::Uninit()
{
	for (int i = 0; i < (UINT)SamplerState::COUNT; ++i)
	{
		m_pStates[i].Reset();
	}
}

void SamplerStateManager::SetState(SamplerState state, UINT slotNum)
{
	// サンプラーステートのチェック
	if (state == SamplerState::COUNT || state == m_currentState)
		return;

	if (m_pContext == nullptr)
		return;

	// ステートを変更
	m_currentState = state;
	m_pContext->PSSetSamplers(slotNum, 1, m_pStates[(UINT)state].GetAddressOf());
}

HRESULT SamplerStateManager::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr = S_OK;		// 関数の結果
	D3D11_SAMPLER_DESC samplerDesc;		// サンプラーステート設定情報

	for (int i = 0; i < (UINT)SamplerState::COUNT; i++)
	{
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		// サンプラーステートの設定
		switch ((SamplerState)i)
		{
		case SamplerState::LINEAR_WRAP:
			// 線形補間 + 繰り返し
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case SamplerState::LINEAR_CLAMP:
			// 線形補間 + 端固定
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case SamplerState::POINT_WRAP:
			// 最近傍 + 繰り返し
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case SamplerState::POINT_CLAMP:
			// 最近傍 + 端固定
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case SamplerState::ANISOTROPIC_WRAP:
			// 異方性 + 繰り返し
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MaxAnisotropy = 16;
			break;

		case SamplerState::ANISOTROPIC_CLAMP:
			// 異方性 + 端固定
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MaxAnisotropy = 16;
			break;

		case SamplerState::BORDER_BLACK:
			// 線形補間 + 枠外黒
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.BorderColor[0] = 0.0f;
			samplerDesc.BorderColor[1] = 0.0f;
			samplerDesc.BorderColor[2] = 0.0f;
			samplerDesc.BorderColor[3] = 1.0f;
			break;
		}

		// サンプラーステートの作成
		hr = m_pDevice->CreateSamplerState(
			&samplerDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) { return hr; }
	}

	return hr;
}