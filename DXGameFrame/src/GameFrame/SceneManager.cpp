// SceneManager.cpp
#include <GameFrame/SceneManager.h>
#include <Utility/Input.h>
#include <GameFrame/Time.h>
#include <../imgui/ImguiManager.h>

std::unique_ptr<Scene> SceneManager::s_activeScene = nullptr;
std::unique_ptr<Scene> SceneManager::s_nextScene = nullptr;

void SceneManager::Init(std::unique_ptr<Scene> startScene)
{
	// 初期シーン登録
	s_activeScene = std::move(startScene);
	s_activeScene->Init();

	Input::Init();
	Time::Init();
}

void SceneManager::Uninit()
{
	s_activeScene->Uninit();
	s_activeScene = nullptr;
	s_nextScene = nullptr;
}

void SceneManager::Execute(float delta)
{
	// 入力更新
	Input::Update();

	// 時間更新
	Time::Update(delta);

	// ImGuiフレーム開始処理
	ImGuiManager::Instance().BeginFrame();

	if (s_activeScene == nullptr)
		return;

	// シーン更新
	s_activeScene->Update();
	ApplyChangeScene();
}

void SceneManager::ChangeScene(std::unique_ptr<Scene> nextScene)
{
	// 最初にセットされたものが有効
	if (s_nextScene == nullptr)
		s_nextScene = std::move(nextScene);
}

Scene* SceneManager::GetActiveScene()
{
	return s_activeScene.get();
}

void SceneManager::ApplyChangeScene()
{
	if (s_nextScene == nullptr)
		return;

	if (s_activeScene != nullptr)
	{
		s_activeScene->Uninit();
	}

	// シーンの変更
	s_activeScene = std::move(s_nextScene);
	s_nextScene = nullptr;
	s_activeScene->Init();

	// シーン内経過時間をリセット
	Time::ResetSceneTime();
}
