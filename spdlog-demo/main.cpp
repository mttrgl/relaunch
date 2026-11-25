#include "async_log.h"
#include "concurrent_log.h"

#include <spdlog/spdlog.h>

void CheckSpdlogVersion() {
#ifdef SPDLOG_HEADER_ONLY
  spdlog::info("Hello spdlog (header-only)!");
#else
  spdlog::info("Hello spdlog (compiled)!");
#endif

  spdlog::info("spdlog version: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
}

int main() {
  CheckSpdlogVersion();

  ConcurrentLog::Run();
  AsyncLog::Run();

  return 0;
}