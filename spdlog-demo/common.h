#pragma once

#include <spdlog/spdlog.h>

#include <filesystem>
#include <print>

void check_spdlog_version() {
  std::println("__cpp_lib_format: {}", __cpp_lib_format);
#ifdef SPDLOG_HEADER_ONLY
  std::println("Hello spdlog (header-only)!");
#else
  std::println("Hello spdlog (compiled)!");
#endif

  std::println("spdlog version: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
}

std::string make_spdlog_path(const std::string &name) {
  return {"/tmp/spdlogs/" + name}; // wsl 跨盘写 /mnt/d/... 性能暴跌100x~200x
  auto exe_path = std::filesystem::canonical("/proc/self/exe");
  auto log_path = exe_path.parent_path() / "logs/" / name;
  return log_path.string();
}