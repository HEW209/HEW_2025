// Fade.h
#pragma once
#include <DXGameFrame.h>

class Fade : public Component
{
public:
	Fade();
	~Fade();

	void Awake() override;
	void Update() override;

	void SetFadeSpeed(float fadeSpeed);
private:
	SpriteRenderer* m_renderer;
	float m_fadeSpeed;
	bool m_isFirst;

public:
	static bool IsActive();
	static void SetFadeRatio(float ratio);

	static void StartIrisOut();
	static void StartIrisIn();

private:
	static Fade* s_main;

	enum FadeMode
	{
		IRIS,
	};
	static FadeMode s_fadeMode;		// フェードモード
	static bool s_isFade;			// フェード状態フラグ
	static bool s_isActive;			// 動作中フラグ
	static float s_fadeRatio;		// フェード割合
};
