#include "concurrent_log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <print>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

constexpr int ThreadCount = 4;
constexpr int ThreadLoopCount = 10000;
constexpr std::string ConcurrentLoggerName = "concurrent";

void ConcurrentLog::Run() {
  auto fs = std::make_shared<spdlog::sinks::basic_file_sink_mt>("spdlog-demo.log", true);
  auto logger = std::make_shared<spdlog::logger>(ConcurrentLoggerName, fs);

  logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
  spdlog::register_logger(logger);

  std::vector<std::jthread> threads;
  std::vector<std::chrono::milliseconds> max_cost_times(ThreadCount, std::chrono::milliseconds(0));

  auto start = std::chrono::steady_clock::now();
  for (auto i : std::views::iota(0, ThreadCount)) {
    threads.emplace_back(std::jthread([&max_cost_times, logger, i]() {
      for (auto j : std::views::iota(0, ThreadLoopCount)) {
        auto start_time = std::chrono::steady_clock::now();
        logger->info("concurrent thread {}: log message {}", i, j);

        auto cost_time = std::chrono::steady_clock::now() - start_time;
        if (cost_time > std::chrono::milliseconds(10) && cost_time > max_cost_times[i]) {
          max_cost_times[i] = std::chrono::duration_cast<std::chrono::milliseconds>(cost_time);
          spdlog::warn("concurrent thread {}: log message {} took {} ms, loop {}", i, j, max_cost_times[i].count(), j);
        }
      }
    }));
  }

  for (auto &t : threads) {
    t.join();
  }

  spdlog::info("concurrent total cost times : {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count());
  spdlog::info("concurrent thread spdlog closed");
  spdlog::drop(ConcurrentLoggerName);
  logger->flush();
  logger.reset();

  std::println("concurrent thread max cost times for each thread:");
  for (const auto &[i, cost] : max_cost_times | std::views::enumerate) {
    std::println("concurrent thread {}: {} ms", i, cost.count());
  }
}