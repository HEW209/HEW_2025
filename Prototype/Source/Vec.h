/*********************************************************************
 * @file   Vec.h
 * @brief  汎用多次元ベクトルクラス
 *
 * @author 芝晃佑
 * @date   2025/11/15
 *********************************************************************/

#pragma once

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <iostream>

#include <DXGameFrame.h>


// 算術型のコンセプト
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

// メンバー存在判定用のコンセプト
template <typename T> concept HasX = requires(T t) { t.x; };
template <typename T> concept HasY = requires(T t) { t.y; };
template <typename T> concept HasZ = requires(T t) { t.z; };
template <typename T> concept HasW = requires(T t) { t.w; };

template <typename T> concept HasR = requires(T t) { t.r; };
template <typename T> concept HasG = requires(T t) { t.g; };
template <typename T> concept HasB = requires(T t) { t.b; };
template <typename T> concept HasA = requires(T t) { t.a; };

template <typename T> concept HasU = requires(T t) { t.u; };
template <typename T> concept HasV = requires(T t) { t.v; };

template <typename T>
concept VectorLike = (HasX<T> || HasR<T> || HasU<T>);


// Vec用ストレージ
template <typename T, std::size_t N>
struct VectorStorage {
    std::array<T, N> data;
};

// 2次元用の特殊化
template <typename T>
struct VectorStorage<T, 2> {
    union {
        std::array<T, 2> data;
        struct { T x, y; };
        struct { T u, v; }; // テクスチャ座標用
    };
};

// 3次元用の特殊化
template <typename T>
struct VectorStorage<T, 3> {
    union {
        std::array<T, 3> data;
        struct { T x, y, z; };
        struct { T r, g, b; };  // 色情報用
    };
};

// 4次元用の特殊化
template <typename T>
struct VectorStorage<T, 4> {
    union {
        std::array<T, 4> data;
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };   // 色情報用
    };
};


/**
 * @brief 汎用多次元ベクトルクラス
 * @tparam T 値の型（算術型のみ）
 * @tparam N 次元数
 */
template <Arithmetic T, std::size_t N>
struct Vec : public VectorStorage<T, N>
{
    using VectorStorage<T, N>::data;

    // --- コンストラクタ ---

    constexpr Vec() : VectorStorage<T, N>{ {0} } {}

    template <typename... Args>
    requires (sizeof...(Args) == N) && (std::convertible_to<Args, T> && ...)
    constexpr Vec(Args... args) : VectorStorage<T, N>{ {static_cast<T>(args)...} } {}

    // すべての要素を scalar で埋めるコンストラクタ
    explicit constexpr Vec(T scalar) : VectorStorage<T, N>{} {
        for (std::size_t i = 0; i < N; ++i) data[i] = scalar;
    }

    // 異なる型や次元数からの変換コンストラクタ
    template <typename U, std::size_t M>
    explicit constexpr Vec(const Vec<U, M>& other, T fillValue = static_cast<T>(0))
        : VectorStorage<T, N>{}
    {
        constexpr std::size_t copyCount = (N < M) ? N : M;

        for (std::size_t i = 0; i < copyCount; ++i) {
            if constexpr (std::is_integral_v<T> && std::is_floating_point_v<U>) {
                U eps = (other[i] >= 0) ? EpsilonScalar : -EpsilonScalar;
                data[i] = static_cast<T>(other[i] + eps);
            }
            else {
                data[i] = static_cast<T>(other[i]);
            }
        }

        if constexpr (N > M) {
            for (std::size_t i = copyCount; i < N; ++i) {
                data[i] = fillValue;
            }
        }
    }

    // 外部クラスからの取り込みコンストラクタ
    template <typename Other>
    requires VectorLike<Other> && (!std::is_same_v<Other, Vec>)
    explicit constexpr Vec(const Other& other, T fillValue = static_cast<T>(0))
        : VectorStorage<T, N>{}
    {
        if constexpr (N >= 1) {
            if constexpr (HasX<Other>)      data[0] = static_cast<T>(other.x);
            else if constexpr (HasR<Other>) data[0] = static_cast<T>(other.r);
            else if constexpr (HasU<Other>) data[0] = static_cast<T>(other.u);
            else                            data[0] = fillValue;
        }

        if constexpr (N >= 2) {
            if constexpr (HasY<Other>)      data[1] = static_cast<T>(other.y);
            else if constexpr (HasG<Other>) data[1] = static_cast<T>(other.g);
            else if constexpr (HasV<Other>) data[1] = static_cast<T>(other.v);
            else                            data[1] = fillValue;
        }

        if constexpr (N >= 3) {
            if constexpr (HasZ<Other>)      data[2] = static_cast<T>(other.z);
            else if constexpr (HasB<Other>) data[2] = static_cast<T>(other.b);
            else                            data[2] = fillValue;
        }

        if constexpr (N >= 4) {
            if constexpr (HasW<Other>)      data[3] = static_cast<T>(other.w);
            else if constexpr (HasA<Other>) data[3] = static_cast<T>(other.a);
            else                            data[3] = fillValue;
        }
    }

