/******************************************************************//**
 * @file   Math.h
 * @brief  ゲーム用数学を扱う
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

/**
 * @brief ゲーム用数学を扱う
 */
class Math
{
public:
	/// 円周率
	static constexpr float PI = 3.14159265f;

	/// 円周率の2倍
	static constexpr float TAU = PI * 2.0f;

	// ラジアン変換
	static constexpr float Deg2Rad = PI / 180.0f;

	// デグリー変換
	static constexpr float Rad2Deg = 180.0f / PI;

	/// 浮動小数点誤差
	static constexpr float Epsilon = 1e-7f;

	/**
	 * @brief 浮動小数点誤差を考慮した比較を行う
	 * @param a	比較値1
	 * @param b 比較値2
	 * @return 値がほぼ等しい場合、trueを返す
	 */
	static bool Approximately(float a, float b);

	/**
	 * @brief 値を制限する
	 * @param min 最小値
	 * @param max 最大値
	 * @param value 現在値
	 * @return min～max間に制限された値
	 */
	static float Clamp(float min, float max, float value);

	/**
	 * @brief 0～1の範囲で値を制限する
	 * @param value 現在値
	 * @return 0～1の範囲に制限された値
	 */
	static float Clamp01(float value);

	/**
	 * @brief 線形補間を行う
	 * @param a 開始値
	 * @param b 終了値
	 * @param t 補完値 (0～1に制限されます)
	 * @return aとbの間でtによって線形補間された値
	 */
	static float Lerp(float a, float b, float t);

	/**
	 * @brief 目標地点に向かって指定量移動した値を返す
	 * @param current 現在値
	 * @param target 目標値
	 * @param maxDelta 移動量
	 * @return currentからtargetまで、maxDeltaだけ移動した値
	 */
	static float MoveTowards(float current, float target, float maxDelta);

	/**
	 * @brief 目標角に向かって指定量回転した値を返す
	 * @param current 現在角
	 * @param target 目標角
	 * @param maxDelta 回転量
	 * @return currentからtargetまで、maxDeltaだけ回転した値
	 */
	static float MoveTowardsAngle(float current, float target, float maxDelta);

	/**
	 * @brief デグリーからラジアンに変換する
	 * @param deg デグリー角
	 * @return 変換したラジアン角
	 */
	static float DegToRad(float deg);

	/**
	 * @brief ラジアンからデグリーに変換する
	 * @param deg ラジアン角
	 * @return 変換したデグリー角
	 */
	static float RadToDeg(float rad);

	/**
	 * @brief 二つの角度間の差を求める
	 * @param a 角度1
	 * @param b 角度2
	 * @return 二つの角度間の最小角度 (-180～180)
	 */
	static float DeltaAngle(float a, float b);

private:
	Math() = delete;
};
