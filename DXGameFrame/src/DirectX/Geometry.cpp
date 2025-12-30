// Geometry.cpp
#include <DirectX/Geometry.h>

HRESULT Geometry::Init()
{
	HRESULT hr = S_OK;

	// ジオメトリ図形を作成
	hr = CreateBox();
	if (FAILED(hr)) { return hr; }
	hr = CreatePlane();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Geometry::Uninit()
{
	for (int i = 0; i < Type::COUNT; ++i)
	{
		m_pModels[i] = nullptr;
	}
}

std::shared_ptr<Model> Geometry::GetModel(Type geometryType)
{
	return m_pModels[geometryType];
}

HRESULT Geometry::CreateBox()
{
	HRESULT hr = S_OK;		// 関数の結果

	const UINT face_count = 6;		// 面の数
	const UINT faceVtx_count = 4;	// 面の頂点数
	const UINT faceIdx_count = 6;	// 面のインデックス数
	const float h = 0.5f;			// 半分のサイズ

	struct Face
	{
		DirectX::XMFLOAT3 vtxPos[faceVtx_count];	// 頂点座標
		DirectX::XMFLOAT3 normal;					// 法線方向
	};

	//面データの作成
	Face face[face_count] = {
		{{{-h, h,-h},{ h, h,-h},{-h,-h,-h},{ h,-h,-h}},{ 0.0f, 0.0f,-1.0f}},		//-Z面
		{{{ h, h, h},{-h, h, h},{ h,-h, h},{-h,-h, h}},{ 0.0f, 0.0f, 1.0f}},		// Z面
		{{{-h, h, h},{-h, h,-h},{-h,-h, h},{-h,-h,-h}},{-1.0f, 0.0f, 0.0f}},		//-X面
		{{{ h, h,-h},{ h, h, h},{ h,-h,-h},{ h,-h, h}},{ 1.0f, 0.0f, 0.0f}},		// X面
		{{{ h,-h, h},{-h,-h, h},{ h,-h,-h},{-h,-h,-h}},{ 0.0f,-1.0f, 0.0f}},		//-Y面
		{{{-h, h, h},{ h, h, h},{-h, h,-h},{ h, h,-h}},{ 0.0f, 1.0f, 0.0f}}			// Y面
	};

	//面共通のUV座標
	DirectX::XMFLOAT2 uv[4] = {
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f},
	};

	// バッファの作成
	Mesh::Description desc = {};

	//頂点データを作成
	desc.vtx.resize(face_count * faceVtx_count);
	for (UINT i = 0; i < face_count; i++)
	{
		for (UINT j = 0; j < faceVtx_count; j++)
		{
			UINT index = i * faceVtx_count + j;		//配列上の位置
			desc.vtx[index].pos = face[i].vtxPos[j];
			desc.vtx[index].normal = face[i].normal;
			desc.vtx[index].uv = uv[j];
			desc.vtx[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}

	//インデックスデータを作成
	desc.idx.resize(face_count * faceIdx_count);
	for (UINT i = 0; i < face_count; i++)
	{
		UINT index = i * faceIdx_count;		//配列上の位置
		UINT offset = i * faceVtx_count;		//面ごとのインデックス番号のずれ

		desc.idx[index++] = offset;
		desc.idx[index++] = offset + 1;
		desc.idx[index++] = offset + 2;
		desc.idx[index++] = offset + 1;
		desc.idx[index++] = offset + 3;
		desc.idx[index++] = offset + 2;
	}

	//その他のデータを設定
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// モデルを作成
	auto model = std::make_shared<Model>();
	hr = model->CreateMesh(desc);
	if (FAILED(hr)) { return hr; };

	m_pModels[Type::BOX] = model;

	return hr;
}

HRESULT Geometry::CreateCylinder()
{
	return E_NOTIMPL;
}

HRESULT Geometry::CreateSphere()
{
	return E_NOTIMPL;
}

HRESULT Geometry::CreatePlane()
{
	HRESULT hr = S_OK;		// 関数の結果

	struct Vtx
	{
		DirectX::XMFLOAT3 pos;			// 頂点座標
		DirectX::XMFLOAT2 uv;			// UV座標
	};

	Vtx vtx[4]
	{
		{{-0.5f, 0.0f,-0.5f}, {0.0f,0.0f} },
		{{ 0.5f, 0.0f,-0.5f}, {1.0f,0.0f} },
		{{-0.5f, 0.0f, 0.5f}, {0.0f,1.0f} },
		{{ 0.5f, 0.0f, 0.5f}, {1.0f,1.0f} }
	};

	int idx[6] = { 0,1,2,1,3,2 };

	// バッファの作成
	Mesh::Description desc = {};

	//頂点データを作成
	desc.vtx.resize(4);
	for (UINT i = 0; i < 4; i++)
	{
		desc.vtx[i].pos = vtx[i].pos;
		desc.vtx[i].normal = { 0.0f, 1.0f, 0.0f };
		desc.vtx[i].uv = vtx[i].uv;
		desc.vtx[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	//インデックスデータを作成
	desc.idx.resize(6);
	for (UINT i = 0; i < 6; i++)
	{
		desc.idx[i] = idx[i];
	}

	//その他のデータを設定
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// モデルを作成
	auto model = std::make_shared<Model>();
	hr = model->CreateMesh(desc);
	if (FAILED(hr)) { return hr; };

	m_pModels[Type::PLANE] = model;

	return hr;
}

Geometry& Geometry::Instance()
{
	static Geometry s_instance;
	return s_instance;
}
