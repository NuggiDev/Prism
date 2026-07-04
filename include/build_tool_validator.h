#pragma once

#include <string>
#include <vector>
#include <map>

struct BuildTool {
    std::string name;
    std::string checkCommand;
    std::string minVersion;
    std::map<std::string, std::string> installGuides;
};

class BuildToolValidator {
public:
    BuildToolValidator();
    bool isInstalled(const std::string& toolName);
    std::vector<std::string> validateTools(const std::vector<std::string>& tools);
    std::string getInstallGuide(const std::string& toolName);
    std::string detectOS();
    std::string getVersion(const std::string& toolName);
    
private:
    std::map<std::string, BuildTool> toolRegistry;
    void initializeToolRegistry();
    bool executeCheck(const std::string& command);
};