    // 外部クラスへの変換
    template <typename Other>
        requires VectorLike<Other>
    explicit constexpr operator Other() const {
        Other result{};

        if constexpr (N >= 1) {
            if constexpr (HasX<Other>)      result.x = static_cast<decltype(Other::x)>(this->data[0]);
            else if constexpr (HasR<Other>) result.r = static_cast<decltype(Other::r)>(this->data[0]);
            else if constexpr (HasU<Other>) result.u = static_cast<decltype(Other::u)>(this->data[0]);
        }

        if constexpr (N >= 2) {
            if constexpr (HasY<Other>)      result.y = static_cast<decltype(Other::y)>(this->data[1]);
            else if constexpr (HasG<Other>) result.g = static_cast<decltype(Other::g)>(this->data[1]);
            else if constexpr (HasV<Other>) result.v = static_cast<decltype(Other::v)>(this->data[1]);
        }

        if constexpr (N >= 3) {
            if constexpr (HasZ<Other>)      result.z = static_cast<decltype(Other::z)>(this->data[2]);
            else if constexpr (HasB<Other>) result.b = static_cast<decltype(Other::b)>(this->data[2]);
        }

        if constexpr (N >= 4) {
            if constexpr (HasW<Other>)      result.w = static_cast<decltype(Other::w)>(this->data[3]);
            else if constexpr (HasA<Other>) result.a = static_cast<decltype(Other::a)>(this->data[3]);
        }

        return result;
    }

    // --- アクセサ ---
    constexpr T& operator[](std::size_t i) { return data[i]; }
    constexpr const T& operator[](std::size_t i) const { return data[i]; }

    // --- 演算 ---
    
    // ベクトル同士の演算
    constexpr Vec& operator+=(const Vec& other) {
        for (std::size_t i = 0; i < N; ++i) data[i] += other[i];
        return *this;
    }

    constexpr Vec& operator-=(const Vec& other) {
        for (std::size_t i = 0; i < N; ++i) data[i] -= other[i];
        return *this;
    }

    constexpr Vec& operator*=(const Vec& other) {
        for (std::size_t i = 0; i < N; ++i) data[i] *= other[i];
        return *this;
    }

    constexpr Vec& operator/=(const Vec& other) {
        for (std::size_t i = 0; i < N; ++i) data[i] /= other[i];
        return *this;
    }

    // スカラーとの演算
    constexpr Vec& operator*=(T scalar) {
        for (std::size_t i = 0; i < N; ++i) data[i] *= scalar;
        return *this;
    }

    constexpr Vec& operator/=(T scalar) {
        T inv = static_cast<T>(1) / scalar;
        for (std::size_t i = 0; i < N; ++i) data[i] *= inv;
        return *this;
    }

    // 単項演算子
    constexpr Vec operator-() const {
        Vec result;
        for (std::size_t i = 0; i < N; ++i) result[i] = -data[i];
        return result;
    }

    constexpr Vec operator+() const {
        return *this;
    }

