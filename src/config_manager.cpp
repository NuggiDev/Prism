#include "config_manager.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

std::string ConfigManager::getUserConfigDir() {
    #ifdef _WIN32
        const char* appdata = getenv("APPDATA");
        if (appdata) {
            return std::string(appdata) + "\\Prism";
        }
        return ".prism";
    #else
        const char* home = getenv("HOME");
        if (home) {
            return std::string(home) + "/.prism";
        }
        return ".prism";
    #endif
}

void ConfigManager::init() {
    std::string configDir = getUserConfigDir();
    fs::create_directories(configDir);
}

std::string ConfigManager::getConfigPath() {
    return getUserConfigDir() + "/config.json";
}

json ConfigManager::loadConfig() {
    std::string path = getConfigPath();
    
    if (!fs::exists(path)) {
        return json::object();
    }
    
    std::ifstream file(path);
    if (!file.is_open()) {
        return json::object();
    }
    
    json config;
    try {
        file >> config;
    } catch (...) {
        return json::object();
    }
    
    return config;
}

void ConfigManager::saveConfig(const json& config) {
    std::string path = getConfigPath();
    init();
    
    std::ofstream file(path);
    if (file.is_open()) {
        file << config.dump(2);
    }
}

json ConfigManager::getProjectConfig(const std::string& projectPath) {
    if (!fs::exists(projectPath + "/prism.json")) {
        return json::object();
    }
    
    std::ifstream file(projectPath + "/prism.json");
    if (!file.is_open()) {
        return json::object();
    }
    
    json config;
    try {
        file >> config;
    } catch (...) {
        return json::object();
    }
    
    return config;
}
