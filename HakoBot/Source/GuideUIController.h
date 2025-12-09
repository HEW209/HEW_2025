#pragma once
//GuideUIController.h
#include <DXGameFrame.h>
#include "GameScene.h"
#include "GameFrame/Component.h"

#define MOZI_SIZE (100.0f)

class GuideUIController : public Component 
{


	void Start() override;
	void Update() override;

	

private:
	ObjPtr<SpriteRenderer> m_pRenderer;
	SpriteRenderer* m_renderer;
	float m_value;
	Vector3 m_defaultPosition;
	bool motu;
};