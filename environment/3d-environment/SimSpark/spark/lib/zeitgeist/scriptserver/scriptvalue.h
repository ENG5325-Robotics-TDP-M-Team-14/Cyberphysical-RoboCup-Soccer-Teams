#ifndef ZEITGEIST_SCRIPTVALUE_H
#define ZEITGEIST_SCRIPTVALUE_H

#include <string>
#include <variant>
#include <zeitgeist/zeitgeist_defines.h>

namespace zeitgeist
{

/**
 * A wrapper class for a decoupled variable returned by a scripting language
 * that can be safely accessed regardless of the caller's thread.
 */
class ZEITGEIST_API ScriptValue
{
public:
    /* constructs an empty value */
    ScriptValue();

    /* constructs a value holding a bool */
    ScriptValue(bool value);

    /* constructs a value holding a float */
    ScriptValue(float value);

    /* constructs a value holding an int */
    ScriptValue(int value);

    /* constructs a value holding a string */
    ScriptValue(std::string s);

    /**
     * tries to convert the stored value to a bool value and writes it to target
     * @return true on success
     */
    bool GetBool(bool& target);

    /**
     * tries to convert the stored value to a float value and writes it to target
     * @return true on success
     */
    bool GetFloat(float& target);

    /**
     * tries to convert the stored value to an int value and writes it to target
     * @return true on success
     */
    bool GetInt(int& target);

    /**
     * tries to convert the stored value to a string value and writes it to target
     * @return true on success
     */
    bool GetString(std::string& target);

    /**
     * checks if the object contains a value
     * @return true if the object contains a value
     */
    bool IsNil();

private:
    /* the stored value */
    std::variant<std::monostate, bool, float, int, std::string> mValue;
};
}

#endif // ZEITGEIST_SCRIPTVALUE_H
