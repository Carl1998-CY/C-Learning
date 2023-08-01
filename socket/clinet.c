#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>  //必须包含      

int main()
{
    // 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }

    // 连接服务器   指定服务器的IP和端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10000);       // 指定端口 转大端
    inet_pton(AF_INET, "10.106.195.9", &saddr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("connect");
        return -1;
    }

    // 连接成功，通信
    //发送数据
    int number = 0;
    while (1)
    {
        // 发送数据 存到buffer
        char buff[1024];
        sprintf(buff, "你好， %d\n", ++number);
        send(fd, buff, strlen(buff)+1, 0);  // strlen 不计算 \n 所以＋1
        
        //接收数据
        memset(buff, 0, sizeof(buff));
        int len = recv(fd, buff, sizeof(buff), 0);
        if (len > 0)
        {
            printf("server say: %s\n", buff);
        }
        else if (len == 0)
        {
            printf("服务器已经断开了连接...\n");
            break;
        }
        else
        {
            perror("recv");
            break;
        }
        sleep(1);   //发慢一点
    }

    //关掉对应的文件描述符
    close(fd);
    return 0;
}