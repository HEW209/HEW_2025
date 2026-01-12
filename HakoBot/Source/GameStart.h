#pragma once
#include <DXGameFrame.h>



class StartUI : public Component
{
public:
	StartUI();
	~StartUI() = default;

	void Start() override;
	void Update() override;


private:
	bool m_HalfProcessed;
	bool m_IsVisible;
	float m_Timer;		

	SpriteRenderer* m_StartSprite;

	// フェード用
	float m_Alpha;

	// サイズ
	Vector2 m_StartSize;  // フェード開始時サイズ
	Vector2 m_EndSize;    // 最終表示サイズ
	Vector2 m_CurrentSize;

	// 補間関数（マグニチュードみたいなのなかったっけ？）
	float Lerp(float a, float b, float t);
};