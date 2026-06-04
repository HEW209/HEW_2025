// Texture.cpp
#include <DirectX/Texture.h>
#include <DirectX/Direct3D.h>
#include <DirectX/Debug.h>
#include <DirectXTex/TextureLoad.h>

Texture::Texture() :
	m_pSRV(nullptr),
	m_size{}
{
}

HRESULT Texture::Load(const std::string& filePath)
{
	HRESULT hr = S_OK;		// 関数の結果

	// ファイルからテクスチャを読み込む
	hr = LoadFromFile(filePath);
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

Texture::Size Texture::GetSize()
{
	return m_size;
}

HRESULT Texture::LoadFromFile(const std::string& filePath)
{
	HRESULT hr = S_OK;		// 関数の結果

	// 文字変換 (DirectXTexではwchar_t*を使用します)
	wchar_t wPath[MAX_PATH];
	MultiByteToWideChar(0, 0, filePath.c_str(), -1, wPath, MAX_PATH);

	// ファイル別読み込み
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	if (strstr(filePath.c_str(), ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_IGNORE_SRGB, &mdata, image);
	}
	if (FAILED(hr)) { return hr; }

	// シェーダリソースビューを作成
	hr = CreateShaderResourceView(
		Direct3D::Instance().GetDevice(),
		image.GetImages(),
		image.GetImageCount(),
		mdata,
		m_pSRV.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	// ファイルサイズを保存
	m_size.x = (UINT)mdata.width;
	m_size.y = (UINT)mdata.height;

	return hr;
}
