/*********************************************************************
 * @file   DynamicDimensionalArray.h
 * @brief  可変長多次元配列
 *
 * @author 芝晃佑
 * @date   2025/11/15
 *********************************************************************/

#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <concepts>
#include <stdexcept>

// N次元可変長配列クラス
template <typename T, size_t Dimensions>
class DynamicDimArray {
public:
    using Container = std::vector<T>;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;

    // コンストラクタ（各次元のサイズを可変長引数で受け取る）
    template <typename... Dims>
        requires (sizeof...(Dims) == Dimensions)
    DynamicDimArray(Dims... dims) {
        // 引数を配列に格納
        m_dims = { static_cast<size_t>(dims)... };
        allocate(T());
    }

    // デフォルトコンストラクタ
    DynamicDimArray() {
        m_dims.fill(0);
    }

    // データを破棄し、サイズを0にする
    void Clear() {
        m_data.clear();
        m_dims.fill(0);
    }

    // データの生ポインタを取得
    // Tがboolの場合は無効

    T* Data() requires (!std::is_same_v<T, bool>) {
        return m_data.data();
    }

    const T* Data() const requires (!std::is_same_v<T, bool>) {
        return m_data.data();
    }

    // 境界チェック付きアクセス
    template <typename... Indices>
        requires (sizeof...(Indices) == Dimensions)
    reference at(Indices... indices) {
        std::array<size_t, Dimensions> idxs = { static_cast<size_t>(indices)... };
        checkBounds(idxs);
        return m_data[getIndex(idxs)];
    }

    template <typename... Indices>
        requires (sizeof...(Indices) == Dimensions)
    value_type at(Indices... indices) const {
        std::array<size_t, Dimensions> idxs = { static_cast<size_t>(indices)... };
        checkBounds(idxs);
        return m_data[getIndex(idxs)];
    }

    // --- アクセッサ ---

    template <typename... Indices>
        requires (sizeof...(Indices) == Dimensions)
    reference operator()(Indices... indices) {
        assert((checkBoundsDebug({ static_cast<size_t>(indices)... }), true));
        return m_data[getIndex({ static_cast<size_t>(indices)... })];
    }

    template <typename... Indices>
        requires (sizeof...(Indices) == Dimensions)
    value_type operator()(Indices... indices) const {
        assert((checkBoundsDebug({ static_cast<size_t>(indices)... }), true));
        return m_data[getIndex({ static_cast<size_t>(indices)... })];
    }

    // --- サイズ変更 ---

    // 各次元の新しいサイズを指定してリサイズ（引数の最後にデフォルト値を指定可能）
    template <typename... Args>
    void Resize(Args... args) {
        constexpr size_t ArgCount = sizeof...(Args);

        static_assert(ArgCount == Dimensions || ArgCount == Dimensions + 1,
            "Resizeの引数は「次元数」か「次元数+1(デフォルト値)」である必要があります");

        if constexpr (ArgCount == Dimensions) {
            // デフォルト値の指定なし
            std::array<size_t, Dimensions> dims = { static_cast<size_t>(args)... };
            ResizeImpl(dims, T());
        }
        else if constexpr (ArgCount == Dimensions + 1) {
            // デフォルト値の指定あり
            auto argsTuple = std::make_tuple(args...);

            // 最後の引数をデフォルト値として取得
            T val = static_cast<T>(std::get<Dimensions>(argsTuple));

            // 最初のDimensions個の引数をサイズとして取得
            std::array<size_t, Dimensions> dims;

            // タプルから配列へ展開
            auto unpack = [&]<size_t... Is>(std::index_sequence<Is...>) {
                ((dims[Is] = static_cast<size_t>(std::get<Is>(argsTuple))), ...);
            };
            unpack(std::make_index_sequence<Dimensions>{});

            ResizeImpl(dims, val);
        }
    }

    // 指定した次元だけ拡張する
    // dimensionIndex: 0=X, 1=Y, 2=Z...
    void Expand(size_t dimensionIndex, size_t addAmount, const T& val = T()) {
        if (dimensionIndex >= Dimensions) throw std::out_of_range("Dimension index out of range");

        std::array<size_t, Dimensions> newDims = m_dims;
        newDims[dimensionIndex] += addAmount;

        // 配列を引数パック展開してResizeに渡す
        auto resizeLambda = [&](auto... args) { this->Resize(args..., val); };
        std::apply(resizeLambda, newDims);
    }

