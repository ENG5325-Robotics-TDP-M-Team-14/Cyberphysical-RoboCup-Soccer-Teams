#include "scriptvalue.h"

using namespace zeitgeist;

ScriptValue::ScriptValue()
{
}

ScriptValue::ScriptValue(bool value) : mValue(value)
{
}

ScriptValue::ScriptValue(float value) : mValue(value)
{
}

ScriptValue::ScriptValue(int value) : mValue(value)
{
}

ScriptValue::ScriptValue(std::string value) : mValue(value)
{
}

bool ScriptValue::GetBool(bool& target)
{
    if (std::holds_alternative<bool>(mValue))
    {
        target = std::get<bool>(mValue);
        return true;
    }
    return false;
}

bool ScriptValue::GetFloat(float& target)
{

    if (std::holds_alternative<float>(mValue))
    {
        target = std::get<float>(mValue);
        return true;
    }
    else if (std::holds_alternative<int>(mValue))
    {
        target = std::get<int>(mValue);
        return true;
    }
    return false;
}

bool ScriptValue::GetInt(int& target)
{
    if (std::holds_alternative<int>(mValue))
    {
        target = std::get<int>(mValue);
        return true;
    }
    else if (std::holds_alternative<float>(mValue))
    {
        target = std::get<float>(mValue);
        return true;
    }
    return false;
}

bool ScriptValue::GetString(std::string& target)
{
    if (std::holds_alternative<std::string>(mValue))
    {
        target = std::get<std::string>(mValue);
        return true;
    }
    else if (std::holds_alternative<int>(mValue))
    {
        target = std::to_string(std::get<int>(mValue));
        return true;
    }
    else if (std::holds_alternative<float>(mValue))
    {
        target = std::to_string(std::get<float>(mValue));
        return true;
    }
    return false;
}

bool ScriptValue::IsNil()
{
    return std::holds_alternative<std::monostate>(mValue);
}
