/*****************************************************************//**
 * @file   MathUtil.h
 * @brief  基本的なゲーム用数学を扱う
 * 
 * @author 石田怜
 * @date   2025/09/15
 *********************************************************************/
#pragma once

/**
 * @brief ゲーム用数学で使用する定数
 */
namespace Math
{
	/// 円周率
	constexpr float PI = 3.14159265f;

	/// 円周率の2倍
	constexpr float TAU = PI * 2.0f;

	/// 誤差許容値
	constexpr float EPS = 1e-6f;

	/// ゼロ付近での誤差許容値
	constexpr float ZERO_EPS = 1e-7f;
}

/**
 * @brief ゲーム用数学を扱う
 */
class MathUtil
{
public:
	/**
	 * @brief 浮動小数点誤差を考慮した比較を行う
	 * @param a	比較に使用する値1
	 * @param b 比較に使用する値2
	 * @return 値がほぼ等しい場合、trueを返す
	 */
	static bool Approximately(float a, float b);

	/**
	 * @brief デグリーからラジアンに変換する
	 * @param deg デグリー角度
	 * @return 変換したラジアン角度
	 */
	static float DegToRad(float deg);

	/**
	 * @brief ラジアンからデグリーに変換する
	 * @param deg ラジアン角度
	 * @return 変換したデグリー角度
	 */
	static float RadToDeg(float rad);

	/**
	 * @brief 角度を正規化する
	 * @param angle 現在の角度
	 * @return (0 =< angle < 360) の範囲に正規化された角度
	 */
	static float NormalizeAngle(float angle);

	/**
	 * @brief 角度を指定した範囲内に収める
	 * @details 360度を跨ぐ場合でも判定できます
	 * @param angle 現在の角度
	 * @param min 最小角度
	 * @param max 最大角度
	 * @return 範囲内に収めた正規化角度
	 */
	static float ClampAngle(float angle, float min, float max);

	/**
	 * @brief 二つの角度間の差を求める
	 * @param angle_a 角度1
	 * @param angle_b 角度2
	 * @return 二つの角度の差
	 */
	static float AngleDistance(float angle_a, float angle_b);

private:
	MathUtil() = delete;
};
