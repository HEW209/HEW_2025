#include "SoundManager.h"

std::unordered_map<std::string, SoundManager::BGM> SoundManager::bgmMap;
std::unordered_map<std::string, SoundManager::SE> SoundManager::seMap;
SoundPlayer SoundManager::bgmPlayer;
SoundPlayer SoundManager::sePlayer;

// ƒTƒEƒ“ƒh“o˜^
void SoundManager::Load()
{
	//--- BGM“o˜^
	bgmMap["Stage1"];
	if(!SoundData::LoadWave("Assets/Sound/BGM_Stage.wav", bgmMap["Stage1"].data))
		MessageBoxA(NULL, "BGM_Stage.wav ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½", "Sound Error", MB_OK | MB_ICONWARNING);

	//--- SE“o˜^
	seMap["PutBox"];
	if(!SoundData::LoadWave("Assets/Sound/SE_putBox.wav", seMap["PutBox"].data))
		MessageBoxA(NULL, "SE_putBox.wav ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½", "Sound Error", MB_OK | MB_ICONWARNING);
}

// BGMÄ¶
void SoundManager::PlayBGM(const std::string& name, float volume, bool loop)
{
	auto it = bgmMap.find(name);
	if (it == bgmMap.end()) return;

	bgmPlayer.PlayWave(&it->second.data, loop, volume);
}

// BGM’â~
void SoundManager::StopBGM()
{
	bgmPlayer.StopWave();
}

// SEÄ¶
void SoundManager::PlaySE(const std::string& name, float volume, bool loop)
{
	auto it = seMap.find(name);
	if (it == seMap.end()) return;

	sePlayer.PlayWave(&it->second.data, loop, volume);
}

// SE’â~
void SoundManager::StopSE()
{
	sePlayer.StopWave();
}

// ‘S‚Ä‚ÌƒTƒEƒ“ƒh’â~
void SoundManager::StopAll()
{
	bgmPlayer.StopWave();
	sePlayer.StopWave();
}
