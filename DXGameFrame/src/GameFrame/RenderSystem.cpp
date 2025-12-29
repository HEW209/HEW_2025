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

void CalcLightMatrices(
	DirectX::XMVECTOR lightDir,          // ライト方向
	DirectX::XMMATRIX cameraViewProj,    // メインカメラの View * Proj
	DirectX::XMMATRIX& outLightView,     // 出力: ライトView
	DirectX::XMMATRIX& outLightProj      // 出力: ライトProj (Ortho)
);

RenderSystem::RenderSystem() :
	m_clearColor(0.4f, 0.4f, 1.0f, 1.0f)
{
}

void RenderSystem::DrawAll()
{
	// 画面クリア
	float clearColor[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a };
	Direct3D::Instance().ClearView(clearColor);

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

	// 2D描画順ソート
	std::stable_sort(m_pRenderer2DComponents.begin(), m_pRenderer2DComponents.end(),
		[](Renderer2D* a, Renderer2D* b) {
			return a->GetOrder() > b->GetOrder();
		});

	DrawALL2DBackGround();
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

void RenderSystem::RegisterShadow(Renderer* pRenderer)
{
	// コンポーネントを追加
	m_pShadowRendererComponents.emplace_back(pRenderer);
}

void RenderSystem::UnregisterShadow(Renderer* pRenderer)
{
	// 削除対象コンポーネントを検索して削除する
	auto it = std::remove(m_pShadowRendererComponents.begin(), m_pShadowRendererComponents.end(), pRenderer);
	m_pShadowRendererComponents.erase(it, m_pShadowRendererComponents.end());
}

void RenderSystem::SetClearColor(Color color)
{
	m_clearColor = color;
}

void RenderSystem::DrawALL2DBackGround()
{
	Direct3D::Instance().BeginDraw();
	PipelineStateManager::Instance().Refresh();

	// カメラ設定
	Camera* pMainCamera = Camera::GetMain();
	if (pMainCamera != nullptr)
	{
		// UI用カメラのビュー行列を設定
		DirectX::XMMATRIX view = Camera::GetDefaultViewMatrix();
		ConstantBufferManager::Instance().SetView(view);

		// 2Dカメラプロジェクション行列設定
		DirectX::XMMATRIX projection = pMainCamera->GetOrthographicProjectionMatrix();
		ConstantBufferManager::Instance().SetProjection(projection);
	}

	// フレーム定数バッファを更新
	ConstantBufferManager::Instance().UpdateFrameConstantBuffer();

	// UI描画処理
	for (auto* renderer : m_pRenderer2DComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			renderer->IsBackGround())
		{
			renderer->Draw();
		}
	}
}

void RenderSystem::DrawAll3D()
{
	// --- 影描画処理 ---

	Direct3D::Instance().BeginDrawShadow();

	DirectionalLight* pDirLight = DirectionalLight::GetMain();
	if (pDirLight == nullptr) return;

	// カメラ設定
	Camera* pMainCamera = Camera::GetMain();
	if (pMainCamera == nullptr) return;

	// カメラ行列取得
	DirectX::XMMATRIX cameraView = pMainCamera->GetViewMatrix();
	DirectX::XMMATRIX cameraProj = pMainCamera->GetProjectionMatrix();

	// ライト行列計算
	DirectX::XMMATRIX lightView;
	DirectX::XMMATRIX lightProj;

	Vector3 lightDirVec3 = pDirLight->GetTransform()->GetQuaternion() * Vector3::forward;

	DirectX::XMFLOAT3 lightDir = { lightDirVec3.x, lightDirVec3.y, lightDirVec3.z };

	DirectX::XMMATRIX shadowCameraProj = pMainCamera->GetShadowProjectionMatrix(50.0f);

	CalcLightMatrices(
		DirectX::XMLoadFloat3(&lightDir),
		cameraView * shadowCameraProj,
		lightView,
		lightProj
	);

	ConstantBufferManager::Instance().SetLightViewProj(lightView * lightProj);

	ConstantBufferManager::Instance().SetView(lightView);
	ConstantBufferManager::Instance().SetProjection(lightProj);

	// フレーム定数バッファを更新
	ConstantBufferManager::Instance().UpdateFrameConstantBuffer();

	// パイプラインステートをリセット
	PipelineStateManager::Instance().Refresh();

	// 影描画処理
	for (auto* renderer : m_pShadowRendererComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			!renderer->IsTransparent())
		{
			renderer->DrawShadow();
		}
	}


	// --- オブジェクト描画処理 ---

	Direct3D::Instance().BeginDraw();
	Direct3D::Instance().SetShadowMap();

	ConstantBufferManager::Instance().SetView(cameraView);
	ConstantBufferManager::Instance().SetProjection(cameraProj);

	// フレーム定数バッファを更新
	ConstantBufferManager::Instance().UpdateFrameConstantBuffer();
	EffectManager::Instance().BeginDraw();

	// パイプラインステートをリセット
	PipelineStateManager::Instance().Refresh();

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
	Direct3D::Instance().BeginDraw();
	PipelineStateManager::Instance().Refresh();

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

	// 2D描画処理
	for (auto* renderer : m_pRenderer2DComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActiveHierarchy() &&
			!renderer->IsUI()&&
			!renderer->IsBackGround())
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
			renderer->IsUI() &&
			!renderer->IsBackGround())
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

