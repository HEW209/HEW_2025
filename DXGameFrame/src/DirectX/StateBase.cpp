//StateBase.cpp
#include <DirectX/StateBase.h>

StateBase::StateBase() :
	m_pDevice(nullptr),
	m_pContext(nullptr)
{
}

HRESULT StateBase::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	HRESULT hr;		//関数の結果

	m_pDevice = pDevice;
	m_pContext = pContext;

	//ステートを作成
	hr = CreateAllState();
	if (FAILED(hr)) { return hr; }

	return hr;
}