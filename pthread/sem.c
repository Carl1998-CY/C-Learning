#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// 添加生产者和消费者信号量类型
sem_t semp;
sem_t semc;

pthread_mutex_t mutex;

struct Node
{
    int number;
    struct Node *next;
};

struct Node *head = NULL;

// producter
void *producer(void *arg)
{
    while (1)
    {
        // 生产之前调用sem_wait检查一下是否有资源
        sem_wait(&semp);
        pthread_mutex_lock(&mutex); //放在sem_wait之后，这样就不会被sem_wait = 0 的情况的线程阻塞，从而造成死锁
        // 创建新节点
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        // init node
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("生产者, id: %ld,  number: %d\n", pthread_self(), newNode->number);

        // 生产出新的产品唤醒消费者
        pthread_mutex_unlock(&mutex);
        sem_post(&semc);
        sleep(rand() % 3);
    }
    return NULL;
}

// consumer
void *consumer(void *arg)
{
    while (1)
    {
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);
        struct Node *node = head;
        printf("消费者, id: %ld,  number: %d\n", pthread_self(), node->number);
        // 头节点后移
        head = node->next;
        free(node);
        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        // 判断链表是不是空的
        sleep(rand() % 3);
    }
    return NULL;
}

int main()
{
    // 生产者初始化
    sem_init(&semp, 0, 5); // 会造成多个线程添加链表，要做线程同步  要锁
    // 消费者初始化为0
    sem_init(&semc, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    pthread_t t1[5], t2[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&t1[i], NULL, producer, NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    // 销毁信号量
    sem_destroy(&semc);
    sem_destroy(&semp);
    return 0;
}