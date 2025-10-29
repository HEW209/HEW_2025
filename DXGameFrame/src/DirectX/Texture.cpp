//Texture.cpp
#include <DirectX/Texture.h>
#include <DirectX/Debug.h>
#include <DirectX/Direct3D.h>
#include <DirectXTex/TextureLoad.h>

Texture::Texture():
	m_pSRV(nullptr)
{
}

void Texture::Load(const std::string& filePath)
{
	//テクスチャ読み込み
	if (FAILED(LoadTextureFromFile(
		Direct3D::Instance().GetDevice(),
		filePath.c_str(),
		m_pSRV.GetAddressOf()
	)))
	{
		Debug::ErrorMessage(filePath + "の読み込みに失敗しました。");
		exit(0);
	}
}

void Texture::Bind(UINT slot)
{
	Direct3D::Instance().GetContext()->PSSetShaderResources(slot, 1, m_pSRV.GetAddressOf());
}
