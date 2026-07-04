#include "build_executor.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <array>
#include <cstdio>

#ifdef _WIN32
    #define popen _popen
    #define pclose _pclose
#else
    #include <sys/wait.h>
#endif

int BuildExecutor::execute(const std::string& command) {
    return system(command.c_str());
}

int BuildExecutor::executeWithOutput(const std::string& command, OutputCallback callback) {
    std::array<char, 256> buffer;
    
    FILE* pipeHandle = popen(command.c_str(), "r");
    if (!pipeHandle) {
        return -1;
    }
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(pipeHandle, &pclose);
    
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
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
    
    FILE* pipeHandle = popen(command.c_str(), "r");
    if (!pipeHandle) {
        return "";
    }
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(pipeHandle, &pclose);
    
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}
