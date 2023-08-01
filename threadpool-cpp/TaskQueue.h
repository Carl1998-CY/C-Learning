#pragma once
#include <queue>
#include <pthread.h>
using namespace std;
using callback = void (*)(void *arg);

// 任务结构体
template<typename T>
struct Task
{
    Task()
    {
        function = nullptr;
        arg = nullptr;
    }
    Task(callback f, void *arg)
    {
        this->arg = (T* )arg;
        this->function = f;
    }
    callback function;
    T *arg;
};

//任务队列
template<typename T>
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    //添加任务
    void addTask(Task<T> task);
    void addTask(callback f, void* arg);
    
    //取出一个任务
    Task<T> takeTask();
    //获取当前任务的个数
    inline size_t taskNumber()
    {
        return m_taskQ.size();
    }

private:
    std::queue<Task<T>> m_taskQ; //自己维护
    pthread_mutex_t m_mutex;
};
