//VertexShader.cpp
#include <DirectX/VertexShader.h>
#include <DirectX/Direct3D.h>
#include <DirectX/ShaderManager.h>

VertexShader::VertexShader() :
	m_pVS(nullptr),
	m_pInputLayout(nullptr)
{
}

void VertexShader::Bind()
{
	ShaderManager::Instance().SetVertexShader(m_pVS.Get(), m_pInputLayout.Get());
}

HRESULT VertexShader::LoadFromCSOFile(const std::string& filePath)
{
	HRESULT hr;			//関数の結果
	FILE* fp;			//ファイルポインタ
	char* data;			//ファイル読み込み先
	long dataSize;		//ファイルサイズ 

	//csoファイルを開く
	fopen_s(&fp, filePath.c_str(), "rb");
	if (fp == nullptr)
		return E_FAIL;

	//ファイルサイズを取得
	fseek(fp, 0, SEEK_END);
	dataSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//ファイル読み込み
	data = new char[dataSize];
	fread_s(data, dataSize, 1, dataSize, fp);
	fclose(fp);

	//頂点シェーダーを作成
	hr = CreateVertexShader(data, dataSize);

	if (SUCCEEDED(hr))
	{
		//入力レイアウトを作成
		hr = CreateInputLayout(data, dataSize);
	}

	//メモリ解放
	delete[] data;

	return hr;
}

HRESULT VertexShader::CreateVertexShader(const char* data, int dataSize)
{
	HRESULT hr;		//関数の結果

	//頂点シェーダーを作成
	hr = Direct3D::Instance().GetDevice()->CreateVertexShader(
		data,
		dataSize,
		nullptr,
		m_pVS.GetAddressOf()
	);

	return hr;
}

HRESULT VertexShader::CreateInputLayout(const char* data, int dataSize)
{
	HRESULT hr;		//関数の結果

	ComPtr<ID3D11ShaderReflection> reflection;	//シェーダーの反射インターフェース
	reflection = nullptr;

	//シェーダーの反射インターフェースを取得
	hr = D3DReflect(
		data,										//コンパイルされた頂点シェーダー
		dataSize,									//コンパイルされた頂点シェーダーのサイズ
		IID_PPV_ARGS(&reflection)					//↓2つを自動で行う。↓のコードだと"error LNK2001"
		//IID_ID3D11ShaderReflection,				//インターフェースの識別子
		//(void**)reflection.GetAddressOf()			//取得した反射インターフェースを格納する
	);
	if (FAILED(hr)) { return hr; }

	//フォーマット設定
	DXGI_FORMAT formats[][4] =		//フォーマット設定を列挙
	{
		{
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32A32_UINT,
		}, {
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	//シェーダーの入力シグネチャを取得
	D3D11_SHADER_DESC shaderDesc;				//シェーダーの設定情報
	reflection->GetDesc(&shaderDesc);

	//入力レイアウトの設定を作成
	D3D11_INPUT_ELEMENT_DESC* inputDescs;		//入力レイアウトの設定情報
	inputDescs = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];

	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		//入力パラメータの詳細を取得
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;			//入力パラメータの詳細
		reflection->GetInputParameterDesc(i, &paramDesc);

		//入力レイアウトの設定
		inputDescs[i].SemanticName = paramDesc.SemanticName;			//セマンティック名
		inputDescs[i].SemanticIndex = paramDesc.SemanticIndex;			//セマンティックインデックス
		inputDescs[i].InputSlot = 0;									//頂点バッファの入力スロット
		inputDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;		//頂点データのクラス
		inputDescs[i].InstanceDataStepRate = 0;							//インスタンスデータのステップレート

		//バイトオフセット
		if (i == 0)
		{
			inputDescs[i].AlignedByteOffset = 0;
		}
		else
		{
			inputDescs[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		}

		//ビットマスクから存在するコンポーネントの要素数を計算
		BYTE elementCount = paramDesc.Mask;
		elementCount = (elementCount & 0x01) + ((elementCount >> 1) & 0x01)
			+ ((elementCount >> 2) & 0x01) + ((elementCount >> 3) & 0x01);

		//データフォーマット
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			inputDescs[i].Format = formats[0][elementCount - 1];
			break;

		case D3D_REGISTER_COMPONENT_SINT32:
			inputDescs[i].Format = formats[1][elementCount - 1];
			break;

		case D3D_REGISTER_COMPONENT_FLOAT32:
			inputDescs[i].Format = formats[2][elementCount - 1];
			break;
		}
	}

	//頂点入力レイアウト作成
	hr = Direct3D::Instance().GetDevice()->CreateInputLayout(
		inputDescs,
		shaderDesc.InputParameters,
		data,
		dataSize,
		&m_pInputLayout
	);

	//メモリ解放
	delete[] inputDescs;

	if (FAILED(hr)) { return hr; }

	return hr;
}