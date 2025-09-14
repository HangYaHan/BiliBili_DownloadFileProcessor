#include "ThreadUtils.h"
#include <thread>

void ThreadUtils::runTasks(const std::vector<std::function<void()>>& tasks) {
    std::vector<std::thread> threads;
    for (const auto& task : tasks) {
        threads.emplace_back(task);
    }
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}
