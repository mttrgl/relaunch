#include "bench.h"
#include "common.h"

int main() {
  check_spdlog_version();

  benchmark_single_thread();
  benchmark_multi_thread();

  return 0;
}