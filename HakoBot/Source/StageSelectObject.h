#pragma once
#include <DXGameFrame.h>

class StegeSelectKeyEnter;

class StageSelectObject : public Component
{
public:
	StageSelectObject() = default;
	~StageSelectObject() = default;

	void Start() override;
	void Update()override;

	void LoadGame(int StageID);
	Vector2 SetPosID(int StageID);

private:


	StegeSelectKeyEnter* StageKeyEnter;
	int m_StageID;

};