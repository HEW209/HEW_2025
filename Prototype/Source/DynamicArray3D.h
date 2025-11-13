#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>


template <typename T>
class DynamicArray3D {
public:
    // イテレータ型定義 (vectorのイテレータをそのまま使う)
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    // STL互換のための型定義
    using value_type = T;
    using size_type = std::size_t;

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

    // 先頭イテレータ
    iterator begin() noexcept { return m_data.begin(); }
    const_iterator begin() const noexcept { return m_data.begin(); }
    const_iterator cbegin() const noexcept { return m_data.cbegin(); }

    // 末尾イテレータ
    iterator end() noexcept { return m_data.end(); }
    const_iterator end() const noexcept { return m_data.end(); }
    const_iterator cend() const noexcept { return m_data.cend(); }

    // イテレータから座標 (x, y, z) を逆算するヘルパー関数
    void GetCoordinates(const_iterator it, size_t& outX, size_t& outY, size_t& outZ) const {
        // 先頭からの距離(インデックス)を計算
        size_t index = std::distance(m_data.cbegin(), it);

        // インデックスから3次元座標へ変換
        // index = x + (y * w) + (z * w * h) の逆算
        size_t xyArea = m_width * m_height;
        outZ = index / xyArea;
        size_t rem = index % xyArea;
        outY = rem / m_width;
        outX = rem % m_width;
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


template <>
class DynamicArray3D<bool> {
public:
    // std::vector<bool> 用の特殊なイテレータ型
    using iterator = std::vector<bool>::iterator;
    using const_iterator = std::vector<bool>::const_iterator;

    // vector<bool>特有のプロキシ参照型
    using reference = std::vector<bool>::reference;
    using const_reference = std::vector<bool>::const_reference;

    // コンストラクタ
    DynamicArray3D(size_t width, size_t height, size_t depth, bool initialValue = false)
        : m_width(width), m_height(height), m_depth(depth)
    {
        m_data.resize(width * height * depth, initialValue);
    }

    DynamicArray3D() : m_width(0), m_height(0), m_depth(0) {}

    // アクセッサ (読み書き用)
    // 戻り値は bool& ではなく、std::vector<bool>::reference という特殊な型
    // grid(x,y,z) = true; のように代入が可能
    reference operator()(size_t x, size_t y, size_t z) {
        assert(x < m_width && y < m_height && z < m_depth);
        return m_data[getIndex(x, y, z)];
    }

    // アクセッサ (読み取り専用)
    // 戻り値は bool
    bool operator()(size_t x, size_t y, size_t z) const {
        assert(x < m_width && y < m_height && z < m_depth);
        return m_data[getIndex(x, y, z)];
    }

    // 安全なアクセッサ
    reference at(size_t x, size_t y, size_t z) {
        if (x >= m_width || y >= m_height || z >= m_depth) {
            throw std::out_of_range("Grid coordinates out of range");
        }
        return m_data[getIndex(x, y, z)];
    }

    // サイズ変更
    void Resize(size_t newWidth, size_t newHeight, size_t newDepth, bool defaultValue = false) {
        if (newWidth == m_width && newHeight == m_height && newDepth == m_depth) {
            return;
        }

        Container newData(newWidth * newHeight * newDepth, defaultValue);

        const size_t copyWidth = std::min(m_width, newWidth);
        const size_t copyHeight = std::min(m_height, newHeight);
        const size_t copyDepth = std::min(m_depth, newDepth);

        for (size_t z = 0; z < copyDepth; ++z) {
            for (size_t y = 0; y < copyHeight; ++y) {
                size_t oldBaseIndex = (z * m_height + y) * m_width;
                size_t newBaseIndex = (z * newHeight + y) * newWidth;

                std::copy(
                    m_data.begin() + oldBaseIndex,
                    m_data.begin() + oldBaseIndex + copyWidth,
                    newData.begin() + newBaseIndex
                );
            }
        }

        m_data = std::move(newData);
        m_width = newWidth;
        m_height = newHeight;
        m_depth = newDepth;
    }

    size_t GetWidth() const { return m_width; }
    size_t GetHeight() const { return m_height; }
    size_t GetDepth() const { return m_depth; }

    // 容量をバイト単位で概算して返す（デバッグ用）
    size_t GetMemoryUsageBytes() const {
        return (m_data.capacity() + 7) / 8;
    }

    void Clear() {
        m_data.clear();
        m_width = m_height = m_depth = 0;
    }

    iterator begin() noexcept { return m_data.begin(); }
    const_iterator begin() const noexcept { return m_data.begin(); }
    const_iterator cbegin() const noexcept { return m_data.cbegin(); }

    iterator end() noexcept { return m_data.end(); }
    const_iterator end() const noexcept { return m_data.end(); }
    const_iterator cend() const noexcept { return m_data.cend(); }

    // 座標逆算ヘルパー
    void GetCoordinates(const_iterator it, size_t& outX, size_t& outY, size_t& outZ) const {
        size_t index = std::distance(m_data.cbegin(), it);
        size_t xyArea = m_width * m_height;
        outZ = index / xyArea;
        size_t rem = index % xyArea;
        outY = rem / m_width;
        outX = rem % m_width;
    }

private:
    inline size_t getIndex(size_t x, size_t y, size_t z) const {
        return x + (y * m_width) + (z * m_width * m_height);
    }

    std::vector<bool> m_data;
    size_t m_width = 0;
    size_t m_height = 0;
    size_t m_depth = 0;
};