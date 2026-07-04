#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::instance;
bool Logger::quietMode = false;

void Logger::init() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("%^[%l]%$ %v");
    
    std::vector<spdlog::sink_ptr> sinks{console_sink};
    instance = std::make_shared<spdlog::logger>("prism", sinks.begin(), sinks.end());
    instance->set_level(spdlog::level::info);
}

std::shared_ptr<spdlog::logger> Logger::get() {
    if (!instance) {
        init();
    }
    return instance;
}

void Logger::setQuiet(bool quiet) {
    quietMode = quiet;
    if (!instance) {
        init();
    }
    if (quiet) {
        instance->set_level(spdlog::level::off);
    } else {
        instance->set_level(spdlog::level::info);
    }
}
