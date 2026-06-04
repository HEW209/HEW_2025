// RasterizerStateManager.cpp
#include <DirectX/RasterizerStateManager.h>

RasterizerStateManager::RasterizerStateManager() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_currentState(RasterizerState::COUNT)
{
}

HRESULT RasterizerStateManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	// 全てのステートを作成
	hr = CreateAllState();
	if (FAILED(hr)) { return hr; }

	// デフォルトステートをセット
	SetState(RasterizerState::DEFAULT);

	return hr;
}

void RasterizerStateManager::Uninit()
{
	for (int i = 0; i < (UINT)RasterizerState::COUNT; ++i)
	{
		m_pStates[i].Reset();
	}
}

void RasterizerStateManager::SetState(RasterizerState state)
{
	// ラスタライザーステートのチェック
	if (state == RasterizerState::COUNT || state == m_currentState)
		return;

	if (m_pContext == nullptr)
		return;

	// ステートを変更
	m_currentState = state;
	m_pContext->RSSetState(m_pStates[(UINT)state].Get());
}

HRESULT RasterizerStateManager::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr = S_OK;		// 関数の結果
	D3D11_RASTERIZER_DESC rasterizerDesc;		// ラスタライザーステート設定情報

	for (int i = 0; i < (UINT)RasterizerState::COUNT; i++)
	{
		// ラスタライザーステートの設定
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		switch ((RasterizerState)i)
		{
		case RasterizerState::DEFAULT:
			// 前面描画
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			break;

		case RasterizerState::BACK:
			// 背面描画
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			break;

		case RasterizerState::NONE:
			// 両面描画
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			break;

		case RasterizerState::SCISSOR:
			// シザー矩形を使用 (両面描画)
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			rasterizerDesc.ScissorEnable = TRUE;
			break;

		case RasterizerState::WIREFRAME:
			// ワイヤーフレーム
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			rasterizerDesc.AntialiasedLineEnable = TRUE;
			break;

		case RasterizerState::SHADOW:
			// シャドウマップ用 (前面描画、深度バイアス有り)
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			rasterizerDesc.DepthBias = 100;
			rasterizerDesc.DepthBiasClamp = 0.0f;
			rasterizerDesc.SlopeScaledDepthBias = 1.0f;
			break;
		}

		// ラスタライザーステートの作成
		hr = m_pDevice->CreateRasterizerState(
			&rasterizerDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) { return hr; }
	}

	return hr;
}