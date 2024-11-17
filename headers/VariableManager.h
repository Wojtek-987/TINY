#ifndef VARIABLEMANAGER_H
#define VARIABLEMANAGER_H

#include <string>
#include <unordered_map>

enum class VariableType {
    Variable,
    Constant
};

struct Variable {
    VariableType type;
    long long value;
};

class VariableManager {
public:
    // Add a variable or constant
    bool add(const std::string& name, VariableType type, long long value);

    // Rename a variable or constant
    bool rename(const std::string& old_name, const std::string& new_name);

    // Delete a variable or constant
    bool remove(const std::string& name);

    // Clear all variables and constants
    void clear();

    // Set the value of a variable
    bool set(const std::string& name, long long value);

    // Check if a variable or constant exists
    [[nodiscard]] bool exists(const std::string& name) const;

    // Get the value of a variable or constant
    [[nodiscard]] bool getValue(const std::string& name, long long& value) const;

    // Get the type of a variable or constant
    [[nodiscard]] bool getType(const std::string& name, VariableType& type) const;

private:
    std::unordered_map<std::string, Variable> variables;
};

#endif // VARIABLEMANAGER_H
