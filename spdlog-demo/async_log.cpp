#include "async_log.h"
#include "common.h"

#include <chrono>
#include <print>
#include <ranges>
#include <thread>
#include <vector>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

constexpr int ThreadCount = 16;
constexpr int ThreadLoopCount = 10000;
constexpr std::string AsyncLoggerName = "async";

void AsyncLog::Run() {
  auto parent_path = exe_parent_path();
  auto log_path = parent_path / "async_demo.log";
  auto tp = std::make_shared<spdlog::details::thread_pool>(65536, 1);
  auto fs = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path);
  auto logger = std::make_shared<spdlog::async_logger>(AsyncLoggerName, fs, tp, spdlog::async_overflow_policy::block);
  logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
  spdlog::register_logger(logger);

  std::vector<std::thread> threads;
  std::vector<std::chrono::milliseconds> max_cost_times(ThreadCount, std::chrono::milliseconds(0));

  auto start = std::chrono::steady_clock::now();
  for (auto i : std::views::iota(0, ThreadCount)) {
    threads.emplace_back([&max_cost_times, logger, i]() {
      for (auto j : std::views::iota(0, ThreadLoopCount)) {
        auto start_time = std::chrono::steady_clock::now();
        logger->info("async thread={}, seq={}", i, j);

        auto cost_time = std::chrono::steady_clock::now() - start_time;
        if (cost_time > std::chrono::milliseconds(10) && cost_time > max_cost_times[i]) {
          max_cost_times[i] = std::chrono::duration_cast<std::chrono::milliseconds>(cost_time);
          spdlog::warn("async thread={}, seq={} took {} ms, loop {}", i, j, max_cost_times[i].count(), j);
        }
      }
    });
  }
  for (auto &t : threads) {
    t.join();
  }

  spdlog::info("async total cost times : {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count());
  spdlog::info("async thread spdlog closed");
  spdlog::drop(AsyncLoggerName);
  start = std::chrono::steady_clock::now();
  logger->flush();
  logger.reset();
  spdlog::info("async thread flush cost times : {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count());

  std::println("async thread max cost times for each thread:");
  for (const auto &[i, cost] : max_cost_times | std::views::enumerate) {
    std::println("async thread {}: {} ms", i, cost.count());
  }
}
