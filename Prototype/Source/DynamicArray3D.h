#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>


template <typename T>
class DynamicArray3D {
public:
    // コンストラクタ
    DynamicArray3D(size_t width, size_t height, size_t depth, const T& initialValue = T())
        : m_width(width), m_height(height), m_depth(depth)
    {
        m_data.resize(width * height * depth, initialValue);
    }

    // デフォルトコンストラクタ
    DynamicArray3D() : m_width(0), m_height(0), m_depth(0) {}

    // アクセッサ (読み書き用)
    // 境界チェックなし（高速化のため）
    T& operator()(size_t x, size_t y, size_t z)
    {
        assert(x < m_width && y < m_height && z < m_depth);
        return m_data[getIndex(x, y, z)];
    }

    // アクセッサ (読み取り専用)
    const T& operator()(size_t x, size_t y, size_t z) const
    {
        assert(x < m_width && y < m_height && z < m_depth);
        return m_data[getIndex(x, y, z)];
    }

    // 安全なアクセッサ (境界チェックあり)
    T& at(size_t x, size_t y, size_t z)
    {
        if (x >= m_width || y >= m_height || z >= m_depth) {
            throw std::out_of_range("Grid coordinates out of range");
        }
        return m_data[getIndex(x, y, z)];
    }

    // グリッドのサイズを変更する関数
    // 新しいサイズに合わせてメモリを再確保し、既存のデータを適切な位置に移動する
    // 拡張された領域は defaultValue で埋められる
    void Resize(size_t newWidth, size_t newHeight, size_t newDepth, const T& defaultValue = T())
    {
        if (newWidth == m_width && newHeight == m_height && newDepth == m_depth) {
            return;
        }

        // 新しい1次元配列を用意
        std::vector<T> newData(newWidth * newHeight * newDepth, defaultValue);

        // 既存のデータを新しいバッファへコピー/移動
        // 共通する領域（重なっている部分）のみを移行する
        const size_t copyWidth = std::min(m_width, newWidth);
        const size_t copyHeight = std::min(m_height, newHeight);
        const size_t copyDepth = std::min(m_depth, newDepth);

        for (size_t z = 0; z < copyDepth; ++z) {
            for (size_t y = 0; y < copyHeight; ++y) {
                size_t oldBaseIndex = (z * m_height + y) * m_width;
                size_t newBaseIndex = (z * newHeight + y) * newWidth;

                for (size_t x = 0; x < copyWidth; ++x) {
                    newData[newBaseIndex + x] = std::move(m_data[oldBaseIndex + x]);
                }
            }
        }

        // メンバ変数を更新
        m_data = std::move(newData);
        m_width = newWidth;
        m_height = newHeight;
        m_depth = newDepth;
    }

    // 幅を増やすヘルパー
    void ExpandWidth(size_t addAmount, const T& val = T()) {
        Resize(m_width + addAmount, m_height, m_depth, val);
    }

    // 高さを増やすヘルパー
    void ExpandHeight(size_t addAmount, const T& val = T()) {
        Resize(m_width, m_height + addAmount, m_depth, val);
    }

    // 奥行きを増やすヘルパー
    void ExpandDepth(size_t addAmount, const T& val = T()) {
        Resize(m_width, m_height, m_depth + addAmount, val);
    }

    // 生データへのポインタ取得
    T* Data() { return m_data.data(); }
    const T* Data() const { return m_data.data(); }

    // 次元情報の取得
    size_t GetWidth() const { return m_width; }
    size_t GetHeight() const { return m_height; }
    size_t GetDepth() const { return m_depth; }
    size_t GetTotalSize() const { return m_data.size(); }

    // 全クリア
    void Clear()
    {
        m_data.clear();
        m_width = m_height = m_depth = 0;
    }

private:
    // 3次元座標を1次元インデックスに変換する
    // Layout: X (Width) -> Y (Height) -> Z (Depth)
    inline size_t getIndex(size_t x, size_t y, size_t z) const
    {
        return x + (y * m_width) + (z * m_width * m_height);
    }

    std::vector<T> m_data;
    size_t m_width = 0;
    size_t m_height = 0;
    size_t m_depth = 0;
};