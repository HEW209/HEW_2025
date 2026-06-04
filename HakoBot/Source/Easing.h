#pragma once
#include <cmath>
#include <limits>
#include <algorithm>

// イージング関数
struct Easing
{
private:
	// 定数定義 (円周率)
	template <class Ty>
	static constexpr Ty Pi{ static_cast<Ty>(3.141592653589793) };

	// 度数法 -> 弧度法
	template<class Ty>
	[[nodiscard]] static inline Ty ToRadian(const Ty angle)
	{
		return angle * Pi<Ty> / static_cast<Ty>(180.0);
	}

	// 浮動小数点の誤差許容比較
	template<typename Ty>
	[[nodiscard]] static inline bool AdjEqual(const Ty a, const Ty b)
	{
		return std::abs(a - b) <= std::numeric_limits<Ty>::epsilon();
	}

public:
	// 基本的な引数: (現在の時間, 総時間, 終了値, 開始値)
	// 内部で max -= min を行うため、第3引数は「移動量」ではなく「目標値」として扱っています。

	// ----------------------------------------------------------------
	// Quad (2乗)
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InQuad(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		return max * time * time + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutQuad(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		return -max * time * (time - static_cast<Ty>(2.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutQuad(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
			return max / static_cast<Ty>(2.0) * time * time + min;

		--time;
		return -max / static_cast<Ty>(2.0) * (time * (time - static_cast<Ty>(2.0)) - static_cast<Ty>(1.0)) + min;
	}

	// ----------------------------------------------------------------
	// Cubic (3乗)
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InCubic(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		return max * time * time * time + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutCubic(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time = time / totaltime - static_cast<Ty>(1.0);
		return max * (time * time * time + static_cast<Ty>(1.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutCubic(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
			return max / static_cast<Ty>(2.0) * time * time * time + min;

		time -= static_cast<Ty>(2.0);
		return max / static_cast<Ty>(2.0) * (time * time * time + static_cast<Ty>(2.0)) + min;
	}

	// ----------------------------------------------------------------
	// Quart (4乗)
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InQuart(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		return max * time * time * time * time + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutQuart(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time = time / totaltime - static_cast<Ty>(1.0);
		return -max * (time * time * time * time - static_cast<Ty>(1.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutQuart(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
			return max / static_cast<Ty>(2.0) * time * time * time * time + min;

		time -= static_cast<Ty>(2.0);
		return -max / static_cast<Ty>(2.0) * (time * time * time * time - static_cast<Ty>(2.0)) + min;
	}

	// ----------------------------------------------------------------
	// Quint (5乗)
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InQuint(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		return max * time * time * time * time * time + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutQuint(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time = time / totaltime - static_cast<Ty>(1.0);
		return max * (time * time * time * time * time + static_cast<Ty>(1.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutQuint(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
			return max / static_cast<Ty>(2.0) * time * time * time * time * time + min;

		time -= static_cast<Ty>(2.0);
		return max / static_cast<Ty>(2.0) * (time * time * time * time * time + static_cast<Ty>(2.0)) + min;
	}

	// ----------------------------------------------------------------
	// Sine
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InSine(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		return -max * std::cos(time * ToRadian(static_cast<Ty>(90.0)) / totaltime) + max + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutSine(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		return max * std::sin(time * ToRadian(static_cast<Ty>(90.0)) / totaltime) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutSine(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		return -max / static_cast<Ty>(2.0) * (std::cos(time * Pi<Ty> / totaltime) - static_cast<Ty>(1.0)) + min;
	}

	// ----------------------------------------------------------------
	// Exp (Exponential)
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InExp(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		return AdjEqual<Ty>(time, static_cast<Ty>(0.0)) ?
			min :
			max * std::pow(static_cast<Ty>(2.0), static_cast<Ty>(10.0) * (time / totaltime - static_cast<Ty>(1.0))) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutExp(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		return AdjEqual<Ty>(time, totaltime) ?
			max + min :
			max * (-std::pow(static_cast<Ty>(2.0), -static_cast<Ty>(10.0) * time / totaltime) + static_cast<Ty>(1.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutExp(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		if (AdjEqual<Ty>(time, static_cast<Ty>(0.0))) return min;
		if (AdjEqual<Ty>(time, totaltime)) return max;

		max -= min;
		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
			return max / static_cast<Ty>(2.0) * std::pow(static_cast<Ty>(2.0), static_cast<Ty>(10.0) * (time - static_cast<Ty>(1.0))) + min;

		--time;
		return max / static_cast<Ty>(2.0) * (-std::pow(static_cast<Ty>(2.0), -static_cast<Ty>(10.0) * time) + static_cast<Ty>(2.0)) + min;
	}

	// ----------------------------------------------------------------
	// Circ (Circular)
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InCirc(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		// クランプ処理
		time = std::clamp(time, static_cast<Ty>(-1.0), static_cast<Ty>(1.0));

		return -max * (std::sqrt(static_cast<Ty>(1.0) - time * time) - static_cast<Ty>(1.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutCirc(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time = time / totaltime - static_cast<Ty>(1.0); // 修正: ここが間違っていました
		time = std::clamp(time, static_cast<Ty>(-1.0), static_cast<Ty>(1.0));

		return max * std::sqrt(static_cast<Ty>(1.0) - time * time) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutCirc(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
		{
			time = std::clamp(time, static_cast<Ty>(-1.0), static_cast<Ty>(1.0));
			return -max / static_cast<Ty>(2.0) * (std::sqrt(static_cast<Ty>(1.0) - time * time) - static_cast<Ty>(1.0)) + min;
		}

		time -= static_cast<Ty>(2.0);
		time = std::clamp(time, static_cast<Ty>(-1.0), static_cast<Ty>(1.0));
		return max / static_cast<Ty>(2.0) * (std::sqrt(static_cast<Ty>(1.0) - time * time) + static_cast<Ty>(1.0)) + min;
	}

	// ----------------------------------------------------------------
	// Back
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty InBack(Ty time, Ty totaltime, Ty back, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;
		// 慣例的にback係数は 1.70158 付近がデフォルトですが、引数で受け取る仕様を維持
		return max * time * time * ((back + static_cast<Ty>(1.0)) * time - back) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutBack(Ty time, Ty totaltime, Ty back, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time = time / totaltime - static_cast<Ty>(1.0);
		return max * (time * time * ((back + static_cast<Ty>(1.0)) * time + back) + static_cast<Ty>(1.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutBack(Ty time, Ty totaltime, Ty back, Ty max = 1, Ty min = 0)
	{
		max -= min;
		back *= static_cast<Ty>(1.525); // InOutの場合は反動を強くする

		time /= (totaltime / static_cast<Ty>(2.0));

		if (time < static_cast<Ty>(1.0))
			return max / static_cast<Ty>(2.0) * (time * time * ((back + static_cast<Ty>(1.0)) * time - back)) + min;

		time -= static_cast<Ty>(2.0);
		return max / static_cast<Ty>(2.0) * (time * time * ((back + static_cast<Ty>(1.0)) * time + back) + static_cast<Ty>(2.0)) + min;
	}

	// ----------------------------------------------------------------
	// Bounce
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutBounce(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		max -= min;
		time /= totaltime;

		const Ty div = static_cast<Ty>(2.75);
		const Ty mul = static_cast<Ty>(7.5625);

		if (time < static_cast<Ty>(1.0) / div)
		{
			return max * (mul * time * time) + min;
		}
		else if (time < static_cast<Ty>(2.0) / div)
		{
			time -= static_cast<Ty>(1.5) / div;
			return max * (mul * time * time + static_cast<Ty>(0.75)) + min;
		}
		else if (time < static_cast<Ty>(2.5) / div)
		{
			time -= static_cast<Ty>(2.25) / div;
			return max * (mul * time * time + static_cast<Ty>(0.9375)) + min;
		}
		else
		{
			time -= static_cast<Ty>(2.625) / div;
			return max * (mul * time * time + static_cast<Ty>(0.984375)) + min;
		}
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InBounce(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		// InBounceは OutBounce を逆算して求める
		return (max - min) - OutBounce<Ty>(totaltime - time, totaltime, max - min, static_cast<Ty>(0.0)) + min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutBounce(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		if (time < totaltime / static_cast<Ty>(2.0))
		{
			return InBounce<Ty>(time * static_cast<Ty>(2.0), totaltime, max - min, static_cast<Ty>(0.0)) * static_cast<Ty>(0.5) + min;
		}
		else
		{
			return OutBounce<Ty>(time * static_cast<Ty>(2.0) - totaltime, totaltime, max - min, static_cast<Ty>(0.0)) * static_cast<Ty>(0.5) + min + (max - min) * static_cast<Ty>(0.5);
		}
	}

	// ----------------------------------------------------------------
	// Linear
	// ----------------------------------------------------------------
	template<typename Ty = float>
	[[nodiscard]] static inline Ty Linear(Ty time, Ty totaltime, Ty max = 1, Ty min = 0)
	{
		return (max - min) * time / totaltime + min;
	}
};