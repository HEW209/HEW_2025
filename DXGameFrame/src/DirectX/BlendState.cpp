// BlendState.cpp
#include <DirectX/BlendState.h>

BlendState::BlendState() :
	m_currentMode(Mode::COUNT)
{
	for (auto& state : m_pStates)
	{
		state = nullptr;
	}
}

void BlendState::Uninit()
{
	for (auto& state : m_pStates)
	{
		state.Reset();
	}
}

void BlendState::SetMode(Mode blendMode)
{
	// ブレンドモードのチェック
	if (blendMode == Mode::COUNT || blendMode == m_currentMode)
		return;

	if (m_pContext == nullptr)
		return;

	float blendFactor[4] = { 0,0,0,0 };
	UINT sampleMask = 0xffffffff;
	
	// ステートを変更
	m_pContext->OMSetBlendState(
		m_pStates[blendMode].Get(), blendFactor, sampleMask);

	m_currentMode = blendMode;
}

HRESULT BlendState::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr;		// 関数の結果
	D3D11_BLEND_DESC blendDesc;		// ブレンドステート設定情報

	for (int i = 0; i < Mode::COUNT; i++)
	{
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		auto& rt = blendDesc.RenderTarget[0];

		// ブレンドステートの設定
		switch ((Mode)i)
		{
		case Mode::DEFAULT:
			// ブレンド無し
			rt.BlendEnable = FALSE;
			rt.SrcBlend = D3D11_BLEND_ONE;
			rt.DestBlend = D3D11_BLEND_ZERO;
			rt.BlendOp = D3D11_BLEND_OP_ADD;
			rt.SrcBlendAlpha = D3D11_BLEND_ONE;
			rt.DestBlendAlpha = D3D11_BLEND_ZERO;
			rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

		case Mode::ALPHA:
			// アルファブレンド
			rt.BlendEnable = TRUE;
			rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			rt.BlendOp = D3D11_BLEND_OP_ADD;
			rt.SrcBlendAlpha = D3D11_BLEND_ONE;
			rt.DestBlendAlpha = D3D11_BLEND_ZERO;
			rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

		case Mode::ADD:
			// 加算
			rt.BlendEnable = TRUE;
			rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rt.DestBlend = D3D11_BLEND_ONE;
			rt.BlendOp = D3D11_BLEND_OP_ADD;
			rt.SrcBlendAlpha = D3D11_BLEND_ONE;
			rt.DestBlendAlpha = D3D11_BLEND_ZERO;
			rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

		case Mode::SUB:
			// 減算
			rt.BlendEnable = TRUE;
			rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			rt.DestBlend = D3D11_BLEND_ONE;
			rt.BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
			rt.SrcBlendAlpha = D3D11_BLEND_ONE;
			rt.DestBlendAlpha = D3D11_BLEND_ZERO;
			rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;

		case Mode::MUL:
			// 乗算
			rt.BlendEnable = TRUE;
			rt.SrcBlend = D3D11_BLEND_DEST_COLOR;
			rt.DestBlend = D3D11_BLEND_ZERO;
			rt.BlendOp = D3D11_BLEND_OP_ADD;
			rt.SrcBlendAlpha = D3D11_BLEND_ONE;
			rt.DestBlendAlpha = D3D11_BLEND_ZERO;
			rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			break;
		}

		// ブレンドステートの作成
		hr = m_pDevice->CreateBlendState(
			&blendDesc, m_pStates[i].GetAddressOf());
		if (FAILED(hr)) return hr;
	}

	return hr;
}