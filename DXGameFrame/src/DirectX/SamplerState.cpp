//SamplerState.cpp
#include <DirectX/SamplerState.h>

SamplerState::SamplerState() :
	m_currentMode(Mode::COUNT)
{
	for (auto& state : m_pStates)
	{
		state = nullptr;
	}
}

void SamplerState::Uninit()
{
	for (auto& state : m_pStates)
	{
		state.Reset();
	}
}

void SamplerState::SetMode(Mode samplerMode, UINT slotNum)
{
	//サンプラーモードのチェック
	if (samplerMode == Mode::COUNT || samplerMode == m_currentMode)
		return;

	if (m_pContext == nullptr)
		return;

	m_currentMode = samplerMode;

	//ステートを変更
	m_pContext->PSSetSamplers(
		slotNum, 1, m_pStates[samplerMode].GetAddressOf());
}

HRESULT SamplerState::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr;		//関数の結果
	D3D11_SAMPLER_DESC samplerDesc;		//サンプラーステート設定情報

	for (int i = 0; i < Mode::COUNT; i++)
	{
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		//サンプラーステートの設定
		switch ((Mode)i)
		{
		case Mode::LINEAR_WRAP:
			//線形補間 + 繰り返し
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case Mode::LINEAR_CLAMP:
			//線形補間 + 端固定
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case Mode::POINT_WRAP:
			//最近傍 + 繰り返し
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case Mode::POINT_CLAMP:
			//最近傍 + 端固定
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			break;

		case Mode::ANISOTROPIC_WRAP:
			//異方性 + 繰り返し
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MaxAnisotropy = 16;
			break;

		case Mode::ANISOTROPIC_CLAMP:
			//異方性 + 端固定
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = samplerDesc.AddressU;
			samplerDesc.AddressW = samplerDesc.AddressU;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MaxAnisotropy = 16;
			break;
		
		case Mode::BORDER_BLACK:
			//線形補間 + 枠外黒
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

		//サンプラーステートの作成
		hr = m_pDevice->CreateSamplerState(
			&samplerDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) return hr;
	}

	return hr;
}