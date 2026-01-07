// GeometryShader.cpp
#include <DirectX/GeometryShader.h>
#include <DirectX/Direct3D.h>
#include <DirectX/ShaderManager.h>

GeometryShader::GeometryShader() :
	m_pGS(nullptr)
{
}

void GeometryShader::Bind()
{
	ShaderManager::Instance().SetGeometryShader(m_pGS.Get());
}

HRESULT GeometryShader::LoadFromCSOFile(const std::string& filePath)
{
	HRESULT hr;			// 関数の結果
	FILE* fp;			// ファイルポインタ
	char* data;			// ファイル読み込み先
	long dataSize;		// ファイルサイズ

	// csoファイルを開く
	fopen_s(&fp, filePath.c_str(), "rb");
	if (fp == nullptr)
		return E_FAIL;

	// ファイルサイズを取得
	fseek(fp, 0, SEEK_END);
	dataSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ファイル読み込み
	data = new char[dataSize];
	fread_s(data, dataSize, 1, dataSize, fp);
	fclose(fp);

	// ジオメトリシェーダーを作成
	hr = CreateGeometryShader(data, dataSize);

	// メモリ解放
	delete[] data;

	return hr;
}

HRESULT GeometryShader::CreateGeometryShader(const char* data, int dataSize)
{
	HRESULT hr;		// 関数の結果

	// ジオメトリシェーダーを作成
	hr = Direct3D::Instance().GetDevice()->CreateGeometryShader(
		data,
		dataSize,
		nullptr,
		m_pGS.GetAddressOf()
	);

	return hr;
}