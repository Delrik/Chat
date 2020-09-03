#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>

using namespace std;

class Client
{
private:
	void recvHandler();
	void sendHandler();
	SOCKET connection;

public:
	Client(string address);
	~Client();

};