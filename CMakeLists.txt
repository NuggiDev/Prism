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

target_link_libraries(prism PRIVATE
    fmt
    spdlog::spdlog
)

# Only link pthread if we are on Linux/Unix, NOT on Windows
if(UNIX AND NOT APPLE)
    target_link_libraries(prism PRIVATE pthread)
endif()

# --- Installation & Packaging ---
install(TARGETS prism DESTINATION bin)

set(CPACK_PACKAGE_NAME "prism")
set(CPACK_PACKAGE_VENDOR "NuggiDev")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "DEB;RPM")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "annalisatorcello9@gmail.com")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27), libstdc++6 (>= 8)")

include(CPack)