    // --- 情報取得 ---

    size_t GetSize(size_t dimensionIndex) const {
        if (dimensionIndex >= Dimensions) return 0;
        return m_dims[dimensionIndex];
    }

    size_t GetTotalElements() const { return m_data.size(); }

    // --- イテレータ ---

    iterator begin() { return m_data.begin(); }
    iterator end() { return m_data.end(); }
    const_iterator begin() const { return m_data.begin(); }
    const_iterator end() const { return m_data.end(); }

    // イテレータから座標を逆算
    std::array<size_t, Dimensions> GetCoordinates(const_iterator it) const {
        size_t index = std::distance(m_data.cbegin(), it);
        std::array<size_t, Dimensions> coords;

        size_t tempIdx = index;
        for (size_t i = 0; i < Dimensions; ++i) {
            coords[i] = tempIdx % m_dims[i];
            tempIdx /= m_dims[i];
        }
        return coords;
    }

private:
    Container m_data;
    std::array<size_t, Dimensions> m_dims;

    void allocate(const T& val) {
        size_t total = 1;
        for (auto d : m_dims) total *= d;
        m_data.resize(total, val);
    }

    // 座標からインデックスに変換
    size_t getIndex(const std::array<size_t, Dimensions>& indices) const {
        size_t index = 0;
        size_t stride = 1;

        for (size_t i = 0; i < Dimensions; ++i) {
            assert(indices[i] < m_dims[i]); // デバッグ用境界チェック
            index += indices[i] * stride;
            stride *= m_dims[i];
        }
        return index;
    }

    // 境界チェック
    void checkBounds(const std::array<size_t, Dimensions>& indices) const {
        for (size_t i = 0; i < Dimensions; ++i) {
            if (indices[i] >= m_dims[i]) {
                throw std::out_of_range("coordinates out of range");
            }
        }
    }

    void checkBoundsDebug(const std::array<size_t, Dimensions>& indices) const {
        for (size_t i = 0; i < Dimensions; ++i) {
            assert(indices[i] < m_dims[i] && "Index out of bounds");
        }
    }

    // リサイズ処理
    void ResizeImpl(const std::array<size_t, Dimensions>& newDims, const T& val) {
        // サイズが変わっていなければ何もしない
        if (newDims == m_dims) return;

        // 新しい合計サイズを計算
        size_t newTotalSize = 1;
        for (auto d : newDims) newTotalSize *= d;

        // 新しいデータバッファ
        Container newData(newTotalSize, val);

        // データが存在する場合のみコピー処理
        if (!m_data.empty()) {
            recursiveCopy(
                m_data, m_dims,      // 送り元
                newData, newDims,    // 送り先
                Dimensions - 1,      // 現在処理中の次元
                0, 0                 // オフセット
            );
        }

        // メンバ変数の更新
        m_data = std::move(newData);
        m_dims = newDims;
    }

    // 再帰的コピー関数
    // currentDim: 現在処理している次元
    void recursiveCopy(
        const Container& srcData, const std::array<size_t, Dimensions>& srcDims,
        Container& dstData, const std::array<size_t, Dimensions>& dstDims,
        size_t currentDim,
        size_t srcBaseIndex, size_t dstBaseIndex
    ) {
        // ストライド計算
        size_t srcStride = 1;
        size_t dstStride = 1;
        for (size_t i = 0; i < currentDim; ++i) {
            srcStride *= srcDims[i];
            dstStride *= dstDims[i];
        }

        // この次元でコピーする要素数
        size_t count = std::min(srcDims[currentDim], dstDims[currentDim]);

        if (currentDim == 0) {
            auto srcStart = srcData.begin() + srcBaseIndex;
            auto dstStart = dstData.begin() + dstBaseIndex;
            std::copy(srcStart, srcStart + count, dstStart);
        }
        else {
            // 再帰
            for (size_t i = 0; i < count; ++i) {
                recursiveCopy(
                    srcData, srcDims,
                    dstData, dstDims,
                    currentDim - 1,
                    srcBaseIndex + (i * srcStride),
                    dstBaseIndex + (i * dstStride)
                );
            }
        }
    }
};