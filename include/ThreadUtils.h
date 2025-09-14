#pragma once
#include <functional>
#include <vector>

class ThreadUtils {
public:
    // 启动多线程处理任务
    static void runTasks(const std::vector<std::function<void()>>& tasks);
};
