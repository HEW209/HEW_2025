//RenderSystem.cpp
#include <System/RenderSystem.h>
#include <GameFrame/GameObject.h>
#include <DirectX/Direct3D.h>
#include <DirectX/MatrixUtil.h>
#include <DirectX/ConstantBuffer.h>
#include <Component/DirectionalLight.h>
#include <algorithm>

RenderSystem::RenderSystem() :
	m_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void RenderSystem::DrawAll()
{
	Direct3D::Instance().BeginDraw(m_clearColor);

	Camera* pMainCamera = Camera::GetMain();

	//ビュー行列設定
	DirectX::XMFLOAT4X4 view = MatrixUtil::CreateViewMatrix(pMainCamera);
	ConstantBuffer::Instance().SetView(view);

	//プロジェクション行列設定
	DirectX::XMFLOAT4X4 projection = MatrixUtil::CreateProjectionMatrix(pMainCamera);
	ConstantBuffer::Instance().SetProjection(projection);

	// ライト設定
	DirectionalLight* pDirLight = DirectionalLight::GetMain();
	if (pDirLight != nullptr)
	{
		ConstantBuffer::Light light = pDirLight->GetLightData();
		ConstantBuffer::Instance().SetLight(light);
	}

	//3D描画処理
	for (auto* renderer : m_pRendererComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActive())
		{
			renderer->Draw();
		}
	}

	//2D描画順ソート
	std::stable_sort(m_pRenderer2DComponents.begin(), m_pRenderer2DComponents.end(),
		[](Renderer2D* a, Renderer2D* b) {
			return a->GetOrder() > b->GetOrder();
		});

	//2D描画処理
	for (auto* renderer : m_pRenderer2DComponents)
	{
		if (renderer->IsEnabled() &&
			renderer->IsStarted() &&
			renderer->GetGameObject()->IsActive())
		{
			renderer->Draw();
		}
	}

	Direct3D::Instance().EndDraw();
}

void RenderSystem::Register(Renderer* pRenderer)
{
	//コンポーネントを追加
	m_pRendererComponents.emplace_back(pRenderer);
}

void RenderSystem::Unregister(Renderer* pRenderer)
{
	//削除対象コンポーネントを検索して削除する
	auto it = std::remove(m_pRendererComponents.begin(), m_pRendererComponents.end(), pRenderer);
	m_pRendererComponents.erase(it, m_pRendererComponents.end());
}

void RenderSystem::Register2D(Renderer2D* pRenderer2D)
{
	//コンポーネントを追加
	m_pRenderer2DComponents.emplace_back(pRenderer2D);
}

void RenderSystem::Unregister2D(Renderer2D* pRenderer2D)
{
	//削除対象コンポーネントを検索して削除する
	auto it = std::remove(m_pRenderer2DComponents.begin(), m_pRenderer2DComponents.end(), pRenderer2D);
	m_pRenderer2DComponents.erase(it, m_pRenderer2DComponents.end());
}