#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "enums.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <conio.h>

using namespace std;

class Client
{
private:
	bool readyToRead;
	bool authenticated;
	bool boolBuf;
	string username;
	SOCKET connection;
	recursive_mutex mut;
	void recvHandler();
	void sendHandler();
public:
	Client(string address);
	~Client();
};