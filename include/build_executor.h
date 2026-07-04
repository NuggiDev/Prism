#pragma once

#include <string>
#include <functional>

class BuildExecutor {
public:
    typedef std::function<void(const std::string&)> OutputCallback;
    
    int execute(const std::string& command);
    int executeWithOutput(const std::string& command, OutputCallback callback = nullptr);
    
private:
    std::string captureOutput(const std::string& command);
};
