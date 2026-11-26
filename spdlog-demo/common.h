#pragma once

#include <filesystem>

static inline auto exe_parent_path() -> std::filesystem::path {
  auto path = std::filesystem::canonical("/proc/self/exe").parent_path();
  return path;
}