//Geometry.cpp
#include <DirectX/Geometry.h>
#include <DirectX/Mesh.h>

HRESULT Geometry::Init()
{
	HRESULT hr = S_OK;

	hr = CreateBox();
	if (FAILED(hr)) { return hr; }

	return hr;
}

void Geometry::Uninit()
{
	m_pBoxMesh = nullptr;
}

HRESULT Geometry::CreateBox()
{
	HRESULT hr = S_OK;		//関数の結果

	const int face_count = 6;		//面の数
	const int faceVtx_count = 4;	//面の頂点数
	const int faceIdx_count = 6;	//面のインデックス数
	const float h = 0.5f;				//半分のサイズ

	struct Face
	{
		DirectX::XMFLOAT3 vtxPos[faceVtx_count];	//頂点座標
		DirectX::XMFLOAT3 normal;					//法線方向
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
	for (int i = 0; i < face_count; i++)
	{
		for (int j = 0; j < faceVtx_count; j++)
		{
			int index = i * faceVtx_count + j;		//配列上の位置
			desc.vtx[index].pos = face[i].vtxPos[j];
			desc.vtx[index].normal = face[i].normal;
			desc.vtx[index].uv = uv[j];
			desc.vtx[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}

	//インデックスデータを作成
	desc.idx.resize(face_count * faceIdx_count);
	for (int i = 0; i < face_count; i++)
	{
		int index = i * faceIdx_count;		//配列上の位置
		int offset = i * faceVtx_count;		//面ごとのインデックス番号のずれ

		desc.idx[index++] = offset;
		desc.idx[index++] = offset + 1;
		desc.idx[index++] = offset + 2;
		desc.idx[index++] = offset + 1;
		desc.idx[index++] = offset + 3;
		desc.idx[index++] = offset + 2;
	}

	//その他のデータを設定
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pBoxMesh = std::make_shared<Mesh>();

	//メッシュを作成
	hr = m_pBoxMesh->Create(desc);
	if (FAILED(hr)) { return hr; };

    return hr;
}