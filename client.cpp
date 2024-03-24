#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char hello[1024];
    char buffer[1024] = {0};

    // 创建套接字
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("创建套接字失败");
        return -1;
    }

    // 初始化服务器地址
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // 将 IP 地址转换为网络地址格式
    if (inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr) <= 0) {
        perror("转换失败");
        return -1;
    }

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("连接失败");
        return -1;
    }
    else{
        std::cout << "连接服务器成功!!!!" << std::endl;
    }
    
    while(1){
    // 发送消息给服务器
    printf("input: ");
    scanf("%s",hello);
    send(clientSocket,hello,sizeof(hello),0);

    // 接收服务器的响应
    int valread = read(clientSocket, buffer, 1024);
    if (valread < 0) {
        perror("未接收到数据");
        break;
    } else {
        std::cout << "服务器回复: " << buffer << std::endl;
    }
   }
    close(clientSocket);
    return 0;
}
