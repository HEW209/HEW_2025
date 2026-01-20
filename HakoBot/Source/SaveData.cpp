#include "SaveData.h"
#include <iostream>
#include <filesystem>

SaveData::GameData SaveData::s_data;

bool SaveData::Find()
{
	return std::filesystem::exists("Assets/Level/SaveData.bin");
}

void SaveData::Save()
{
	FILE* fp;
	fopen_s(&fp,"Assets/Level/SaveData.bin","wb");

	if (fp)
	{
		fwrite(&s_data, sizeof(s_data), 1, fp);
		fclose(fp);
	}
}

void SaveData::Load()
{
	FILE* fp;
	fopen_s(&fp, "Assets/Level/SaveData.bin", "rb");

	if (fp)
	{
		fread(&s_data, sizeof(s_data), 1, fp);
		fclose(fp);
	}
}

void SaveData::Reset()
{
	s_data.clearStage = 0;
}

void SaveData::SetClearLevel(int clearStage)
{
	s_data.clearStage = clearStage;
}

int SaveData::GetClearLevel()
{
	return s_data.clearStage;
}
