#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

void clientThread(int id) {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1024] = {0};
    char hello[1024];
    // 创建套接字
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror(("客户端 " + std::to_string(id) + " 套接字创建失败").c_str()); //string 转换为 const char*
        return;
    }

    // 初始化服务器地址
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // 将 IP 地址转换为网络地址格式
    if (inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr) <= 0) {
        perror(("客户端 " + std::to_string(id) + " 无效地址或不支持的地址").c_str()); 
        return;
    }

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror(("客户端 " + std::to_string(id) + " 连接失败").c_str());
        return;
    }

    // 发送消息给服务器
    //char message[1024] = ("来自客户端 " + std::to_string(id) + " 的问候" + '\0').c_str();
    sprintf(hello,"来自客户端%d的问候",id);
    send(clientSocket, hello, 1024, 0);
    std::cout << "客户端 " << id << " 向服务器发送消息" << std::endl;

    // 接收服务器的响应
    int valread = read(clientSocket, buffer, 1024);
    if (valread <= 0) {
        std::cerr << "客户端 " << id << " 未收到服务器响应" << std::endl;
    } else {
        std::cout << "客户端 " << id << " 收到服务器响应: " << buffer << std::endl;
    }

    close(clientSocket);
}

int main() {
    std::vector<std::thread> threads;

    // 创建多个客户端线程
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(std::thread(clientThread, i + 1));
    }

    // 等待所有线程结束
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
