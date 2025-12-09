/*****************************************************************//**
 * @file   Color.h
 * @brief  色情報を扱う
 *
 * @author 石田怜
 * @date   2025/09/23
 *********************************************************************/
#pragma once

/// 0～255 表記のRGB値の最大値
constexpr float RGB255_Max = 255.0f;

/// 0～255 表記のRGB値の最大値の逆数
constexpr float RGB255_InvMax = 1.0f / RGB255_Max;

/**
 * @brief 色情報を扱う
 */
class Color
{
public:
	Color();
	Color(float red, float green, float blue, float alpha = 1.0f);
	~Color() = default;

	/// r成分 (0.0～1.0)
	float r;

	/// g成分 (0.0～1.0)
	float g;

	/// b成分 (0.0～1.0)
	float b;

	/// a成分 (0.0～1.0)
	float a;

	// 比較・代入
	Color& operator=(const Color& other);
	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;

	// 加算・減算・乗算
	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(const Color& other) const;
	Color& operator+=(const Color& other);
	Color& operator-=(const Color& other);
	Color& operator*=(const Color& other);

	/**
	 * @brief 0～255の範囲のRGB値から色を設定する
	 * @param red r成分
	 * @param green g成分
	 * @param brue b成分
	 * @param alpha a成分
	 */
	void SetColor255(float red, float green, float blue, float alpha = RGB255_Max);

	/**
	 * @brief 正規化された色を取得する
	 * @return 全ての成分が 0.0～1.0 に収まった色を返す
	 */
	Color Normalized();

public:
	/// 黒色
	static const Color black;

	/// 白色
	static const Color white;

	/// 赤色
	static const Color red;

	/// 緑色
	static const Color green;

	/// 青色
	static const Color blue;
};