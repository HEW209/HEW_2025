// PipelineStateManager.cpp
#include <DirectX/PipelineStateManager.h>

HRESULT PipelineStateManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr = S_OK;		// 関数の結果

	// ラスタライザーステートの初期化
	hr = m_rasterizerStateManager.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	// 深度ステンシルステートの初期化
	hr = m_depthStencilStateManager.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	// サンプラーステートの初期化
	hr = m_samplerStateManager.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	// ブレンドステートの初期化
	hr = m_blendStateManager.Init(pDevice, pContext);
	if (FAILED(hr)) { return hr; }

	return hr;
}

void PipelineStateManager::Uninit()
{
	// 各ステートの終了処理
	m_rasterizerStateManager.Uninit();
	m_depthStencilStateManager.Uninit();
	m_samplerStateManager.Uninit();
	m_blendStateManager.Uninit();
}

void PipelineStateManager::SetPipelineState(
	RasterizerState rasterizerState,
	DepthStencilState depthStencilState,
	SamplerState samplerState,
	BlendState blendState)
{
	// 各ステートを設定する
	m_rasterizerStateManager.SetState(rasterizerState);
	m_depthStencilStateManager.SetState(depthStencilState);
	m_samplerStateManager.SetState(samplerState, 0);
	m_blendStateManager.SetState(blendState);
}

void PipelineStateManager::SetPipelineState(PipelineState pipelineState)
{
	SetPipelineState(
		pipelineState.rasterizerState,
		pipelineState.depthStencilState,
		pipelineState.samplerState,
		pipelineState.blendState
	);
}

void PipelineStateManager::SetRasterizerState(RasterizerState rasterizerState)
{
	m_rasterizerStateManager.SetState(rasterizerState);
}

void PipelineStateManager::SetDepthStencilState(DepthStencilState depthStencilState)
{
	m_depthStencilStateManager.SetState(depthStencilState);
}

void PipelineStateManager::SetSamplerState(SamplerState samplerState, UINT slotNum)
{
	m_samplerStateManager.SetState(samplerState, slotNum);
}

void PipelineStateManager::SetBlendState(BlendState blendState)
{
	m_blendStateManager.SetState(blendState);
}

PipelineStateManager& PipelineStateManager::Instance()
{
	static PipelineStateManager s_instance;
	return s_instance;
}
