// DepthStencilStateManager.cpp
#include <DirectX/DepthStencilStateManager.h>

DepthStencilStateManager::DepthStencilStateManager() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_currentState(DepthStencilState::COUNT)
{
}

HRESULT DepthStencilStateManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	// 全てのステートを作成
	hr = CreateAllState();
	if (FAILED(hr)) { return hr; }

	// デフォルトステートをセット
	SetState(DepthStencilState::DEFAULT);

	return hr;
}

void DepthStencilStateManager::Uninit()
{
	for (int i = 0; i < (UINT)DepthStencilState::COUNT; ++i)
	{
		m_pStates[i].Reset();
	}
}

void DepthStencilStateManager::SetState(DepthStencilState state)
{
	// 深度ステンシルステートのチェック
	if (state == DepthStencilState::COUNT || state == m_currentState)
		return;

	if (m_pContext == nullptr)
		return;

	// ステートを変更
	m_currentState = state;
	m_pContext->OMSetDepthStencilState(m_pStates[(UINT)state].Get(), 1);
}

HRESULT DepthStencilStateManager::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr = S_OK;		// 関数の結果
	D3D11_DEPTH_STENCIL_DESC dsDesc;		// 深度ステンシルステート設定情報

	// 深度ステンシルステートの作成
	for (int i = 0; i < (UINT)DepthStencilState::COUNT; ++i)
	{
		// 深度ステンシルステートの設定
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		switch ((DepthStencilState)i)
		{
		case DepthStencilState::DEFAULT:
			// 深度テストON 書き込みON (通常)
			dsDesc.DepthEnable = TRUE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = FALSE;
			break;

		case DepthStencilState::READ_ONLY:
			// 深度テストON 書き込みOFF (半透明用)
			dsDesc.DepthEnable = TRUE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			dsDesc.StencilEnable = FALSE;
			break;

		case DepthStencilState::DISABLE:
			// 深度テストOFF (UI・2D用)
			dsDesc.DepthEnable = FALSE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.StencilEnable = FALSE;
			break;

		case DepthStencilState::STENCIL:
			// ステンシルON
			dsDesc.DepthEnable = TRUE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = TRUE;
			dsDesc.StencilReadMask = 0xFF;
			dsDesc.StencilWriteMask = 0xFF;

			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			dsDesc.BackFace = dsDesc.FrontFace;
			break;

		case DepthStencilState::DISABLE_STENCIL:
			// 深度テストOFF、ステンシルON
			dsDesc.DepthEnable = FALSE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.StencilEnable = TRUE;
			dsDesc.StencilReadMask = 0xFF;
			dsDesc.StencilWriteMask = 0xFF;

			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			dsDesc.BackFace = dsDesc.FrontFace;
			break;

		case DepthStencilState::OUTLINE:
			// アウトライン描画用
			dsDesc.DepthEnable = TRUE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = TRUE;
			dsDesc.StencilReadMask = 0xFF;
			dsDesc.StencilWriteMask = 0xFF;

			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

			dsDesc.BackFace = dsDesc.FrontFace;
			break;
		}

		// 深度ステンシルステートの作成
		hr = m_pDevice->CreateDepthStencilState(
			&dsDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) { return hr; }
	}

	return hr;
}