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


set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Build example" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "Build benchmarks" FORCE)
set(SPDLOG_HEADER_ONLY ON CACHE BOOL "Build header-only" FORCE)
set(SPDLOG_COMPILED_LIB OFF CACHE BOOL "Build compiled library" FORCE)

FetchContent_MakeAvailable(spdlog)

FetchContent_MakeAvailable(spdlog)

# 自动收集当前目录下所有 .cpp 文件
file(GLOB DEMO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(demo ${DEMO_SOURCES})
target_link_libraries(demo PRIVATE spdlog::spdlog_header_only)

