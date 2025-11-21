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


std::unordered_map<uint64_t, InputAction> InputSystem::m_actions;
std::unordered_map<uint64_t, bool> InputSystem::m_prevButtonStates;

void InputSystem::CreateButtonAction(std::string_view name)
{
	CreateButtonAction(StringHash(name));
}

void InputSystem::CreateButtonAction(uint64_t nameHash)
{
    if (m_actions.find(nameHash) == m_actions.end())
    {
        m_actions.emplace(nameHash, InputAction(ActionType::Button));
        m_prevButtonStates[nameHash] = false;
    }
}

void InputSystem::CreateAxisAction(std::string_view name)
{
	CreateAxisAction(StringHash(name));
}

void InputSystem::CreateAxisAction(uint64_t nameHash)
{
    if (m_actions.find(nameHash) == m_actions.end())
    {
        m_actions.emplace(nameHash, InputAction(ActionType::Axis));
    }
}

void InputSystem::RemoveAction(std::string_view name)
{
	RemoveAction(StringHash(name));
}

void InputSystem::RemoveAction(uint64_t nameHash)
{
    m_actions.erase(nameHash);
    m_prevButtonStates.erase(nameHash);
}

void InputSystem::Clear()
{
    m_actions.clear();
    m_prevButtonStates.clear();
}

void InputSystem::BindKey(std::string_view actionName, KeyCode key)
{
	BindKey(StringHash(actionName), key);
}

void InputSystem::BindKey(uint64_t actionNameHash, KeyCode key)
{
    if (auto* action = FindAction(actionNameHash))
    {
        action->AddBinding(std::make_unique<KeyBinding>(key));
    }
}

void InputSystem::BindPadButton(std::string_view actionName, PadCode button)
{
	BindPadButton(StringHash(actionName), button);
}

void InputSystem::BindPadButton(uint64_t actionNameHash, PadCode button)
{
    if (auto* action = FindAction(actionNameHash))
    {
        action->AddBinding(std::make_unique<PadButtonBinding>(button));
    }
}

void InputSystem::BindPadStick(std::string_view actionName, StickCode stick, std::optional<float> deadzone)
{
	BindPadStick(StringHash(actionName), stick, deadzone);
}

void InputSystem::BindPadStick(uint64_t actionNameHash, StickCode stick, std::optional<float> deadzone)
{
    if (auto* action = FindAction(actionNameHash))
    {
        action->AddBinding(std::make_unique<PadStickBinding>(stick, deadzone));
    }
}

void InputSystem::BindVectorKey(std::string_view actionName, KeyCode key, const Vector2& value)
{
	BindVectorKey(StringHash(actionName), key, value);
}

void InputSystem::BindVectorKey(uint64_t actionNameHash, KeyCode key, const Vector2& value)
{
    if (auto* action = FindAction(actionNameHash))
    {
        action->AddBinding(std::make_unique<VectorKeyBinding>(key, value));
    }
}

void InputSystem::BindVectorKeys(std::string_view actionName, KeyCode up, KeyCode down, KeyCode left, KeyCode right)
{
	BindVectorKeys(StringHash(actionName), up, down, left, right);
}

void InputSystem::BindVectorKeys(uint64_t actionNameHash, KeyCode up, KeyCode down, KeyCode left, KeyCode right)
{
    if (auto* action = FindAction(actionNameHash))
    {
        action->AddBinding(std::make_unique<DirectionalKeyBinding>(up, down, left, right));
    }
}

bool InputSystem::GetButtonHold(std::string_view name)
{
	return GetButtonHold(StringHash(name));
}

bool InputSystem::GetButtonHold(uint64_t nameHash)
{
    if (const auto* action = FindAction(nameHash))
    {
        return action->GetBool();
    }
    return false;
}

bool InputSystem::GetButtonDown(std::string_view name)
{
	return GetButtonDown(StringHash(name));
}

bool InputSystem::GetButtonDown(uint64_t nameHash)
{
    bool current = GetButtonHold(nameHash);

    auto it = m_prevButtonStates.find(nameHash);
    bool prev = (it != m_prevButtonStates.end()) ? it->second : false;

    return current && !prev;
}

bool InputSystem::GetButtonUp(std::string_view name)
{
	return GetButtonUp(StringHash(name));
}

bool InputSystem::GetButtonUp(uint64_t nameHash)
{
    bool current = GetButtonHold(nameHash);

    auto it = m_prevButtonStates.find(nameHash);
    bool prev = (it != m_prevButtonStates.end()) ? it->second : false;

    return !current && prev;
}

Vector2 InputSystem::GetAxis(std::string_view name)
{
	return GetAxis(StringHash(name));
}

Vector2 InputSystem::GetAxis(uint64_t nameHash)
{
    if (const auto* action = FindAction(nameHash))
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

InputAction* InputSystem::FindAction(uint64_t nameHash)
{
    auto it = m_actions.find(nameHash);
    if (it != m_actions.end())
    {
        return &it->second;
    }
    return nullptr;
}