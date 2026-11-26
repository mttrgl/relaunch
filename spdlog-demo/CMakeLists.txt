cmake_minimum_required(VERSION 3.16.0)
project(spdlog-demo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(spdlog
    GIT_REPOSITORY git@github.com:gabime/spdlog.git
    GIT_TAG v1.15.0
    GIT_SHALLOW ON
)


# 必须在 FetchContent_MakeAvailable 之前设置，否则 spdlog 自己的 CMakeLists 已执行完
set(SPDLOG_HEADER_ONLY ON CACHE BOOL "Header-only mode, do not build static lib" FORCE)
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "Use std::format instead of fmt::format" FORCE)
set(SPDLOG_DISABLE_DEFAULT_LOGGER ON CACHE BOOL "Disable default logger" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Build spdlog examples" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Build spdlog tests" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "Build spdlog benchmarks" FORCE)

FetchContent_MakeAvailable(spdlog)

# 自动收集当前目录下所有 .cpp 文件
file(GLOB DEMO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(demo ${DEMO_SOURCES})
target_link_libraries(demo PRIVATE spdlog::spdlog_header_only)