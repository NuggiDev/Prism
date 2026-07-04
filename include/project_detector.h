#pragma once

#include <string>
#include <vector>
#include <map>

enum class ProjectType {
    CPP_CMAKE,
    CSHARP_DOTNET,
    RUST_CARGO,
    C_MAKE,
    NODEJS_NPM,
    PYTHON_PIP,
    JAVA_MAVEN,
    UNKNOWN
};

struct ProjectInfo {
    ProjectType type;
    std::string buildTool;
    std::string buildCommand;
    std::string language;
    std::string entryPoint;
};

class ProjectDetector {
public:
    ProjectInfo detect(const std::string& projectPath);
    std::string projectTypeToString(ProjectType type);
    
private:
    ProjectType identifyType(const std::string& projectPath);
    ProjectInfo createProjectInfo(ProjectType type);
    bool fileExists(const std::string& path);
};
