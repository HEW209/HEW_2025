#include "InputSystem.h"


bool InputAction::GetBool() const
{
    for (const auto& binding : m_bindings)
    {
        if (binding->GetBoolValue())
        {
            return true;
        }
    }
    return false;
}

Vector2 InputAction::GetVector2() const
{
    Vector2 result = Vector2::zero;

    for (const auto& binding : m_bindings)
    {
        result += binding->GetVector2Value();
    }

    result.x = std::clamp(result.x, -1.0f, 1.0f);
    result.y = std::clamp(result.y, -1.0f, 1.0f);

    if (result.Magnitude() > 1.0f)
    {
        result = result.Normalized();
    }

    return result;
}


std::unordered_map<std::string, InputAction> InputSystem::m_actions;
std::unordered_map<std::string, bool> InputSystem::m_prevButtonStates;

void InputSystem::CreateButtonAction(const std::string& name)
{
    if (m_actions.find(name) == m_actions.end())
    {
        m_actions.emplace(name, InputAction(name, ActionType::Button));
        m_prevButtonStates[name] = false;
    }
}

void InputSystem::CreateAxisAction(const std::string& name)
{
    if (m_actions.find(name) == m_actions.end())
    {
        m_actions.emplace(name, InputAction(name, ActionType::Axis));
    }
}

void InputSystem::RemoveAction(const std::string& name)
{
    m_actions.erase(name);
    m_prevButtonStates.erase(name);
}

void InputSystem::Clear()
{
    m_actions.clear();
    m_prevButtonStates.clear();
}

void InputSystem::BindKey(const std::string& actionName, KeyCode key)
{
    if (auto* action = FindAction(actionName))
    {
        action->AddBinding(std::make_unique<KeyBinding>(key));
    }
}

void InputSystem::BindPadButton(const std::string& actionName, PadCode button)
{
    if (auto* action = FindAction(actionName))
    {
        action->AddBinding(std::make_unique<PadButtonBinding>(button));
    }
}

void InputSystem::BindPadStick(const std::string& actionName, StickCode stick, std::optional<float> deadzone)
{
    if (auto* action = FindAction(actionName))
    {
        action->AddBinding(std::make_unique<PadStickBinding>(stick, deadzone));
    }
}

void InputSystem::BindVectorKey(const std::string& actionName, KeyCode key, const Vector2& value)
{
    if (auto* action = FindAction(actionName))
    {
        action->AddBinding(std::make_unique<VectorKeyBinding>(key, value));
    }
}

void InputSystem::BindVectorKeys(const std::string& actionName, KeyCode up, KeyCode down, KeyCode left, KeyCode right)
{
    if (auto* action = FindAction(actionName))
    {
        action->AddBinding(std::make_unique<DirectionalKeyBinding>(up, down, left, right));
    }
}

bool InputSystem::GetButton(const std::string& name)
{
    if (const auto* action = FindAction(name))
    {
        return action->GetBool();
    }
    return false;
}

bool InputSystem::GetButtonDown(const std::string& name)
{
    bool current = GetButton(name);

    auto it = m_prevButtonStates.find(name);
    bool prev = (it != m_prevButtonStates.end()) ? it->second : false;

    return current && !prev;
}

Vector2 InputSystem::GetAxis(const std::string& name)
{
    if (const auto* action = FindAction(name))
    {
        return action->GetVector2();
    }
    return Vector2::zero;
}

void InputSystem::Update()
{
    for (auto& [name, action] : m_actions)
    {
        if (action.GetType() == ActionType::Button)
        {
            m_prevButtonStates[name] = action.GetBool();
        }
    }
}

InputAction* InputSystem::FindAction(const std::string& name)
{
    auto it = m_actions.find(name);
    if (it != m_actions.end())
    {
        return &it->second;
    }
    return nullptr;
}