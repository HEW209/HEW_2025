// BlendStateManager.cpp
#include <DirectX/BlendStateManager.h>

BlendStateManager::BlendStateManager() :
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_currentState(BlendState::COUNT)
{
}

HRESULT BlendStateManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	// 全てのステートを作成
	hr = CreateAllState();
	if (FAILED(hr)) { return hr; }

	// デフォルトステートをセット
	SetState(BlendState::DEFAULT);

	return hr;
}

void BlendStateManager::Uninit()
{
	for (int i = 0; i < (UINT)BlendState::COUNT; ++i)
	{
		m_pStates[i].Reset();
	}
}

void BlendStateManager::SetState(BlendState state)
{
	// ブレンドステートのチェック
	if (state == BlendState::COUNT || state == m_currentState)
		return;

	if (m_pContext == nullptr)
		return;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	// ステートを変更
	m_currentState = state;
	m_pContext->OMSetBlendState(m_pStates[(UINT)state].Get(), blendFactor, sampleMask);
}

HRESULT BlendStateManager::CreateAllState()
{
	if (m_pDevice == nullptr)
		return S_FALSE;

	HRESULT hr = S_OK;		// 関数の結果
	D3D11_BLEND_DESC blendDesc;		// ブレンドステート設定情報

	for (int i = 0; i < (UINT)BlendState::COUNT; ++i)
	{
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		auto& rt = blendDesc.RenderTarget[0];

		// ブレンドステートの設定
		switch ((BlendState)i)
		{
		case BlendState::DEFAULT:
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

		case BlendState::ALPHA:
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

		case BlendState::ADD:
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

		case BlendState::SUB:
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

		case BlendState::MUL:
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
		if (FAILED(hr)) { return hr; }
	}

	return hr;
}
