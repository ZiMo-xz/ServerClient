# 设置编译器和编译选项
CC = g++
CFLAGS = -std=c++11 -pthread

# 目标文件
SERVER_TARGET = Server
CLIENT_TARGET = Client
MULTCLIENT_TARGET = MultClient

# 源文件
SERVER_SRCS = server.cpp
CLIENT_SRCS = client.cpp
MULTCLIENT_SRCS = MultClient.cpp

# 生成可执行文件
all: $(SERVER_TARGET) $(CLIENT_TARGET) $(MULTCLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SRCS)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_SRCS)

$(CLIENT_TARGET): $(CLIENT_SRCS)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRCS)

$(MULTCLIENT_TARGET): $(MULTCLIENT_SRCS)
	$(CC) $(CFLAGS) -o $(MULTCLIENT_TARGET) $(MULTCLIENT_SRCS)

# 清理目标文件
clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) $(MULTCLIENT_TARGET)

