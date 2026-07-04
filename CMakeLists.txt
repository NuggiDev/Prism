cmake_minimum_required(VERSION 3.20)
project(Prism CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(fmt
    URL https://github.com/fmtlib/fmt/releases/download/10.1.1/fmt-10.1.1.zip
)

FetchContent_Declare(spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.13.0.zip
)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip
)

FetchContent_MakeAvailable(fmt spdlog json)

add_executable(prism
    src/main.cpp
    src/project_detector.cpp
    src/build_tool_validator.cpp
    src/build_executor.cpp
    src/logger.cpp
    src/config_manager.cpp
)

target_include_directories(prism PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${spdlog_SOURCE_DIR}/include
    ${json_SOURCE_DIR}/single_include
)

target_link_libraries(prism PRIVATE
    fmt
    spdlog::spdlog
)

if(UNIX AND NOT APPLE)
    target_link_libraries(prism PRIVATE pthread)
endif()

install(TARGETS prism DESTINATION bin)