void CalcLightMatrices(
	DirectX::XMVECTOR lightDir,          // ライト方向
	DirectX::XMMATRIX cameraViewProj,    // メインカメラの View * Proj
	DirectX::XMMATRIX& outLightView,     // 出力: ライトView
	DirectX::XMMATRIX& outLightProj      // 出力: ライトProj (Ortho)
)
{
	// 視錐台の8頂点をワールド空間へ変換
	DirectX::XMVECTOR frustumCorners[8] = {
		{-1, 1, 0, 1}, { 1, 1, 0, 1}, {-1,-1, 0, 1}, { 1,-1, 0, 1}, // Near
		{-1, 1, 1, 1}, { 1, 1, 1, 1}, {-1,-1, 1, 1}, { 1,-1, 1, 1}  // Far
	};

	DirectX::XMMATRIX invViewProj = XMMatrixInverse(nullptr, cameraViewProj);

	// 重心計算用
	DirectX::XMVECTOR center = DirectX::XMVectorSet(0, 0, 0, 0);

	for (int i = 0; i < 8; ++i)
	{
		frustumCorners[i] = XMVector3TransformCoord(frustumCorners[i], invViewProj);
		center = DirectX::XMVectorAdd(center, frustumCorners[i]);
	}
	center = DirectX::XMVectorScale(center, 1.0f / 8.0f);

	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(lightDir, up);
	float dotVal = DirectX::XMVectorGetX(dot);
	if (std::abs(dotVal) > 0.99f)
	{
		up = DirectX::XMVectorSet(0, 0, 1, 0);
	}

	float shadowDistance = 200.0f;
	DirectX::XMVECTOR lightOffset = DirectX::XMVectorScale(lightDir, shadowDistance);
	DirectX::XMVECTOR lightPos = DirectX::XMVectorSubtract(center, lightOffset);

	outLightView = DirectX::XMMatrixLookAtLH(lightPos, center, up);

	// ライト空間でのAABBを計算し、正射影サイズを決定
	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;
	float minZ = FLT_MAX, maxZ = -FLT_MAX;

	for (int i = 0; i < 8; ++i)
	{
		// ワールド頂点をライトビューで変換
		DirectX::XMVECTOR vLightSpace = XMVector3TransformCoord(frustumCorners[i], outLightView);

		float x = DirectX::XMVectorGetX(vLightSpace);
		float y = DirectX::XMVectorGetY(vLightSpace);
		float z = DirectX::XMVectorGetZ(vLightSpace);

		minX = min(minX, x); maxX = max(maxX, x);
		minY = min(minY, y); maxY = max(maxY, y);
		minZ = min(minZ, z); maxZ = max(maxZ, z);
	}

	float shadowCasterMargin = 200.0f;
	float nearPlane = minZ - shadowCasterMargin;
	float farPlane = maxZ + 20.0f;

	outLightProj = DirectX::XMMatrixOrthographicOffCenterLH(
		minX, maxX, minY, maxY, nearPlane, farPlane
	);
}