    // 二項演算子
    friend constexpr Vec operator+(Vec lhs, const Vec& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend constexpr Vec operator-(Vec lhs, const Vec& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend constexpr Vec operator*(Vec lhs, const Vec& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr Vec operator/(Vec lhs, const Vec& rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr Vec operator*(Vec lhs, T scalar) {
        lhs *= scalar;
        return lhs;
    }

    friend constexpr Vec operator*(T scalar, Vec rhs) {
        rhs *= scalar;
        return rhs;
    }

    friend constexpr Vec operator/(Vec lhs, T scalar) {
        lhs /= scalar;
        return lhs;
    }

    // クォータニオンによる回転
    friend constexpr Vec<float, N> operator*(const Quaternion& q, const Vec& v)
        requires (N == 3)
    {
        Vec<float, N> vec(v);
        Vec<float, N> qVec(q.x, q.y, q.z);

        float qScalar = q.w;

        Vec<float, N> t = static_cast<Vec<float, N>>(2 * qVec.Cross(vec));

        return vec + (qScalar * t) + qVec.Cross(t);
    }

    // 比較演算子
    constexpr bool operator==(const Vec& other) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (data[i] != other[i]) return false;
        }
        return true;
    }

    // 近似比較
    constexpr bool NearEqual(const Vec& other, T epsilon = EpsilonScalar) const {
        if constexpr (!std::is_floating_point_v<T>) {
            return *this == other;
        }

        for (std::size_t i = 0; i < N; ++i) {
            if (std::abs(data[i] - other[i]) > epsilon) return false;
        }
        return true;
    }

    // ゼロベクトルか判定（浮動小数点誤差考慮）
    constexpr bool IsZero(T epsilon = EpsilonScalar) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (std::abs(data[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }

    // 内積
    constexpr T Dot(const Vec& other) const {
        T sum = static_cast<T>(0);
        for (std::size_t i = 0; i < N; ++i) {
            sum += data[i] * other[i];
        }
        return sum;
    }

    // 外積
    constexpr Vec Cross(const Vec& other) const
        requires (N == 3)
    {
        return Vec(
            data[1] * other.data[2] - data[2] * other.data[1], // y*z - z*y
            data[2] * other.data[0] - data[0] * other.data[2], // z*x - x*z
            data[0] * other.data[1] - data[1] * other.data[0]  // x*y - y*x
        );
    }

    // 長さの二乗
    constexpr T LengthSq() const {
        return Dot(*this);
    }

    // 長さ
    constexpr T Length() const {
        return static_cast<T>(std::sqrt(LengthSq()));
    }

    // 正規化ベクトルを返す
    constexpr Vec Normalized() const {
        T lenSq = LengthSq();

        if (lenSq < std::numeric_limits<T>::epsilon()) {
            return Vec(); // ゼロベクトル
        }

        T lenInv = static_cast<T>(1) / std::sqrt(lenSq);
        return *this * lenInv;
    }

    // 自身を正規化する
    constexpr void Normalize() {
        *this = Normalized();
    }

    // 距離
    constexpr T DistanceFrom(const Vec& other) const {
        return (*this - other).Length();
    }

    // 距離の二乗
    constexpr T DistanceSqFrom(const Vec& other) const {
        return (*this - other).LengthSq();
    }
    
    // 出力用（デバッグ）
    friend std::ostream& operator<<(std::ostream& os, const Vec& v) {
        os << "(";
        for (std::size_t i = 0; i < N; ++i) {
            os << v[i] << (i < N - 1 ? ", " : "");
        }
        os << ")";
        return os;
    }


    // 極小の値のスカラー
    static constexpr T EpsilonScalar = []() {
        if constexpr (std::is_same_v<T, float>) {
            return 1e-5f;
        }
        else if constexpr (std::is_same_v<T, double>) {
            return 1e-9;
        }
        else {
            return static_cast<T>(0);
        }
    }();

    // 極小のベクトル
    static constexpr Vec Epsilon() {
        return Vec(EpsilonScalar);
    }

    // ゼロベクトル (0, 0, 0, ...)
    static constexpr Vec Zero() {
        return Vec();
    }

    // 全て1のベクトル (1, 1, 1, ...)
    static constexpr Vec One() {
        return Vec(static_cast<T>(1));
    }

    // 右 (1, 0, 0)
    static constexpr Vec Right() requires (N >= 1) {
        Vec v;
        v[0] = static_cast<T>(1);
        return v;
    }

    // 左 (-1, 0, 0)
    static constexpr Vec Left() requires (N >= 1) {
        Vec v;
        v[0] = static_cast<T>(-1);
        return v;
    }

    // 上方向 (0, 1, 0)
    static constexpr Vec Up() requires (N >= 2) {
        Vec v;
        v[1] = static_cast<T>(1);
        return v;
    }

    // 下方向 (0, -1, 0)
    static constexpr Vec Down() requires (N >= 2) {
        Vec v;
        v[1] = static_cast<T>(-1);
        return v;
    }

    // 前方・奥 (0, 0, 1)
    static constexpr Vec Forward() requires (N >= 3) {
        Vec v;
        v[2] = static_cast<T>(1);
        return v;
    }

    // 後方・手前 (0, 0, -1)
    static constexpr Vec Back() requires (N >= 3) {
        Vec v;
        v[2] = static_cast<T>(-1);
        return v;
    }
};


// エイリアス
using Vec2 = Vec<float, 2>;
using Vec3 = Vec<float, 3>;
using Vec4 = Vec<float, 4>;
using Vec2Int = Vec<int, 2>;
using Vec3Int = Vec<int, 3>;
using Vec4Int = Vec<int, 4>;