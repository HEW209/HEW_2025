#pragma once

#include <DXGameFrame.h>

class ITutorial : public Component
{
public:
	virtual ~ITutorial() = 0;
	virtual bool IsFinished() const = 0;
};


class Tutorial1 final : public ITutorial
{
public:
	Tutorial1();
	virtual ~Tutorial1() = default;

	void Awake() override;
	void LateUpdate() override;

	bool IsFinished() const override { return m_finished; }

private:
	bool m_finished;
	std::array<bool, 3> m_sePlayed;
	int m_step;

	ObjPtr<Camera> m_pCamera;
	ObjPtr<Camera> m_pDefaultCamera;
	ObjPtr<SpriteRenderer> m_pLine;
	ObjPtr<SpriteRenderer> m_pText1;
	ObjPtr<SpriteRenderer> m_pText2;

	Vector3 m_prevCameraPos;
	Quaternion m_prevCameraRot;
	Vector3 m_nextCameraPos;
	Quaternion m_nextCameraRot;
	float m_cameraMoveDuration;
	float m_cameraMoveTimer;
};


class Tutorial2 final : public ITutorial
{
public:
	Tutorial2();
	virtual ~Tutorial2() = default;

	void Awake() override;
	void LateUpdate() override;

	bool IsFinished() const override { return m_finished; }

private:
	bool m_finished;
	std::array<bool, 3> m_sePlayed;
	int m_step;

	ObjPtr<Camera> m_pCamera;
	ObjPtr<Camera> m_pDefaultCamera;
	ObjPtr<SpriteRenderer> m_pText;
	ObjPtr<MeshRenderer> m_rotateArrow[2];

	Vector3 m_prevCameraPos;
	Quaternion m_prevCameraRot;
	Vector3 m_nextCameraPos;
	Quaternion m_nextCameraRot;
	float m_cameraMoveDuration;
	float m_cameraMoveTimer;

	float m_arrowMoveTimer;
	float m_arrowAlpha;
};

class Tutorial3 final : public ITutorial
{
public:
	Tutorial3();
	virtual ~Tutorial3() = default;

	void Awake() override;
	void LateUpdate() override;

	bool IsFinished() const override { return m_finished; }

private:
	bool m_finished;
	std::array<bool, 3> m_sePlayed;
	int m_step;

	ObjPtr<Camera> m_pCamera;
	ObjPtr<Camera> m_pDefaultCamera;
	ObjPtr<SpriteRenderer> m_pLine;
	ObjPtr<SpriteRenderer> m_pText1;
	ObjPtr<SpriteRenderer> m_pText2;

	Vector3 m_prevCameraPos;
	Quaternion m_prevCameraRot;
	Vector3 m_nextCameraPos;
	Quaternion m_nextCameraRot;
	float m_cameraMoveDuration;
	float m_cameraMoveTimer;
	ObjPtr<MeshRenderer> m_arrow[2];

	float m_arrowMoveTimer;
	float m_arrowAlpha;
	float m_lightAngleY;
	float m_lightRotateDelay;
};