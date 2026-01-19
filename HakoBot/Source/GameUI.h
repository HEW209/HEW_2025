#pragma once

#include <DXGameFrame.h>

class GameUI : public Component
{
public:
	GameUI() = default;
	virtual ~GameUI() = default;
	void Awake() override;
	void Update() override;
private:
	ObjPtr<GameObject> m_pRoot;
	ObjPtr<SpriteRenderer> m_pTimeUI;
	ObjPtr<SpriteRenderer> m_pClearUI;
	ObjPtr<SpriteRenderer> m_pPauseUI;
	ObjPtr<SpriteRenderer> m_pGuideUI;
};