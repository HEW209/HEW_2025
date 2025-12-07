#include "StageSelectKeyEnter.h"

void StegeSelectKeyEnter::Start()
{
	
}

void StegeSelectKeyEnter::Update()
{
	static float a = 0;
	a += 0.01f;
	GetTransform()->SetPosition(0.0f, a, 0.0f);
	
}

void StegeSelectKeyEnter::SetStageID()
{
}
