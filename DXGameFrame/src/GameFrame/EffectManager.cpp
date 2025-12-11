#include <GameFrame/EffectManager.h>
#include <Component/Camera.h>
#include <Effekseer/EffekseerRendererDX11.h>
#include <GameFrame/Transform.h>
#include <DirectX/Direct3D.h>
#include <Component/EffectRenderer.h>
void EffectManager::Init()
{
	m_effectManager = Effekseer::Manager::Create(8000);
	//セットアップ
	SetupEffekseerModules();

}

EffectManager& EffectManager::Instance()
{
	static EffectManager s_instance;
	return s_instance;
}

void EffectManager::BeginDraw()
{
	static float begintime = 0.0f;
	begintime += 1.0f / 60.0f;
	m_effectRendererRef->SetTime(begintime);
	Effekseer::Manager::LayerParameter layerParameter;

	auto camera = Camera::GetMain();

	if (camera) {
		Vector3 cameraPos = camera->GetTransform()->GetPosition();
		layerParameter.ViewerPosition.X = cameraPos.x;
		layerParameter.ViewerPosition.Y = cameraPos.y;
		layerParameter.ViewerPosition.Z = cameraPos.z;

		m_effectRendererRef->SetProjectionMatrix(ConvertXMMatrixToEffekseerMatrix44(camera->GetProjectionMatrix()));
		m_effectRendererRef->SetCameraMatrix(ConvertXMMatrixToEffekseerMatrix44(camera->GetViewMatrix()));
	}

	m_effectManager->SetLayerParameter(0, layerParameter);

	Effekseer::Manager::UpdateParameter updateParameter;
	m_effectManager->Update(updateParameter);



	m_effectRendererRef->BeginRendering();

}

void EffectManager::EndDraw()
{
	m_effectRendererRef->EndRendering();
}


void EffectManager::SetupEffekseerModules()
{
	auto& instance = Direct3D::Instance();
	// Create a  graphics device
	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX11::CreateGraphicsDevice(instance.GetDevice(), instance.GetContext());

	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	m_effectRendererRef = ::EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	m_effectManager->SetSpriteRenderer(m_effectRendererRef->CreateSpriteRenderer());
	m_effectManager->SetRibbonRenderer(m_effectRendererRef->CreateRibbonRenderer());
	m_effectManager->SetRingRenderer(m_effectRendererRef->CreateRingRenderer());
	m_effectManager->SetTrackRenderer(m_effectRendererRef->CreateTrackRenderer());
	m_effectManager->SetModelRenderer(m_effectRendererRef->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_effectManager->SetTextureLoader(m_effectRendererRef->CreateTextureLoader());
	m_effectManager->SetModelLoader(m_effectRendererRef->CreateModelLoader());
	m_effectManager->SetMaterialLoader(m_effectRendererRef->CreateMaterialLoader());
	m_effectManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	//// Specify sound modules
	//// サウンドモジュールの設定
	//efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	//// Specify a metho to play sound from an instance of efkSound
	//// 音再生用インスタンスから再生機能を指定
	//m_effectManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	//// Specify a sound data loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// サウンドデータの読込機能を設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//m_effectManager->SetSoundLoader(efkSound->CreateSoundLoader());
}

