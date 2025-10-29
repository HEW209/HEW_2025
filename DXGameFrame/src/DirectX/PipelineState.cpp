//PipelineState.cpp
#include <DirectX/PipelineState.h>

HRESULT PipelineState::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr;		//関数の結果

	//ラスタライザーステートの初期化
	hr = m_rasterizerState.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	//深度ステンシルステートの初期化
	hr = m_depthStencilState.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	//ブレンドステートの初期化
	hr = m_blendState.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	//サンプラーステートの初期化
	hr = m_samplerState.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	return hr;
}

void PipelineState::Uninit()
{
	//各ステートの終了処理
	m_rasterizerState.Uninit();
	m_depthStencilState.Uninit();
	m_blendState.Uninit();
	m_samplerState.Uninit();
}

void PipelineState::SetPipelineMode(
	RasterizerState::Mode rasterizeMode,
	DepthStencilState::Mode depthStencilMode,
	BlendState::Mode blendMode,
	SamplerState::Mode samplerMode)
{
	//各ステートモードを設定する
	m_rasterizerState.SetMode(rasterizeMode);
	m_depthStencilState.SetMode(depthStencilMode);
	m_blendState.SetMode(blendMode);
	m_samplerState.SetMode(samplerMode, 0);
}

void PipelineState::SetRasterizeMode(RasterizerState::Mode rasterizeMode)
{
	//ラスタライズモードを設定する
	m_rasterizerState.SetMode(rasterizeMode);
}

void PipelineState::SetDepthStencilMode(DepthStencilState::Mode depthStencilMode)
{
	//深度ステンシルモードを設定する
	m_depthStencilState.SetMode(depthStencilMode);
}

void PipelineState::SetBlendMode(BlendState::Mode blendMode)
{
	//ブレンドモードを設定する
	m_blendState.SetMode(blendMode);
}

void PipelineState::SetSamplerMode(SamplerState::Mode samplerMode, UINT slotNum)
{
	//サンプラーモードを設定する
	m_samplerState.SetMode(samplerMode, slotNum);
}