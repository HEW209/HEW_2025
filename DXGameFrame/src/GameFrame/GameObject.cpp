//GameObject.cpp
#include <GameFrame/GameObject.h>
#include <GameFrame/Transform.h>

GameObject::GameObject(Scene* pScene) :
	m_pScene(pScene),
	m_pTransform(nullptr),
	m_isActive(true),
	m_destroyed(false)
{
	//Transformコンポーネントの追加
	auto transform = std::make_unique<Transform>(this);
	m_pTransform = transform.get();
	m_components.emplace_back(std::move(transform));
	m_pTransform->SetGameObject(this);
}

GameObject::~GameObject()
{
	m_components.clear();
}

void GameObject::Start()
{
	std::vector<Component*> comBuffer;			//操作するコンポーネントへのポインタリスト

	//先にメモリを確保しておく
	comBuffer.reserve(m_components.size());

	//コンポーネントの処理中に新しいコンポーネントが作成される可能性があるため、
	//操作するコンポーネントへの参照を作成してから処理を行う
	for (auto& com : m_components)
	{
		if (com->IsStarted())
			continue;

		comBuffer.emplace_back(com.get());
	}

	//全ての追加したコンポーネントの開始処理を呼びだす
	for (auto* com : comBuffer)
	{
		if (!com->IsEnabled())
			continue;

		com->Start();
		com->SetStarted();
	}
}

void GameObject::Update()
{
	std::vector<Component*> comBuffer;		//操作するコンポーネントへのポインタリスト

	//先にメモリを確保しておく
	comBuffer.reserve(m_components.size());

	//操作するコンポーネントへの参照を作成
	for (auto& com : m_components)
	{
		if (!com->IsStarted())
			continue;

		comBuffer.emplace_back(com.get());
	}

	//全てのコンポーネントのUpdateメソッドを呼びだす
	for (auto* com : comBuffer)
	{
		if (!com->IsEnabled())
			continue;

		com->Update();
	}
}

void GameObject::LateUpdate()
{
	std::vector<Component*> comBuffer;		//操作するコンポーネントへのポインタリスト

	//先にメモリを確保しておく
	comBuffer.reserve(m_components.size());

	//操作するコンポーネントへの参照を作成
	for (auto& com : m_components)
	{
		if (!com->IsStarted())
			continue;

		comBuffer.emplace_back(com.get());
	}

	//全てのコンポーネントのLateUpdateメソッドを呼びだす
	for (auto* com : comBuffer)
	{
		if (!com->IsEnabled())
			continue;

		com->LateUpdate();
	}
}

void GameObject::OnDestroy()
{
	for (auto& com : m_components)
	{
		com->OnDestroy();
	}
}

void GameObject::Destroy()
{
	// 削除フラグを立てる
	m_destroyed = true;

	// 子に削除を伝播
	std::vector<Transform*> children = m_pTransform->GetChildren();
	for (auto child : children)
	{
		child->GetGameObject()->Destroy();
	}
}

void GameObject::ApplyRemoveComponent()
{
	std::vector<Component*> comBuffer;		//操作するコンポーネントへのポインタリスト

	//先にメモリを確保しておく
	comBuffer.reserve(m_components.size());

	//削除対象コンポーネントへの参照を作成
	for (auto& com : m_components)
	{
		if (!com->IsDestroyed())
			continue;

		comBuffer.emplace_back(com.get());
	}

	//削除時処理の実行と削除を行う
	for (auto* com : comBuffer)
	{
		com->OnDestroy();

		//削除対象の実体を検索
		auto it = std::find_if(m_components.begin(), m_components.end(),
			[com](const std::unique_ptr<Component>& ptr) { return ptr.get() == com; });

		if (it != m_components.end())
			m_components.erase(it);
	}
}