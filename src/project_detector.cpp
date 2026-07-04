#include "project_detector.h"
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

ProjectInfo ProjectDetector::detect(const std::string& projectPath) {
    ProjectType type = identifyType(projectPath);
    return createProjectInfo(type);
}

ProjectType ProjectDetector::identifyType(const std::string& projectPath) {
    std::map<std::string, ProjectType> markers = {
        {"CMakeLists.txt", ProjectType::CPP_CMAKE},
        {".csproj", ProjectType::CSHARP_DOTNET},
        {".sln", ProjectType::CSHARP_DOTNET},
        {"Cargo.toml", ProjectType::RUST_CARGO},
        {"Makefile", ProjectType::C_MAKE},
        {"package.json", ProjectType::NODEJS_NPM},
        {"setup.py", ProjectType::PYTHON_PIP},
        {"pyproject.toml", ProjectType::PYTHON_PIP},
        {"pom.xml", ProjectType::JAVA_MAVEN},
        {"build.gradle", ProjectType::JAVA_MAVEN}
    };
    
    for (const auto& [marker, type] : markers) {
        fs::path markerPath = fs::path(projectPath) / marker;
        if (fileExists(markerPath.string())) {
            return type;
        }
    }
    
    return ProjectType::UNKNOWN;
}

bool ProjectDetector::fileExists(const std::string& path) {
    return fs::exists(fs::path(path));
}

ProjectInfo ProjectDetector::createProjectInfo(ProjectType type) {
    ProjectInfo info;
    info.type = type;
    
    switch (type) {
        case ProjectType::CPP_CMAKE:
            info.buildTool = "cmake";
            info.language = "C++";
            #ifdef _WIN32
                info.buildCommand = "mkdir -Force build 2>$null; cd build; cmake ..; cmake --build . --config Release";
            #else
                info.buildCommand = "mkdir -p build && cd build && cmake .. && make";
            #endif
            break;
        case ProjectType::CSHARP_DOTNET:
            info.buildTool = "dotnet";
            info.language = "C#";
            info.buildCommand = "dotnet build -c Release";
            break;
        case ProjectType::RUST_CARGO:
            info.buildTool = "cargo";
            info.language = "Rust";
            info.buildCommand = "cargo build --release";
            break;
        case ProjectType::C_MAKE:
            info.buildTool = "make";
            info.language = "C";
            info.buildCommand = "make";
            break;
        case ProjectType::NODEJS_NPM:
            info.buildTool = "npm";
            info.language = "JavaScript";
            info.buildCommand = "npm run build";
            break;
        case ProjectType::PYTHON_PIP:
            info.buildTool = "pip";
            info.language = "Python";
            info.buildCommand = "pip install -e .";
            break;
        case ProjectType::JAVA_MAVEN:
            info.buildTool = "mvn";
            info.language = "Java";
            info.buildCommand = "mvn clean package";
            break;
        case ProjectType::UNKNOWN:
            info.buildTool = "unknown";
            info.language = "unknown";
            break;
    }
    
    return info;
}

std::string ProjectDetector::projectTypeToString(ProjectType type) {
    switch (type) {
        case ProjectType::CPP_CMAKE:
            return "C++ (CMake)";
        case ProjectType::CSHARP_DOTNET:
            return "C# (.NET)";
        case ProjectType::RUST_CARGO:
            return "Rust (Cargo)";
        case ProjectType::C_MAKE:
            return "C (Make)";
        case ProjectType::NODEJS_NPM:
            return "JavaScript (npm)";
        case ProjectType::PYTHON_PIP:
            return "Python (pip)";
        case ProjectType::JAVA_MAVEN:
            return "Java (Maven)";
        case ProjectType::UNKNOWN:
            return "Unknown";
        default:
            return "Unknown";
    }
}
