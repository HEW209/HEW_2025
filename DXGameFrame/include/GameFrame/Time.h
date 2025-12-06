/******************************************************************//**
 * @file   Time.h
 * @brief  ゲーム内時間を管理する
 * 
 * @author 石田怜
 * @date   2025/12/06
 *********************************************************************/
#pragma once

/**
 * @brief ゲーム内時間を管理する
 */
class Time
{
public:
	/**
	 * @brief ゲーム内時間を初期化する
	 */
	static void Init();

	/**
	 * @brief シーン内経過時間をリセットする
	 */
	static void ResetSceneTime();

	/**
	 * @brief ゲーム内時間を更新する
	 * @param delta 前のフレームからの経過時間
	 */
	static void Update(float delta);

	/**
	 * @brief 前のフレームからの経過時間を取得する
	 * @return 前のフレームからの経過時間
	 */
	static float GetDeltaTime();

	/**
	 * @brief シーン内経過時間を取得する
	 * @return シーン内経過時間
	 */
	static float GetSceneTime();

	/**
	 * @brief ゲーム起動からの経過時間を取得する
	 * @return ゲーム起動からの経過時間
	 */
	static float GetTotalTime();

private:
	Time() = delete;

	/// 前のフレームからの経過時間
	static float s_deltaTime;

	/// シーン内経過時間
	static float s_sceneTime;

	/// ゲーム起動からの経過時間
	static float s_totalTime;
};
