#include "headers/VariableManager.h"

bool VariableManager::add(const std::string& name, const VariableType type, const long long value) {
    if (variables.contains(name)) {
        return false; // Variable or constant already exists
    }
    variables[name] = {type, value};
    return true;
}

bool VariableManager::rename(const std::string& old_name, const std::string& new_name) {
    if (!variables.contains(old_name) || variables.contains(new_name)) {
        return false; // Old name doesn't exist or new name already exists
    }
    variables[new_name] = variables[old_name];
    variables.erase(old_name);
    return true;
}

bool VariableManager::remove(const std::string& name) {
    return variables.erase(name) > 0;
}

void VariableManager::clear() {
    variables.clear();
}

bool VariableManager::set(const std::string& name, const long long value) {
    if (!variables.contains(name)) {
        return false; // Variable doesn't exist
    }
    auto& variable = variables[name];
    if (variable.type == VariableType::Constant) {
        return false; // Cannot modify a constant
    }
    variable.value = value;
    return true;
}

bool VariableManager::exists(const std::string& name) const {
    return variables.contains(name);
}

bool VariableManager::getValue(const std::string& name, long long& value) const {
    if (!variables.contains(name)) {
        value = 0; // Assign a default value for non-existent variables
        return false;
    }
    value = variables.at(name).value;
    return true;
}

bool VariableManager::getType(const std::string& name, VariableType& type) const {
    const auto it = variables.find(name);
    if (it == variables.end()) {
        // Variable does not exist
        return false;
    }
    type = it->second.type;
    return true;
}
