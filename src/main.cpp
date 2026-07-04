#include "project_detector.h"
#include "build_tool_validator.h"
#include "build_executor.h"
#include "logger.h"
#include "config_manager.h"
#include <iostream>
#include <vector>
#include <string>
#include <fmt/core.h>
#include <chrono>

void printBanner() {
    fmt::print("\n  ╔════════════════════════════════════════╗\n");
    fmt::print("  ║    Prism - Universal Build System    ║\n");
    fmt::print("  ║           v1.0.0                     ║\n");
    fmt::print("  ╚════════════════════════════════════════╝\n\n");
}

void printHelp() {
    fmt::print("  Usage:\n");
    fmt::print("    prism build [path]      Detect and build project\n");
    fmt::print("    prism rebuild [path]    Clean and rebuild\n");
    fmt::print("    prism clean [path]      Remove build artifacts\n");
    fmt::print("    prism detect [path]     Show detected project type\n");
    fmt::print("    prism validate [path]   Check required build tools\n");
    fmt::print("    prism info [path]       Show project information\n");
    fmt::print("    prism help              Show this help\n\n");
}

void cmdBuild(const std::string& projectPath) {
    auto logger = Logger::get();
    
    ProjectDetector detector;
    ProjectInfo info = detector.detect(projectPath);
    
    if (info.type == ProjectType::UNKNOWN) {
        logger->error("Could not detect project type");
        return;
    }
    
    logger->info(fmt::format("Detected: {}", detector.projectTypeToString(info.type)));
    
    BuildToolValidator validator;
    std::vector<std::string> tools{info.buildTool};
    auto missing = validator.validateTools(tools);
    
    if (!missing.empty()) {
        for (const auto& tool : missing) {
            logger->error(fmt::format("Build tool '{}' is missing, please install it to continue", tool));
            logger->info(fmt::format("Install: {}", validator.getInstallGuide(tool)));
        }
        return;
    }
    
    logger->info(fmt::format("Build tool '{}' validated", info.buildTool));
    
    auto start = std::chrono::high_resolution_clock::now();
    logger->info(fmt::format("Building with: {}", info.buildCommand));
    fmt::print("\n");
    
    BuildExecutor executor;
    int result = executor.executeWithOutput(info.buildCommand);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    fmt::print("\n");
    if (result == 0) {
        logger->info(fmt::format("✓ Build successful ({}s)", duration.count()));
    } else {
        logger->error(fmt::format("✗ Build failed ({}s)", duration.count()));
    }
}

void cmdRebuild(const std::string& projectPath) {
    auto logger = Logger::get();
    
    ProjectDetector detector;
    ProjectInfo info = detector.detect(projectPath);
    
    if (info.type == ProjectType::UNKNOWN) {
        logger->error("Could not detect project type");
        return;
    }
    
    logger->info("Performing clean rebuild...");
    
    BuildToolValidator validator;
    std::vector<std::string> tools{info.buildTool};
    auto missing = validator.validateTools(tools);
    
    if (!missing.empty()) {
        for (const auto& tool : missing) {
            logger->error(fmt::format("Build tool '{}' is missing", tool));
        }
        return;
    }
    
    BuildExecutor executor;
    
    #ifdef _WIN32
        logger->info("Cleaning build directory...");
        executor.executeWithOutput("rmdir /s /q build 2>nul");
    #else
        logger->info("Cleaning build directory...");
        executor.executeWithOutput("rm -rf build");
    #endif
    
    cmdBuild(projectPath);
}

void cmdDetect(const std::string& projectPath) {
    auto logger = Logger::get();
    
    ProjectDetector detector;
    ProjectInfo info = detector.detect(projectPath);
    
    if (info.type == ProjectType::UNKNOWN) {
        logger->warn("Unknown project type");
        return;
    }
    
    fmt::print("\n");
    logger->info(fmt::format("Project Type: {}", detector.projectTypeToString(info.type)));
    logger->info(fmt::format("Language: {}", info.language));
    logger->info(fmt::format("Build Tool: {}", info.buildTool));
    logger->info(fmt::format("Build Command: {}", info.buildCommand));
    fmt::print("\n");
}

void cmdValidate(const std::string& projectPath) {
    auto logger = Logger::get();
    
    ProjectDetector detector;
    ProjectInfo info = detector.detect(projectPath);
    
    if (info.type == ProjectType::UNKNOWN) {
        logger->warn("Unknown project type");
        return;
    }
    
    BuildToolValidator validator;
    std::vector<std::string> tools{info.buildTool};
    auto missing = validator.validateTools(tools);
    
    fmt::print("\n");
    if (missing.empty()) {
        logger->info(fmt::format("✓ Build tool '{}' is installed", info.buildTool));
        logger->info(fmt::format("Version: {}", validator.getVersion(info.buildTool)));
    } else {
        for (const auto& tool : missing) {
            logger->error(fmt::format("✗ Build tool '{}' is missing, please install it to continue", tool));
            logger->info(fmt::format("Install: {}", validator.getInstallGuide(tool)));
        }
    }
    fmt::print("\n");
}

void cmdInfo(const std::string& projectPath) {
    auto logger = Logger::get();
    
    ProjectDetector detector;
    ProjectInfo info = detector.detect(projectPath);
    
    if (info.type == ProjectType::UNKNOWN) {
        logger->warn("Unknown project type");
        return;
    }
    
    BuildToolValidator validator;
    
    fmt::print("\n");
    logger->info(fmt::format("Project: {}", projectPath));
    logger->info(fmt::format("Type: {}", detector.projectTypeToString(info.type)));
    logger->info(fmt::format("Language: {}", info.language));
    logger->info(fmt::format("Build Tool: {}", info.buildTool));
    
    if (validator.isInstalled(info.buildTool)) {
        logger->info(fmt::format("Status: ✓ Ready to build"));
    } else {
        logger->info(fmt::format("Status: ✗ Missing build tool"));
    }
    fmt::print("\n");
}

int main(int argc, char* argv[]) {
    Logger::init();
    ConfigManager::init();
    
    auto logger = Logger::get();
    
    std::string command = argc > 1 ? argv[1] : "help";
    std::string projectPath = argc > 2 ? argv[2] : ".";
    
    if (argc == 2 && (command == "help" || command == "--help" || command == "-h")) {
        printBanner();
        printHelp();
    } else if (command == "build") {
        printBanner();
        cmdBuild(projectPath);
    } else if (command == "rebuild") {
        printBanner();
        cmdRebuild(projectPath);
    } else if (command == "detect") {
        printBanner();
        cmdDetect(projectPath);
    } else if (command == "validate") {
        printBanner();
        cmdValidate(projectPath);
    } else if (command == "info") {
        printBanner();
        cmdInfo(projectPath);
    } else if (command == "help" || command == "--help" || command == "-h") {
        printBanner();
        printHelp();
    } else {
        printBanner();
        logger->error(fmt::format("Unknown command: {}", command));
        printHelp();
    }
    
    return 0;
}
