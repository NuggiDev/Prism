#pragma once

#include <memory>
#include <spdlog/spdlog.h>

class Logger {
public:
    static void init();
    static std::shared_ptr<spdlog::logger> get();
    static void setQuiet(bool quiet);
    
private:
    static std::shared_ptr<spdlog::logger> instance;
    static bool quietMode;
};
