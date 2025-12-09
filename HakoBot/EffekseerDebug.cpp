#include "EffekseerDebug.h"






EffeksserDebug::EffeksserDebug()
{
	// DirectXデバイスの取得
	ID3D11Device* device = Direct3D::Instance().GetDevice();
	ID3D11DeviceContext* context = Direct3D::Instance().GetContext();

	//マネージャの作成
	m_EfkManager = Effekseer::Manager::Create(8000);

	//effekseer
	SetupEffekseerModules();
	auto viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);
}

void EffeksserDebug::SetupEffekseerModules()
{
	// Create a  graphics devices
	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX11::CreateGraphicsDevice(Direct3D::Instance().GetDevice(), Direct3D::Instance().GetContext());

	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	m_EfkRenderer = ::EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	m_EfkManager->SetSpriteRenderer(m_EfkRenderer->CreateSpriteRenderer());
	m_EfkManager->SetRibbonRenderer(m_EfkRenderer->CreateRibbonRenderer());
	m_EfkManager->SetRingRenderer(m_EfkRenderer->CreateRingRenderer());
	m_EfkManager->SetTrackRenderer(m_EfkRenderer->CreateTrackRenderer());
	m_EfkManager->SetModelRenderer(m_EfkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_EfkManager->SetTextureLoader(m_EfkRenderer->CreateTextureLoader());
	m_EfkManager->SetModelLoader(m_EfkRenderer->CreateModelLoader());
	m_EfkManager->SetMaterialLoader(m_EfkRenderer->CreateMaterialLoader());
	m_EfkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	//// Specify sound modules
	//// サウンドモジュールの設定
	//efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	//// Specify a metho to play sound from an instance of efkSound
	//// 音再生用インスタンスから再生機能を指定
	//efkManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	//// Specify a sound data loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// サウンドデータの読込機能を設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//efkManager->SetSoundLoader(efkSound->CreateSoundLoader());
}

void EffeksserDebug::Update()
{
	if (m_EfkManager != nullptr) {
		m_EfkManager->Update();
	}
	BeginDraw();
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = m_EfkRenderer->GetCameraProjectionMatrix();
	m_EfkManager->Draw(drawParameter);
}


void EffeksserDebug::Draw()
{
	if (m_EfkManager != nullptr) {
		m_EfkManager->Draw();
	}
}

void EffeksserDebug::BeginDraw()
{
	//メインカメラを取得
	Camera* mainCamera = Camera::GetMain();

	if (mainCamera == nullptr || m_EfkRenderer == nullptr)
	{
		Debug::ErrorMessage("Effekseer描画失敗: メインカメラまたはレンダラーが設定されていません");
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
