#pragma once

#include <DXGameFrame.h>

#include "HashUtil.h"


/**
 * @brief アクションの値の型
 */
enum class ActionType
{
    Button, // bool
    Axis    // Vector2
};

/**
 * @brief 物理入力の基底インターフェース
 */
struct IInputBinding
{
    virtual ~IInputBinding() = default;
    virtual bool GetBoolValue() const { return false; }
    virtual Vector2 GetVector2Value() const { return Vector2::zero; }
};

/**
 * @brief 入力アクションクラス
 */
class InputAction
{
public:
    InputAction(ActionType type)
        : m_type(type) {
    }

    // バインディングを追加
    void AddBinding(std::unique_ptr<IInputBinding> binding)
    {
        m_bindings.push_back(std::move(binding));
    }

    // 現在のbool値を取得
    bool GetBool() const;

    // 現在のVector2値を取得
    Vector2 GetVector2() const;

    ActionType GetType() const { return m_type; }

private:
    ActionType m_type;
    std::vector<std::unique_ptr<IInputBinding>> m_bindings;
};

/**
 * @brief 入力システム管理クラス
 */
class InputSystem
{
public:
    InputSystem() = default;
    ~InputSystem() = default;

    // --- アクション作成 ---

    /**
     * @brief bool型のアクションを作成
     * @param name アクション名
     */
    static void CreateButtonAction(std::string_view name);

    /**
     * @brief bool型のアクションを作成
	 * @param nameHash アクション名のハッシュ値
     */
    static void CreateButtonAction(uint64_t nameHash);

    /**
     * @brief Vector2型のアクションを作成
     * @param name アクション名
     */
    static void CreateAxisAction(std::string_view name);

    /**
     * @brief Vector2型のアクションを作成
	 * @param nameHash アクション名のハッシュ値
     */
    static void CreateAxisAction(uint64_t nameHash);

    // --- アクション削除 ---

    /**
     * @brief 指定した名前のアクションを削除する
     * @param name 削除したいアクション名
     */
    static void RemoveAction(std::string_view name);

    /**
     * @brief 指定した名前のアクションを削除する
	 * @param nameHash 削除したいアクション名のハッシュ値
     */
    static void RemoveAction(uint64_t nameHash);

    /**
     * @brief 全てのアクションを削除する
     * シーン遷移時などにリセットする場合に使用
     */
    static void Clear();

    // --- バインディング設定 ---

    // キーをアクションに割り当て
    static void BindKey(std::string_view actionName, KeyCode key);
    static void BindKey(uint64_t actionNameHash, KeyCode key);

    // コントローラーボタンをアクションに割り当て
    static void BindPadButton(std::string_view actionName, PadCode button);
    static void BindPadButton(uint64_t actionNameHash, PadCode button);

    // コントローラーのスティックをアクションに割り当て (Axisアクション用)
    static void BindPadStick(std::string_view actionName, StickCode stick, std::optional<float> deadzone = std::nullopt);
    static void BindPadStick(uint64_t actionNameHash, StickCode stick, std::optional<float> deadzone = std::nullopt);

    // キーをVector2アクションに指定した値で割り当て
    static void BindVectorKey(std::string_view actionName, KeyCode key, const Vector2& value);
    static void BindVectorKey(uint64_t actionNameHash, KeyCode key, const Vector2& value);

    // 4つのキーをVector2アクションに割り当て
    static void BindVectorKeys(std::string_view actionName, KeyCode up, KeyCode down, KeyCode left, KeyCode right);
    static void BindVectorKeys(uint64_t actionNameHash, KeyCode up, KeyCode down, KeyCode left, KeyCode right);

    // --- 入力取得 ---

    // ボタンアクションの状態を取得
    static bool GetButtonHold(std::string_view name);
    static bool GetButtonHold(uint64_t nameHash);

    // ボタンアクションが押された瞬間を取得
    static bool GetButtonDown(std::string_view name);
    static bool GetButtonDown(uint64_t nameHash);

    // ボタンアクションが離された瞬間を取得
    static bool GetButtonUp(std::string_view name);
    static bool GetButtonUp(uint64_t nameHash);

    // Axisアクションの値を取得
    static Vector2 GetAxis(std::string_view name);
    static Vector2 GetAxis(uint64_t nameHash);

    // 毎フレーム更新
    static void Update();

private:
    static InputAction* FindAction(uint64_t nameHash);

private:
    static std::unordered_map<uint64_t, InputAction> m_actions;

    // トリガー判定用、前フレームのbool状態を保持
    static std::unordered_map<uint64_t, bool> m_prevButtonStates;
};


// 具体的なバインディング実装クラス

// キー単体 -> bool
struct KeyBinding : public IInputBinding
{
    KeyCode key;
    KeyBinding(KeyCode k) : key(k) {}
    bool GetBoolValue() const override { return Input::GetKeyHold(key); }
};

// パッドボタン -> bool
struct PadButtonBinding : public IInputBinding
{
    PadCode button;
    PadButtonBinding(PadCode b) : button(b) {}
    bool GetBoolValue() const override { return Input::GetButtonHold(button); }
};

// パッドスティック -> Vector2
struct PadStickBinding : public IInputBinding
{
    StickCode stick;
    std::optional<float> deadzone;
    PadStickBinding(StickCode s, std::optional<float> dz) : stick(s), deadzone(dz) {}
    Vector2 GetVector2Value() const override { return Input::GetStick(stick, deadzone); }
};

// キー単体 -> Vector2
struct VectorKeyBinding : public IInputBinding
{
    KeyCode key;
    Vector2 targetValue;

    VectorKeyBinding(KeyCode k, const Vector2& v)
        : key(k), targetValue(v) {
    }

    Vector2 GetVector2Value() const override
    {
        return Input::GetKeyHold(key) ? targetValue : Vector2::zero;
    }
};

// 4キー -> Vector2
struct DirectionalKeyBinding : public IInputBinding
{
    KeyCode up, down, left, right;
    DirectionalKeyBinding(KeyCode u, KeyCode d, KeyCode l, KeyCode r)
        : up(u), down(d), left(l), right(r) {
    }

    Vector2 GetVector2Value() const override
    {
        Vector2 vec = Vector2::zero;
        if (Input::GetKeyHold(up))    vec.y += 1.0f;
        if (Input::GetKeyHold(down))  vec.y -= 1.0f;
        if (Input::GetKeyHold(right)) vec.x += 1.0f;
        if (Input::GetKeyHold(left))  vec.x -= 1.0f;
        return vec;
    }
};