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
	bool m_motu;
	SpriteRenderer* m_bRend;
	SpriteRenderer* m_rtRend;
	SpriteRenderer* m_ltRend;
	SpriteRenderer* m_kaitenRend;
	SpriteRenderer* m_rbRend;
	SpriteRenderer* m_lbRend;
};