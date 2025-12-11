//Title.cpp
#include "TitleUI.h"
#include "Easing.h"

void TitleUI::Start()
{
    //‚Í‚¶‚ß‚é•¶Žš
	m_pRend = GetGameObject()->AddComponent<SpriteRenderer>();
    m_pRend->SetUI(true);
    m_pRend->LoadTexture("Assets/Textures/hajimeru.png");
    m_pRend->SetSize(400.0f);
    m_pRend->SetOffsetPos(0.0f, -2.0f);
    
}

void TitleUI::Update()
{
    
}
