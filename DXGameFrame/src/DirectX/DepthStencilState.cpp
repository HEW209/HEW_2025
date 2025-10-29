//DepthStencilState.cpp
#include <DirectX/DepthStencilState.h>

DepthStencilState::DepthStencilState() :
	m_currentMode(Mode::COUNT)
{
	for (auto& state : m_pStates)
	{
		state = nullptr;
	}
}

void DepthStencilState::Uninit()
{
	for (auto& state : m_pStates)
	{
		state.Reset();
	}
}

void DepthStencilState::SetMode(Mode depthStencilMode)
{
	//深度ステンシルモードのチェック
	if (depthStencilMode == Mode::COUNT || depthStencilMode == m_currentMode)
		return;

	if (m_pContext == nullptr)
		return;

	m_currentMode = depthStencilMode;

	//ステートを変更
	m_pContext->OMSetDepthStencilState(
		m_pStates[depthStencilMode].Get(), 0);
}

HRESULT DepthStencilState::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr;		//関数の結果
	D3D11_DEPTH_STENCIL_DESC dsDesc;		//深度ステンシルステート設定情報

	for (int i = 0; i < Mode::COUNT; i++)
	{
		//ラスタライザーステートの設定
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		switch ((Mode)i)
		{
		case Mode::DEFAULT:
			//深度テストON 書き込みON (通常)
			dsDesc.DepthEnable = TRUE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = FALSE;
			break;

		case Mode::READ_ONLY:
			//深度テストON 書き込みOFF (半透明用)
			dsDesc.DepthEnable = TRUE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			dsDesc.StencilEnable = FALSE;
			break;

		case Mode::DISABLE:
			//深度テストOFF (UI・2D用)
			dsDesc.DepthEnable = FALSE;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.StencilEnable = FALSE;
			break;

		case Mode::STENCIL:
			//ステンシルON
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
		}

		//ラスタライザーステートの作成
		hr = m_pDevice->CreateDepthStencilState(
			&dsDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) return hr;
	}

	return hr;
}