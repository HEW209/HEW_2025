#pragma once

#include <cassert>
#include <concepts>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <limits>

template <std::unsigned_integral T = uint32_t>
class IDGenerator final
{
public:
    using IdType = T;

    // 無効値
    static constexpr IdType INVALID_ID = 0;

    IDGenerator() = default;
    ~IDGenerator() = default;

    // コピー禁止
    IDGenerator(const IDGenerator&) = delete;
    IDGenerator& operator=(const IDGenerator&) = delete;

    // ムーブは許可
    IDGenerator(IDGenerator&&) = default;
    IDGenerator& operator=(IDGenerator&&) = default;

    // IDを生成
    [[nodiscard]] IdType Generate()
    {
        if (!m_freeIds.empty())
        {
            std::pop_heap(m_freeIds.begin(), m_freeIds.end(), std::greater<IdType>{});

            IdType id = m_freeIds.back();
            m_freeIds.pop_back();
            return id;
        }

        // オーバーフロー対策
        if (m_placeholder == std::numeric_limits<IdType>::max()) [[unlikely]]
        {
            assert(false && "IDGenerator: IDが枯渇しました");
            return INVALID_ID;
        }

        return ++m_placeholder;
    }

    // IDを解放
    void Release(IdType id)
    {
        if (id == INVALID_ID || id > m_placeholder) {
            return;
        }

        if (id == m_placeholder)
        {
            --m_placeholder;
            return;
        }

        auto it = std::find(m_freeIds.begin(), m_freeIds.end(), id);
        if (it != m_freeIds.end()) {
            return;
        }

        m_freeIds.push_back(id);
        std::push_heap(m_freeIds.begin(), m_freeIds.end(), std::greater<IdType>{});
    }

    // すべてのIDをリセット
    void Reset()
    {
        m_placeholder = 0;
        m_freeIds.clear();
        m_freeIds.shrink_to_fit();
    }

    // 現在の発行済みID数を取得
    [[nodiscard]] size_t GetActiveCount() const {
        return static_cast<size_t>(m_placeholder) - m_freeIds.size();
    }

private:
    // これまでに発行された最大ID
    IdType m_placeholder = 0;

    // 再利用可能なID
    std::vector<IdType> m_freeIds;
};