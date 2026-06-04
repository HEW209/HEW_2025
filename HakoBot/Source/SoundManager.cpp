#include "SoundManager.h"

std::unordered_map<std::string, SoundManager::BGM> SoundManager::bgmMap;
std::unordered_map<std::string, SoundManager::SE> SoundManager::seMap;
SoundPlayer SoundManager::bgmPlayer;
std::vector<std::unique_ptr<SoundPlayer>> SoundManager::sePlayers;

// サウンド登録
void SoundManager::Load()
{
	//----------------------------- タイトル -------------------------------------
	//BGM
	LoadBGMFile("Title", "Assets/Sound/BGM_Title.wav");
	//決定
	LoadSEFile("Title_Decision", "Assets/Sound/SE_TitleDecision.wav");
	//選択
	LoadSEFile("Title_Select", "Assets/Sound/SE_Select.wav");


	//-------------------------- ステージセレクト ---------------------------------
	//BGM
	LoadBGMFile("StageSelect", "Assets/Sound/BGM_StageSelect(1).wav");
	//選択
	LoadSEFile("StageSelect_Select", "Assets/Sound/SE_Slide.wav");
	//選択(長押し)
	// LoadSEFile("StageSelect_Select_Long", "Assets/Sound/SE_Slide.wav");
	//決定
	LoadSEFile("StageSelect_Decision", "Assets/Sound/SE_StageSelectDecision.wav");
	//タイトルに戻る
	LoadSEFile("BackToTitle", "Assets/Sound/SE_BackToTitle.wav");


	//-------------------------- チュートリアル ---------------------------------
	//ゲームBGM
	LoadBGMFile("Game", "Assets/Sound/BGM_Game(1).wav");
	//テキストボックス出現
	LoadSEFile("TextBoxIn", "Assets/Sound/SE_TextBoxIn.wav");
	//テキストボックス退場
	LoadSEFile("TextBoxOut", "Assets/Sound/SE_TextBoxOut.wav");
	//アナウンス
	LoadSEFile("Announcement", "Assets/Sound/SE_Announce2.wav");
	//次に進む
	LoadSEFile("Tutorial_Decision", "Assets/Sound/SE_TutorialDecision.wav");


	//-------------------------- プレイヤー ---------------------------------
	//移動SE
	// LoadSEFile("PlayerMove", "Assets/Sound/SE_Move.wav");
	//ブロック持つ
	LoadSEFile("PutBox", "Assets/Sound/SE_putBox.wav");
	//ブロック置く（グリッド外）
	LoadSEFile("OutGrid", "Assets/Sound/SE_OutGrid.wav");
	//ブロック置く（グリッド内.正解）
	LoadSEFile("Correct", "Assets/Sound/SE_Correct.wav");
	//ブロック置く（グリッド内.不正解）
	LoadSEFile("Wrong", "Assets/Sound/SE_Wrong.wav");
	//ブロック回転
	LoadSEFile("BlockRotate", "Assets/Sound/SE_Slide.wav");
	//高さ変更
	//LoadSEFile("PlayerStretch", "Assets/Sound/SE_Slide.wav");


	//-------------------------- メニュー ---------------------------------
	//開く
	LoadSEFile("Menu_Open", "Assets/Sound/SE_MenuOpen.wav");
	//閉じる
	LoadSEFile("Menu_Close", "Assets/Sound/SE_MenuClose.wav");
	//選択
	LoadSEFile("Menu_Select", "Assets/Sound/SE_Select.wav");
	//決定
	LoadSEFile("Menu_Decision", "Assets/Sound/SE_Decision.wav");


	//-------------------------- システム ---------------------------------
	//向きロック開始
	//LoadSEFile("CameraLock", "Assets/Sound/SE_Decision.wav");
	//向きロック終了
	//LoadSEFile("CameraLock_end", "Assets/Sound/SE_Decision.wav");
	//ブロック透明化開始
	//LoadSEFile("Transparent", "Assets/Sound/SE_Decision.wav");
	//ブロック透明化終了
	//LoadSEFile("Transparent_end", "Assets/Sound/SE_Decision.wav");


	//-------------------------- リザルト ---------------------------------
	//BGM開始
	LoadBGMFile("ResultStart", "Assets/Sound/BGM_ResultStart.wav");
	//BGMループ
	LoadBGMFile("ResultLoop", "Assets/Sound/BGM_ResultLoop.wav");
	//選択
	LoadSEFile("Result_Select", "Assets/Sound/SE_Select.wav");
	//決定
	LoadSEFile("Result_Decision", "Assets/Sound/SE_Decision.wav");


	//-------------------------- 演出 ---------------------------------
	//ステージ開始時UI
	LoadSEFile("StageStart", "Assets/Sound/SE_TitleLanding.wav");
	//完成UI出現
	//LoadSEFile("Completed_in", "Assets/Sound/SE_TitleLanding.wav");
	//完成UI退場
	//LoadSEFile("Completed_out", "Assets/Sound/SE_TitleLanding.wav");
	//クリア（回転）
	LoadSEFile("Clear", "Assets/Sound/SE_Clear.wav");
	//クリア（クラッカー）
	LoadSEFile("Cracker", "Assets/Sound/SE_Cracker.wav");
	//フェードイン
	LoadSEFile("FadeIn", "Assets/Sound/SE_FadeIn.wav");
	//フェードアウト
	LoadSEFile("FadeOut", "Assets/Sound/SE_FadeOut.wav");
}

// BGM再生
void SoundManager::PlayBGM(const std::string& name, float volume, bool loop)
{
	auto it = bgmMap.find(name);
	if (it == bgmMap.end())
		return;

	bgmPlayer.PlayWave(&it->second.data, loop, volume);
}

// BGM停止
void SoundManager::StopBGM()
{
	bgmPlayer.StopWave();
}

// SE再生
void SoundManager::PlaySE(const std::string& name, float volume, bool loop)
{
	auto it = seMap.find(name);
	if (it == seMap.end()) return;

	bool isPlayed = false;

	for (auto&& player : sePlayers) {
		if (!player->IsPlaying()) {
			player->PlayWave(&it->second.data, loop, volume);
			isPlayed = true;
			return;
		}
	}

	if (!isPlayed) 
	{
		sePlayers.push_back(std::make_unique<SoundPlayer>());
		sePlayers.back()->PlayWave(&it->second.data, loop, volume);
	}
}

// SE停止
void SoundManager::StopSE()
{
	for (auto&& player : sePlayers) {
		player->StopWave();
	}
	sePlayers.clear();
}

// 全てのサウンド停止
void SoundManager::StopAll()
{
	StopBGM();
	StopSE();
}

bool SoundManager::IsBGMPlaying()
{
	return bgmPlayer.IsPlaying();
}

bool SoundManager::LoadBGMFile(const std::string& soundname,const std::string& filepath)
{
	bgmMap[soundname];
	if (!SoundData::LoadWave(filepath, bgmMap[soundname].data))
	{
		std::string message = filepath + "の読み込みに失敗しました";
		MessageBoxA(NULL, message.c_str(), "Sound Error", MB_OK | MB_ICONWARNING);

		return false;
	}

	return true;
}

bool SoundManager::LoadSEFile(const std::string& soundname, const std::string& filepath)
{
	seMap[soundname];
	if (!SoundData::LoadWave(filepath, seMap[soundname].data))
	{
		std::string message = filepath + "の読み込みに失敗しました";
		MessageBoxA(NULL, message.c_str(), "Sound Error", MB_OK | MB_ICONWARNING);

		return false;
	}

	return true;
}
