#include "build_tool_validator.h"
#include <cstdlib>
#include <memory>
#include <array>

#ifdef _WIN32
    #define DEVNULL "nul"
    #define PATH_SEP "\\"
#else
    #define DEVNULL "/dev/null"
    #define PATH_SEP "/"
#endif

BuildToolValidator::BuildToolValidator() {
    initializeToolRegistry();
}

void BuildToolValidator::initializeToolRegistry() {
    toolRegistry["cmake"] = {
        "cmake",
        "cmake --version",
        "3.10",
        {
            {"windows", "choco install cmake"},
            {"macos", "brew install cmake"},
            {"linux", "sudo apt install cmake"}
        }
    };
    
    toolRegistry["dotnet"] = {
        "dotnet",
        "dotnet --version",
        "6.0",
        {
            {"windows", "choco install dotnet-sdk"},
            {"macos", "brew install dotnet"},
            {"linux", "sudo apt install dotnet-sdk-8.0"}
        }
    };
    
    toolRegistry["cargo"] = {
        "cargo",
        "cargo --version",
        "1.0",
        {
            {"windows", "choco install rustup.install"},
            {"macos", "brew install rust"},
            {"linux", "curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh"}
        }
    };
    
    toolRegistry["make"] = {
        "make",
        "make --version",
        "4.0",
        {
            {"windows", "choco install make"},
            {"macos", "brew install make"},
            {"linux", "sudo apt install build-essential"}
        }
    };
    
    toolRegistry["npm"] = {
        "npm",
        "npm --version",
        "6.0",
        {
            {"windows", "choco install nodejs"},
            {"macos", "brew install node"},
            {"linux", "sudo apt install nodejs npm"}
        }
    };
    
    toolRegistry["pip"] = {
        "pip",
        "pip --version",
        "20.0",
        {
            {"windows", "choco install python"},
            {"macos", "brew install python"},
            {"linux", "sudo apt install python3-pip"}
        }
    };
    
    toolRegistry["mvn"] = {
        "mvn",
        "mvn --version",
        "3.6",
        {
            {"windows", "choco install maven"},
            {"macos", "brew install maven"},
            {"linux", "sudo apt install maven"}
        }
    };
}

bool BuildToolValidator::isInstalled(const std::string& toolName) {
    if (toolRegistry.find(toolName) == toolRegistry.end()) {
        return false;
    }
    
    #ifdef _WIN32
        std::string cmd = "where " + toolName + " >nul 2>&1";
    #else
        std::string cmd = "which " + toolName + " >/dev/null 2>&1";
    #endif
    
    return system(cmd.c_str()) == 0;
}

std::vector<std::string> BuildToolValidator::validateTools(const std::vector<std::string>& tools) {
    std::vector<std::string> missing;
    
    for (const auto& tool : tools) {
        if (!isInstalled(tool)) {
            missing.push_back(tool);
        }
    }
    
    return missing;
}

std::string BuildToolValidator::getInstallGuide(const std::string& toolName) {
    if (toolRegistry.find(toolName) == toolRegistry.end()) {
        return "Tool not found in registry";
    }
    
    std::string os = detectOS();
    const auto& guides = toolRegistry[toolName].installGuides;
    
    if (guides.find(os) != guides.end()) {
        return guides.at(os);
    }
    
    return "No installation guide available for your OS";
}

std::string BuildToolValidator::getVersion(const std::string& toolName) {
    if (toolRegistry.find(toolName) == toolRegistry.end()) {
        return "unknown";
    }
    
    std::string cmd = toolRegistry[toolName].checkCommand + " 2>&1";
    std::array<char, 128> buffer;
    std::string result;
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), &pclose);
    if (!pipe) return "unknown";
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result.substr(0, result.find('\n'));
}

std::string BuildToolValidator::detectOS() {
    #ifdef _WIN32
        return "windows";
    #elif __APPLE__
        return "macos";
    #else
        return "linux";
    #endif
}

bool BuildToolValidator::executeCheck(const std::string& command) {
    std::string fullCmd = command + " > " + DEVNULL + " 2>&1";
    return system(fullCmd.c_str()) == 0;
}
