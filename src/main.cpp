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
    fmt::println("\n  ╔════════════════════════════════════════╗");
    fmt::println("  ║    Prism - Universal Build System    ║");
    fmt::println("  ║           v1.0.0                     ║");
    fmt::println("  ╚════════════════════════════════════════╝\n");
}

void printHelp() {
    fmt::println("  Usage:");
    fmt::println("    prism build [path]      Detect and build project");
    fmt::println("    prism rebuild [path]    Clean and rebuild");
    fmt::println("    prism clean [path]      Remove build artifacts");
    fmt::println("    prism detect [path]     Show detected project type");
    fmt::println("    prism validate [path]   Check required build tools");
    fmt::println("    prism info [path]       Show project information");
    fmt::println("    prism help              Show this help\n");
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
    fmt::println("");
    
    BuildExecutor executor;
    int result = executor.executeWithOutput(info.buildCommand);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    fmt::println("");
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
    
    fmt::println("");
    logger->info(fmt::format("Project Type: {}", detector.projectTypeToString(info.type)));
    logger->info(fmt::format("Language: {}", info.language));
    logger->info(fmt::format("Build Tool: {}", info.buildTool));
    logger->info(fmt::format("Build Command: {}", info.buildCommand));
    fmt::println("");
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
    
    fmt::println("");
    if (missing.empty()) {
        logger->info(fmt::format("✓ Build tool '{}' is installed", info.buildTool));
        logger->info(fmt::format("Version: {}", validator.getVersion(info.buildTool)));
    } else {
        for (const auto& tool : missing) {
            logger->error(fmt::format("✗ Build tool '{}' is missing, please install it to continue", tool));
            logger->info(fmt::format("Install: {}", validator.getInstallGuide(tool)));
        }
    }
    fmt::println("");
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
    
    fmt::println("");
    logger->info(fmt::format("Project: {}", projectPath));
    logger->info(fmt::format("Type: {}", detector.projectTypeToString(info.type)));
    logger->info(fmt::format("Language: {}", info.language));
    logger->info(fmt::format("Build Tool: {}", info.buildTool));
    
    if (validator.isInstalled(info.buildTool)) {
        logger->info(fmt::format("Status: ✓ Ready to build"));
    } else {
        logger->info(fmt::format("Status: ✗ Missing build tool"));
    }
    fmt::println("");
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
