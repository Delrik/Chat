#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>


using namespace std;

class Server
{
private:
	map<int, SOCKET> connections;
	void clientHandler(int index);
	int counter;
public:
	Server(string address);
};