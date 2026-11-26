#include "async_log.h"
#include "common.h"
#include "concurrent_log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

void CheckSpdlogVersion() {
#ifdef SPDLOG_HEADER_ONLY
  spdlog::info("Hello spdlog (header-only)!");
#else
  spdlog::info("Hello spdlog (compiled)!");
#endif

  spdlog::info("spdlog version: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

  SPDLOG_INFO("Hello spdlog (header-only)!");
}

int main() {
  auto parent_path = exe_parent_path();
  auto log_path = parent_path / "demo.log";
  auto fs = std::make_shared<spdlog::sinks::basic_file_sink_st>(log_path, true);
  auto logger = std::make_shared<spdlog::logger>("demo", fs);
  logger->set_level(spdlog::level::trace);
  spdlog::set_default_logger(logger);

  CheckSpdlogVersion();

  ConcurrentLog::Run();
  AsyncLog::Run();

  return 0;
}