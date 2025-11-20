#pragma once

#include <cstdint>
#include <string_view>

constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
constexpr uint64_t FNV_PRIME = 1099511628211ULL;

constexpr uint64_t StringHash(std::string_view str)
{
    uint64_t hash = FNV_OFFSET_BASIS;
    for (char c : str)
    {
        hash ^= static_cast<uint64_t>(c);
        hash *= FNV_PRIME;
    }
    return hash;
}

// 文字列をハッシュ値に変換するユーザー定義リテラル
consteval uint64_t operator""_hash(const char* str, size_t len)
{
    return StringHash(std::string_view(str, len));
}