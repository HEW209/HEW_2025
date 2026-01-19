//ResultController.h
//2026 01 15 ²“¡‘“–ç
//ƒŠƒUƒ‹ƒg‰æ–Ê‚ğì‚è‚Ü‚·
//‚ ‚¯‚Ü‚µ‚Ä‚¨‚ß‚Å‚Æ‚¤‚²‚´‚¢‚Ü‚·
#pragma once

#include <DXGameFrame.h>
#include "GameFrame/Component.h"

class ResultController : public Component
{
public:
	ResultController();
	void Start() override;
	void Update() override;
private:
	ObjPtr<SpriteRenderer> m_kuroDown;
	ObjPtr<SpriteRenderer> m_kuroUp;
	ObjPtr<SpriteRenderer> m_result;
	ObjPtr<SpriteRenderer> m_selectText[3];
	ObjPtr<SpriteRenderer> m_illust;

	float m_resultTime;

	enum class ResultState
	{
		MOVE,SELECT,END
	};

	ResultState m_state;
	void MoveUpdate();
	void SelectUpdate();
	void EndUpdate();

	enum Select
	{
		NEXT,STAGE_SELECT,RESTART,COUNT
	};

	int m_currentSelect;
};