//ResultController.h
//2026 01 15 ²“¡‘“–ç
//ƒŠƒUƒ‹ƒg‰æ–Ê‚ğì‚è‚Ü‚·
//‚ ‚¯‚Ü‚µ‚Ä‚¨‚ß‚Å‚Æ‚¤‚²‚´‚¢‚Ü‚·
#pragma once

#include <DXGameFrame.h>
#include "GameFrame/Component.h"

class ResultController : public Component
{
	void Start() override;
	void Update() override;
private:
	ObjPtr<SpriteRenderer> m_kuroDown;
	ObjPtr<SpriteRenderer> m_kuroUp;
	ObjPtr<SpriteRenderer> m_result;
	ObjPtr<SpriteRenderer> m_tugi;
	ObjPtr<SpriteRenderer> m_stage;
	ObjPtr<SpriteRenderer> m_re;

	float m_resultTime;
};