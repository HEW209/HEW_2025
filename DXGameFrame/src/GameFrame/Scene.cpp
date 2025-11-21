//Scene.cpp
#include <GameFrame/Scene.h>
#include <System/RenderSystem.h>
#include <System/ColliderSystem.h>

Scene::~Scene()
{
	m_gameObjects.clear();
}

GameObject* Scene::CreateGameObject()
{
	//ゲームオブジェクトを生成・追加する
	auto gameobject = std::make_unique<GameObject>(this);
	GameObject* ptr = gameobject.get();
	m_gameObjects.emplace_back(std::move(gameobject));

	return ptr;
}

void Scene::Update()
{
	//ゲームオブジェクトの更新処理
	std::vector<GameObject*> objBuffer;			//操作するオブジェクトへのポインタリスト

	//先にメモリを確保しておく
	objBuffer.reserve(m_gameObjects.size());

	//処理中に新しいオブジェクトが作成される可能性があるため、
	//操作するオブジェクトへの参照を作成してから処理を行う
	for (auto& obj : m_gameObjects)
	{
		//操作するゲームオブジェクトへのポインタを作成
		objBuffer.emplace_back(obj.get());
	}

	//オブジェクトの開始処理
	for (auto* obj : objBuffer)
	{
		obj->Start();
	}

	//オブジェクトの更新処理
	for (auto* obj : objBuffer)
	{
		obj->Update();
	}

	//オブジェクトの遅延更新処理
	for (auto* obj : objBuffer)
	{
		obj->LateUpdate();
	}

	//オブジェクトの削除を適用
	ApplyDestroyGameObject();

	// 当たり判定チェック
	ColliderSystem::Instance().Check();

	//描画処理
	RenderSystem::Instance().DrawAll();
}

void Scene::ApplyDestroyGameObject()
{
	//全てのゲームオブジェクトの削除フラグをチェックする
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); )
	{
		if ((*it)->IsDestroyed())
		{
			(*it)->OnDestroy();

			//削除して次に進む
			it = m_gameObjects.erase(it);
		}
		else
		{
			//次に進む
			++it;
		}
	}
}