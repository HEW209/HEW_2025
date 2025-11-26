// ConfigManager.cpp
#include <DirectX/ConfigManager.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool ConfigManager::Load(const std::string& filePath)
{
    // 設定ファイルを開く
    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    // JSONの読み込み
    json j;
    file >> j;

    // 型ごとに格納
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        const std::string& key = it.key();
        const json& value = it.value();

        if (value.is_boolean())
        {
            m_boolConfig[key] = value.get<bool>();
        }
        else if (value.is_number_integer())
        {
            m_intConfig[key] = value.get<int>();
        }
        else if (value.is_number_float())
        {
            m_floatConfig[key] = value.get<float>();
        }
        else if (value.is_string())
        {
            m_stringConfig[key] = value.get<std::string>();
        }
    }

    return true;
}

bool ConfigManager::Save(const std::string& filePath)
{
    // 設定ファイルを開く
    std::ofstream file(filePath);
    if (!file.is_open())
        return false;

    json j;

    // 設定値の書き込み
    for (auto& [key, value] : m_boolConfig)
    {
        j[key] = value;
    }
    for (auto& [key, value] : m_intConfig)
    {
        j[key] = value;
    }
    for (auto& [key, value] : m_floatConfig)
    {
        j[key] = value;
    }
    for (auto& [key, value] : m_stringConfig)
    {
        j[key] = value;
    }

    // 設定項目が無ければ終了
    if (j.size() == 0)
        return false;

    // 設定ファイルに書き込み
    file << j.dump(4);
    return true;
}

bool ConfigManager::GetBool(const std::string& key)
{
    auto it = m_boolConfig.find(key);
    if (it == m_boolConfig.end())
        return false;

    return it->second;
}

int ConfigManager::GetInt(const std::string& key)
{
    auto it = m_intConfig.find(key);
    if (it == m_intConfig.end())
        return 0;

    return it->second;
}

float ConfigManager::GetFloat(const std::string& key)
{
    auto it = m_floatConfig.find(key);
    if (it == m_floatConfig.end())
        return 0.0f;

    return it->second;
}

const std::string& ConfigManager::GetString(const std::string& key)
{
    static std::string errorStr = "設定値読み込みエラー";
    auto it = m_stringConfig.find(key);
    if (it == m_stringConfig.end())
        return errorStr;

    return it->second;
}

void ConfigManager::SetBool(const std::string& key, bool value)
{
    m_boolConfig[key] = value;
}

void ConfigManager::SetInt(const std::string& key, int value)
{
    m_intConfig[key] = value;
}

void ConfigManager::SetFloat(const std::string& key, float value)
{
    m_floatConfig[key] = value;
}

void ConfigManager::SetString(const std::string& key, const std::string& value)
{
    m_stringConfig[key] = value;
}

ConfigManager& ConfigManager::Instance()
{
    static ConfigManager s_instance;
    return s_instance;
}