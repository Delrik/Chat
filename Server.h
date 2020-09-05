#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "enums.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>

using namespace std;

class Server
{
private:
	int counter;
	map<int, SOCKET> connections;
	bool closeConnection(int index);
	void clientHandler(int index);
	bool checkConnection(int index);
public:
	Server(string address);
	~Server();
};