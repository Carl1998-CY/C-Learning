#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

struct Node
{
    int number;
    struct Node* next;
};

struct Node* head = NULL;

// producter
void* producer(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);     //临界区
        //创建新节点
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        //init node
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("生产者, id: %ld,  number: %d\n", pthread_self(), newNode->number);
        pthread_mutex_unlock(&mutex);  //临界区
        //生产出新的产品唤醒消费者
        pthread_cond_broadcast(&cond);
        sleep(rand() % 3);
    }
    return NULL;
}

// consumer
void* consumer(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);  //临界区
        while(head == NULL)
        {
            //阻塞消费者线程
            pthread_cond_wait(&cond, &mutex);
        }
        struct Node* node = head;
        printf("消费者, id: %ld,  number: %d\n", pthread_self(), node->number);
        //头节点后移
        head = node->next;
        free(node);
        pthread_mutex_unlock(&mutex);   //临界区
        //判断链表是不是空的
        sleep(rand() % 3);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

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
    pthread_cond_destroy(&cond);
    return 0;
}

 

