#include "SampleScene.h"
#include "SamplePlayer.h"
#include "SampleCamera.h"

void SampleScene::Init()
{
	GameObject* playerObj = nullptr;
	SampleCamera* sampleCamera = nullptr;

	//ƒJƒƒ‰
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(20.0f, 45.0f, 0.0f);
		sampleCamera = obj->AddComponent<SampleCamera>();
		camera->SetMain();
	}

	//ƒvƒŒƒCƒ„[
	{
		/*auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		renderer->LoadModel("Assets/Model/karubi/‚Ü‚¯‚ñŽ®ÔŒ©‚©‚é‚Ñ•Pˆß‘•ver1.0.pmx");
		obj->AddComponent<SamplePlayer>();
		auto transform = obj->GetTransform();
		transform->m_scale = { 0.1f, 0.1f, 0.1f };*/

		playerObj = CreateGameObject();
		auto renderer = playerObj->AddComponent<MeshRenderer>();
		renderer->LoadModel("Assets/Model/karubi/‚Ü‚¯‚ñŽ®ÔŒ©‚©‚é‚Ñ•Pˆß‘•ver1.0.pmx");
		playerObj->AddComponent<SamplePlayer>();
		auto t = playerObj->GetTransform();
		t->m_scale = { 0.1f, 0.1f, 0.1f };
	}

	if (sampleCamera && playerObj)
	{
		sampleCamera->SetTarget(playerObj->GetTransform());
	}


	//°
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position.y = -0.5f;
		transform->m_scale = { 1.0f, 1.0f, 1.0f };
	}

}