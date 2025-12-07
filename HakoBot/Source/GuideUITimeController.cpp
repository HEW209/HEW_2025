//GuideUITimeController.cpp
#include "GuideUITimeController.h"
#include "GuideUIController.h"

void GuideUITimeController::Start()
{
	m_pRenderer1 = GetGameObject()->GetComponent<SpriteRenderer>();
	auto renderer = GetGameObject()->AddComponent<SpriteRenderer>();
	renderer->SetUI(true);
	renderer->LoadTexture("Assets/Textures/rizaruto.png");
	renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	renderer->SetSize(0.0f,0.0f);

}

void GuideUITimeController::Update()
{
	if (Input::GetKeyHold(KeyCode::R))
	{
	//	m_pRenderer1->SetSize(MOZI_SIZE + 400.0f, MOZI_SIZE + 200.0f);
	}
}



