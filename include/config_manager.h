#pragma once

#include <string>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConfigManager {
public:
    static void init();
    static json loadConfig();
    static void saveConfig(const json& config);
    static std::string getConfigPath();
    static json getProjectConfig(const std::string& projectPath);
    
private:
    static std::string getUserConfigDir();
};
