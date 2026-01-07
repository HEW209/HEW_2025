// SaveData.h
#pragma once

class SaveData
{
public:
	static void Save();
	static void Load();
	static void Reset();

	static void SetClearLevel(int clearStage);
	static int GetClearLevel();
private:
	SaveData() = delete;

	struct GameData
	{
		int clearStage;
	};
	static GameData s_data;
};
