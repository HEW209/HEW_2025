//RasterizerState.cpp
#include <DirectX/RasterizerState.h>

RasterizerState::RasterizerState() :
	m_currentMode(Mode::COUNT)
{
	for (auto& state : m_pStates)
	{
		state = nullptr;
	}
}

void RasterizerState::Uninit()
{
	for (auto& state : m_pStates)
	{
		state.Reset();
	}
}

void RasterizerState::SetMode(Mode rasterizeMode)
{
	//ラスタライズモードのチェック
	if (rasterizeMode == Mode::COUNT || rasterizeMode == m_currentMode)
		return;

	if (m_pDevice == nullptr)
		return;

	//ステートを変更
	m_pContext->RSSetState(
		m_pStates[rasterizeMode].Get());

	m_currentMode = rasterizeMode;
}

HRESULT RasterizerState::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr;		//関数の結果
	D3D11_RASTERIZER_DESC rasterizerDesc;		//ラスタライザーステート設定情報

	for (int i = 0; i < Mode::COUNT; i++)
	{
		//ラスタライザーステートの設定
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		switch ((Mode)i)
		{
		case Mode::DEFAULT:
			//前面描画
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			break;

		case Mode::BACK:
			//背面描画
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			break;

		case Mode::NONE:
			//両面描画
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			break;

		case Mode::SCISSOR:
			//シザー矩形を使用 (両面描画)
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			rasterizerDesc.ScissorEnable = TRUE;
			break;

		case Mode::WIREFRAME:
			//ワイヤーフレーム
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = TRUE;
			rasterizerDesc.AntialiasedLineEnable = TRUE;
			break;
		}

		//ラスタライザーステートの作成
		hr = m_pDevice->CreateRasterizerState(
			&rasterizerDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) return hr;
	}

	return hr;
}