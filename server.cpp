// 服务端
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <queue>
#include <fstream>

#define PORT 8080
#define FILE_PATH "messages.txt"
#define MAX_CLIENTS 10
#define MAX_THREADS 5

// 创建一个客户端队列
std::queue<int> clientQueue;
// 定义互斥锁并且初始化
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 定义条件变量并且初始化
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//线程同步处理客户端请求
void *handleClient(void *arg) {
    while (true) {
        int clientSocket;
        pthread_mutex_lock(&mutex);
        if (clientQueue.empty()) {
            //为空阻塞，后续代码不执行
            pthread_cond_wait(&cond, &mutex);
        }
        // 获取套接字
        clientSocket = clientQueue.front();
        // 客户端套接字出队
        clientQueue.pop();
        pthread_mutex_unlock(&mutex);
        
        char buffer[1024] = {0};
        int valread = read(clientSocket, buffer, 1024);
        if (valread <= 0) {
            close(clientSocket);
            continue;
        }
        std::cout << "收到客户端消息: " << buffer << std::endl;
        //将接收的客户端消息写入到文件messages.txt
        std::ofstream file(FILE_PATH, std::ios::app);
        file << buffer << std::endl;
        file.close();
        
        //发送消息给客户端
        send(clientSocket, buffer, strlen(buffer), 0);
        close(clientSocket);
    }
}

int main() {
    int serverSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // 创建套接字
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 初始化并设置服务器地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // 设置监听
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        return -1;
    }
    else{
        printf("正在监听.........\n");
    }

    // 创建线程池
    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, handleClient, NULL) != 0) {
            perror("Failed to create thread");
            return -1;
        }
    }

    // 接受连接并且同步执行
    while (true) {
        int newSocket;
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            return -1;
        }
        else{
            printf("Accept Success!!!\n");
        }
        pthread_mutex_lock(&mutex);
        //客户端套接字入队
        clientQueue.push(newSocket);
        pthread_mutex_unlock(&mutex);
        //唤醒等待中的进程
        pthread_cond_signal(&cond);
    }

    return 0;
}




 