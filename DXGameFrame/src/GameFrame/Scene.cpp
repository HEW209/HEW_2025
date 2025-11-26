// Scene.cpp
#include <GameFrame/Scene.h>
#include <GameFrame/RenderSystem.h>

Scene::~Scene()
{
	// ゲームオブジェクトをクリアしてから削除
	m_gameObjects.clear();
}

GameObject* Scene::CreateGameObject()
{
	// ゲームオブジェクトを生成・追加する
	auto gameobject = std::make_unique<GameObject>(this);
	GameObject* ptr = gameobject.get();
	m_gameObjects.emplace_back(std::move(gameobject));

	return ptr;
}

void Scene::Update()
{
	UpdateAllGameObject();

	// オブジェクトの削除を適用
	ApplyDestroyGameObject();

	RenderSystem::Instance().DrawAll();
}

void Scene::UpdateAllGameObject()
{
	std::vector<GameObject*> objBuffer;		// 操作するオブジェクトへのポインタリスト

	// 先にメモリを確保しておく
	objBuffer.reserve(m_gameObjects.size());

	// 処理中に新しいオブジェクトが作成される可能性があるため、
	// 操作するオブジェクトへの参照を作成してから処理を行う
	for (auto& obj : m_gameObjects)
	{
		// 操作するゲームオブジェクトへのポインタを作成
		objBuffer.emplace_back(obj.get());
	}

	// オブジェクトの開始処理
	for (auto* obj : objBuffer)
	{
		if (!obj->IsActiveHierarchy())
			continue;

		obj->StartAllComponent();
	}

	// オブジェクトの更新処理
	for (auto* obj : objBuffer)
	{
		if (!obj->IsActiveHierarchy())
			continue;

		obj->UpdateAllComponent();
	}

	// オブジェクトの遅延更新処理
	for (auto* obj : objBuffer)
	{
		if (!obj->IsActiveHierarchy())
			continue;

		obj->LateUpdateAllComponent();
	}
}

void Scene::ApplyDestroyGameObject()
{
	// 全てのゲームオブジェクトの削除フラグをチェックする
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); )
	{
		if ((*it)->IsDestroyed())
		{
			(*it)->OnDestroy();

			// 削除して次に進む
			it = m_gameObjects.erase(it);
		}
		else
		{
			// 次に進む
			++it;
		}
	}
}