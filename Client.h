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

class Client
{
private:
	bool readyToRead;
	SOCKET connection;
	void recvHandler();
	void sendHandler();
public:
	Client(string address);
	~Client();
};