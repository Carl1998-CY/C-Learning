#include <iostream>
#include "ThreadPool.h"
#include "ThreadPool.cpp"       //  类模板需要包含源文件
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

void taskFunc(void* arg)
{
    int num = *(int*)arg;
    printf("thread %ld  %d\n",
        pthread_self(), num);
    cout << "Thread" << to_string(pthread_self()) << "is working, number = " << num << endl;
    sleep(1);
}

int main()
{
    // 创建线程池
    ThreadPool<int> pool(3, 100);

    for (int i = 0; i < 100; ++i)
    {
        int* num = new int(i + 100);
        pool.addTask(Task<int>(taskFunc, num));
    }

    sleep(30);

    return 0;
}
