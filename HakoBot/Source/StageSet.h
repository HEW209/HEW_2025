#pragma once

#include <DXGameFrame.h>

class StageSet : public Component
{
public:
	StageSet();
	virtual ~StageSet() = default;

	void Awake() override;
	void Update() override;

private:
	ObjPtr<MeshRenderer> m_pConveyors;
	ObjPtr<MeshRenderer> m_pMiddleWall;
	ObjPtr<MeshRenderer> m_pLeftWall;
	ObjPtr<MeshRenderer> m_pRightWall;
	ObjPtr<MeshRenderer> m_pLeftPillar;
	ObjPtr<MeshRenderer> m_pRightPillar;
	ObjPtr<MeshRenderer> m_pFans[3];
	MeshGroup::AnimeNo m_fanAnim;

	void SetTransparent(MeshRenderer* renderer, float transparency);

	float m_time;
};