#include "EffeksserDebug.h"






void EffeksserDebug::SetupEffekseerModules(ID3D11Device* device , ID3D11DeviceContext* context)
{
	
	//描画デバイスの作成
	auto graphicsDevice = EffekseerRendererDX11::CreateGraphicsDevice(device , context);

	//エフェクトのレンダラーの作成
	m_EfkRenderer = EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// 描画モジュールの設定
	m_EfkManager->SetSpriteRenderer(m_EfkRenderer->CreateSpriteRenderer());
	m_EfkManager->SetRibbonRenderer(m_EfkRenderer->CreateRibbonRenderer());
	m_EfkManager->SetRingRenderer(m_EfkRenderer->CreateRingRenderer());
	m_EfkManager->SetTrackRenderer(m_EfkRenderer->CreateTrackRenderer());
	m_EfkManager->SetModelRenderer(m_EfkRenderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_EfkManager->SetTextureLoader(m_EfkRenderer->CreateTextureLoader());
	m_EfkManager->SetModelLoader(m_EfkRenderer->CreateModelLoader());
	m_EfkManager->SetMaterialLoader(m_EfkRenderer->CreateMaterialLoader());
	m_EfkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());


}

EffeksserDebug::EffeksserDebug()
{
	// DirectXデバイスの取得
	ID3D11Device* device = Direct3D::Instance().GetDevice();
	ID3D11DeviceContext* context = Direct3D::Instance().GetContext();

	//マネージャの作成
	m_EfkManager = Effekseer::Manager::Create(8000);

	//Effekseerのモジュールをセットアップする
	SetupEffekseerModules(device, context);
}

void EffeksserDebug::Update()
{
	m_EfkManager->Update();

}

void EffeksserDebug::BeginDraw()
{
	// メインカメラを取得
	Camera* mainCamera = Camera::GetMain();

	if (mainCamera == nullptr || m_EfkRenderer == nullptr)
	{
		// Debug::ErrorMessage("Effekseer描画失敗: メインカメラまたはレンダラーが設定されていません");
		return;
	}

	// 1. DirectXの行列を取得
	DirectX::XMMATRIX dxView = mainCamera->GetViewMatrix();
	DirectX::XMMATRIX dxProjection = mainCamera->GetProjectionMatrix();

	// 2. Effekseerの行列型に変換 (メモリコピー)
	::Effekseer::Matrix44 efkViewMatrix;
	::Effekseer::Matrix44 efkProjectionMatrix;

	// Effekseer::Matrix44のメンバ変数 .Values (float[4][4]) にコピー
	memcpy(efkViewMatrix.Values, &dxView, sizeof(float) * 16);
	memcpy(efkProjectionMatrix.Values, &dxProjection, sizeof(float) * 16);

	// 3. Effekseerレンダラーに設定し、描画開始
	m_EfkRenderer->SetProjectionMatrix(efkProjectionMatrix);
	m_EfkRenderer->SetCameraMatrix(efkViewMatrix);
	m_EfkRenderer->BeginRendering();
}
