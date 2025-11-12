// Texture.cpp
#include <DirectX/Texture.h>
#include <DirectX/Debug.h>
#include <DirectX/Direct3D.h>
#include <DirectXTex/TextureLoad.h>

Texture::Texture():
	m_pSRV(nullptr)
{
}

HRESULT Texture::Load(const std::string& filePath)
{
	HRESULT hr = S_OK;		// 関数の結果

	//テクスチャ読み込み
	hr = LoadTextureFromFile(
		Direct3D::Instance().GetDevice(),
		filePath.c_str(),
		m_pSRV.GetAddressOf()
	);

	if (FAILED(hr))
	{
		Debug::ErrorMessage(filePath + "の読み込みに失敗しました。");
		return hr;
	}

	return hr;
}

void Texture::Bind(UINT slot)
{
	// スロット番号チェック
	if (slot >= TextureSlot::Count)
		return;

	// テクスチャをセット
	Direct3D::Instance().GetContext()->PSSetShaderResources(slot, 1, m_pSRV.GetAddressOf());
}
