#pragma once
#include <DXGameFrame.h>
#include"StageSelectKeyEnter.h"

class StageSelectObject : public Component
{
public:
	StageSelectObject() = default;
	~StageSelectObject() = default;

	void Start() override;
	void Update()override;

	void LoadGame(int StageID);
	
private:


	ObjPtr<StegeSelectKeyEnter> StageKeyEnter;
	int m_StageID;

};