#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "enums.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mysql.h>

using namespace std;

class Server
{
private:
	MYSQL* connection, mysql;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int queryState;
	string mysqlLogin, mysqlPassword;

	int counter;
	map<int, SOCKET> connections;
	map<int, string> usernames;
	void clientHandler(int index);
	bool checkConnection(int index);
public:
	Server(string address, string mysqlLogin = "root", string mysqlPassword = "toor");
	~Server();
};