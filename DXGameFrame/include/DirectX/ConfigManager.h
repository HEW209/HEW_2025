/******************************************************************//**
 * @file   ConfigManager.h
 * @brief  ライブラリ設定を管理する
 * 
 * @author 石田怜
 * @date   2025/11/23
 *********************************************************************/
#pragma once

#include "ConfigKey.h"
#include <string>
#include <unordered_map>

/**
 * @brief ライブラリ設定を管理する
 */
class ConfigManager
{
public:
    /**
     * @brief 設定ファイルを読み込む
     * @param filePath 設定ファイル(.json)へのパス
     * @return 実行結果
     */
    bool Load(const std::string& filePath);

    /**
     * @brief 設定値をファイルに保存する
     * @param filePath 設定ファイル(.json)へのパス
     * @return 実行結果
     */
    bool Save(const std::string& filePath);

    /**
     * @brief bool型の設定値を読み込む
     * @param key 取得に使用するキー
     * @return 設定値
     */
    bool GetBool(const std::string& key);

    /**
     * @brief int型の設定値を読み込む
     * @param key 取得に使用するキー
     * @return 設定値
     */
    int GetInt(const std::string& key);

    /**
     * @brief float型の設定値を読み込む
     * @param key 取得に使用するキー
     * @return 設定値
     */
    float GetFloat(const std::string& key);

    /**
     * @brief string型の設定値を読み込む
     * @param key 取得に使用するキー
     * @return 設定値
     */
    const std::string& GetString(const std::string& key);

    /**
     * @brief bool型の設定値を登録する
     * @param key 設定に使用するキー
     * @param value 設定値
     */
    void SetBool(const std::string& key, bool value);

    /**
     * @brief int型の設定値を登録する
     * @param key 設定に使用するキー
     * @param value 設定値
     */
    void SetInt(const std::string& key, int value);

    /**
     * @brief float型の設定値を登録する
     * @param key 設定に使用するキー
     * @param value 設定値
     */
    void SetFloat(const std::string& key, float value);

    /**
     * @brief string型の設定値を登録する
     * @param key 設定に使用するキー
     * @param value 設定値
     */
    void SetString(const std::string& key, const std::string& value);

private:
    ConfigManager() = default;

    /// bool型の設定値
    std::unordered_map<std::string, bool> m_boolConfig;

    /// int型の設定値
    std::unordered_map<std::string, int> m_intConfig;

    /// float型の設定値
    std::unordered_map<std::string, float> m_floatConfig;

    /// string型の設定値
    std::unordered_map<std::string, std::string> m_stringConfig;

public:
    /**
     * @brief 唯一のインスタンスを取得する
     * @return ConfigManagerインスタンスへの参照
     */
    static ConfigManager& Instance();
};