#pragma once

#include "common.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <ranges>
#include <thread>
#include <vector>

static void benchmark_single_thread() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::microseconds;
  using std::chrono::steady_clock;

  const int num_logs = 1000000;

  auto log_path = make_spdlog_path("bench_st.log");
  auto logger = spdlog::basic_logger_st("bench_st", log_path, true);
  //   logger->set_pattern("%Y-%m-%d %H:%M:%S.%e %l %n %v");
  //   auto fs = std::make_shared<spdlog::sinks::basic_file_sink_st>(log_path, true);
  //   auto logger = std::make_shared<spdlog::logger>("bench_st", fs);

  auto start = steady_clock::now();
  for (auto i : std::views::iota(0, num_logs) | std::views::take(num_logs)) {
    logger->info("single thread logger: message number {}", i);
  }
  auto end = steady_clock::now();
  logger->info("bench_st: {} us", duration_cast<microseconds>(end - start).count());
}

static void benchmark_multi_thread() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::microseconds;
  using std::chrono::steady_clock;

  const int num_threads = 32;
  const int num_logs = 100000;

  auto log_path = make_spdlog_path("bench_mt.log");
  auto logger = spdlog::basic_logger_mt("bench_mt", log_path, true);
  //   logger->set_pattern("%Y-%m-%d %H:%M:%S.%e %l %n %v");
  //   auto fs = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, true);
  //   auto logger = std::make_shared<spdlog::logger>("bench_mt", fs);

  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  auto start = steady_clock::now();
  for (auto i : std::views::iota(0, num_threads) | std::views::take(num_threads)) {
    threads.emplace_back([i, logger, num_logs]() {
      std::ostringstream oss;
      auto thread_id = std::this_thread::get_id();
      oss << thread_id;
      auto thread_id_str = oss.str();
      for (auto j : std::views::iota(0, num_logs) | std::views::take(num_logs)) {
        logger->info("multi thread logger: message number {} from thread number {}", j, thread_id_str);
      }
    });
  }
  for (auto &thread : threads) {
    thread.join();
  }

  auto end = steady_clock::now();
  logger->info("bench_mt: {} us", duration_cast<microseconds>(end - start).count());
}
