#include "build_executor.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <array>

int BuildExecutor::execute(const std::string& command) {
    return system(command.c_str());
}

int BuildExecutor::executeWithOutput(const std::string& command, OutputCallback callback) {
    std::array<char, 256> buffer;
    std::string result;
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), &pclose);
    if (!pipe) {
        return -1;
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        std::string line(buffer.data());
        std::cout << line;
        
        if (callback) {
            callback(line);
        }
    }
    
    return 0;
}

std::string BuildExecutor::captureOutput(const std::string& command) {
    std::array<char, 256> buffer;
    std::string result;
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), &pclose);
    if (!pipe) {
        return "";
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}
