#pragma once
#include "TaskQueue.h"
#include "TaskQueue.cpp"

template <typename T>
class ThreadPool
{
private:
    TaskQueue<T> *taskQ;
    pthread_t managerID;  // 管理者线程ID
    pthread_t *threadIDs; // 工作的线程ID
    int minNum;
    int maxNum;
    int busyNum;               // 工作的线程数
    int liveNum;               // 总的存活的线程数
    int exitNum;               // 需要杀死多少个线程数
    pthread_mutex_t mutexpool; // 锁整个线程池
    pthread_cond_t notEmpty;   // 任务队列是不是空了
    static const int NUMBER = 2;

    bool shutdown;

    static void *worker(void *arg);
    static void *manager(void *arg);

    void threadExit();

public:
    // 创建线程池并初始化
    // 销毁线程池
    ThreadPool(int min, int max);
    ~ThreadPool();

    // 给线程池添加任务
    void addTask(Task<T> task);

    // 获取线程池中工作的线程的个数
    int getBusyNum();

    // 获取线程池中活着的线程的个数
    int getAliveNum();
};
