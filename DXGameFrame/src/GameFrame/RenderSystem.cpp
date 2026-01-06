// RenderSystem.cpp
#include <GameFrame/RenderSystem.h>
#include <GameFrame/GameObject.h>
#include <GameFrame/Transform.h>
#include <DirectX/Direct3D.h>
#include <DirectX/ConstantBufferManager.h>
#include <Component/Camera.h>
#include <Component/DirectionalLight.h>
#include <GameFrame/Time.h>
#include <DirectX/PipelineStateManager.h>
#include <../imgui/ImguiManager.h>
#include <algorithm>
#include <GameFrame/EffectManager.h>
#include <DirectX/ShaderManager.h>
RenderSystem::RenderSystem() :
	m_clearColor(0.4f, 0.4f, 1.0f, 1.0f)
{
}

void RenderSystem::DrawAll()
{
	// 画面クリア
	float clearColor[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a };
	Direct3D::Instance().BeginDraw(clearColor);

	// ライト設定
	DirectionalLight* pDirLight = DirectionalLight::GetMain();
	LightConstantBuffer lightCB = {};
	if (pDirLight != nullptr)
	{
		lightCB = pDirLight->GetLightCB();
	}
	else
	{
		lightCB.lightDir = { 0.0f, -1.0f, 0.0f };
		lightCB.lightColor = { 1.0f, 1.0f, 1.0f };
		lightCB.ambientColor = { 0.4f, 0.4f, 0.4f };
		lightCB.lightIntensity = 1.0f;
	}
	ConstantBufferManager::Instance().SetLight(lightCB);

	// 時間設定
	ConstantBufferManager::Instance().SetTime(Time::GetSceneTime());

	DrawAll3D();
	DrawAll2D();
	DrawDebugUI();

	// 画面に出力
	Direct3D::Instance().Present();
}

void RenderSystem::Register(Renderer* pRenderer)
{
	// コンポーネントを追加
	m_pRendererComponents.emplace_back(pRenderer);
}

void RenderSystem::Unregister(Renderer* pRenderer)
{
	// 削除対象コンポーネントを検索して削除する
	auto it = std::remove(m_pRendererComponents.begin(), m_pRendererComponents.end(), pRenderer);
	m_pRendererComponents.erase(it, m_pRendererComponents.end());
}

void RenderSystem::Register2D(Renderer2D* pRenderer2D)
{
	// コンポーネントを追加
	m_pRenderer2DComponents.emplace_back(pRenderer2D);
}

void RenderSystem::Unregister2D(Renderer2D* pRenderer2D)
{
	// 削除対象コンポーネントを検索して削除する
	auto it = std::remove(m_pRenderer2DComponents.begin(), m_pRenderer2DComponents.end(), pRenderer2D);
	m_pRenderer2DComponents.erase(it, m_pRenderer2DComponents.end());
}

void RenderSystem::SetClearColor(Color color)
{
	m_clearColor = color;
}

void RenderSystem::DrawAll3D()
{
	// カメラ設定
	Camera* pMainCamera = Camera::GetMain();
	if (pMainCamera != nullptr)
	{
		// ビュー行列設定
		DirectX::XMMATRIX view = pMainCamera->GetViewMatrix();
		ConstantBufferManager::Instance().SetView(view);

		// プロジェクション行列設定
		DirectX::XMMATRIX projection = pMainCamera->GetProjectionMatrix();
		ConstantBufferManager::Instance().SetProjection(projection);
	}

	// フレーム定数バッファを更新
	ConstantBufferManager::Instance().UpdateFrameConstantBuffer();
	EffectManager::Instance().BeginDraw();

	PipelineStateManager::Instance().Refresh();
	ShaderManager::Instance().Reflesh();
	// 3D描画処理
	for (auto* renderer : m_pRendererComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			!renderer->IsTransparent())
		{
			renderer->Draw();
		}
	}

	// 3D透過オブジェクト描画処理
	// 描画順ソートの準備
	struct TransparentRendererInfo
	{
		Renderer* pRenderer;
		float cameraDistance = 0.0f;
	};
	std::vector<TransparentRendererInfo> transparentRenderer;		// 透過描画オブジェクト配列
	Vector3 cameraPos;
	if (pMainCamera != nullptr)
	{
		cameraPos = pMainCamera->GetTransform()->GetPosition();
	}

	// 透過オブジェクト登録
	for (auto* renderer : m_pRendererComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			renderer->IsTransparent())
		{
			// 透過オブジェクト情報登録
			TransparentRendererInfo info;
			info.pRenderer = renderer;
			Vector3 rendererPos = renderer->GetTransform()->GetPosition();
			info.cameraDistance = (cameraPos - rendererPos).Magnitude();
			transparentRenderer.emplace_back(info);
		}
	}

	// カメラ距離でソート
	std::stable_sort(transparentRenderer.begin(), transparentRenderer.end(),
		[](TransparentRendererInfo a, TransparentRendererInfo b) {
			return a.cameraDistance > b.cameraDistance;
		});

	// 透過オブジェクト描画
	for (auto& rendererInfo : transparentRenderer)
	{
		rendererInfo.pRenderer->Draw();
	}
	EffectManager::Instance().EndDraw();
}

void RenderSystem::DrawAll2D()
{
	// カメラ設定
	Camera* pMainCamera = Camera::GetMain();
	if (pMainCamera != nullptr)
	{
		// ビュー行列設定
		DirectX::XMMATRIX view = pMainCamera->GetViewMatrix();
		ConstantBufferManager::Instance().SetView(view);

		// 2Dカメラプロジェクション行列設定
		DirectX::XMMATRIX projection = pMainCamera->GetOrthographicProjectionMatrix();
		ConstantBufferManager::Instance().SetProjection(projection);
	}

	// フレーム定数バッファを更新
	ConstantBufferManager::Instance().UpdateFrameConstantBuffer();

	// 2D描画順ソート
	std::stable_sort(m_pRenderer2DComponents.begin(), m_pRenderer2DComponents.end(),
		[](Renderer2D* a, Renderer2D* b) {
			return a->GetOrder() > b->GetOrder();
		});

	// 2D描画処理
	for (auto* renderer : m_pRenderer2DComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			!renderer->IsUI())
		{
			renderer->Draw();
		}
	}

	if (pMainCamera != nullptr)
	{
		// UI用カメラのビュー行列を設定
		DirectX::XMMATRIX view = Camera::GetDefaultViewMatrix();
		ConstantBufferManager::Instance().SetView(view);
	}

	// フレーム定数バッファを更新
	ConstantBufferManager::Instance().UpdateFrameConstantBuffer();

	// UI描画処理
	for (auto* renderer : m_pRenderer2DComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			renderer->IsUI())
		{
			renderer->Draw();
		}
	}
}

void RenderSystem::DrawDebugUI()
{
	// ImGuiの描画
	ImGuiManager::Instance().Draw();
}

RenderSystem& RenderSystem::Instance()
{
	static RenderSystem s_instance;
	return s_instance;
}
