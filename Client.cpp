#include "Client.h"

void Client::recvHandler()
{
	msgType type = FAILURE;
	bool check = true;
	char msg[256];
	while (true) {
		recv(connection, (char*)&type, sizeof(msgType), NULL);
		switch (type)
		{
		case MESSAGE:
			recv(connection, msg, sizeof(msg), NULL);
			if(readyToRead) cout << msg << endl;
			break;
		case CONNECTION:
			send(connection, (char*)&check, sizeof(bool), NULL);
			break;
		default:
			break;
		}
	}
}

void Client::sendHandler()
{
	char option[2];
	msgType type = FAILURE;
	char msg[256];
	while (true) {
		cout << "Choose the option:\n1. Start chatting\n";
		cin.getline(option, sizeof(option));
		cin.clear();
		system("cls");
		switch (option[0])
		{
		case '1':
			cout << "Input 0 to go back\n";
			readyToRead = true;
			while (true) {
				cin.getline(msg, sizeof(msg));
				cin.clear();
				if (strcmp(msg, "0")==0)
				{
					readyToRead = false;
					system("cls");
					break;
				}
				type = MESSAGE;
				send(connection, (char*)&type, sizeof(msgType), NULL);
				send(connection, msg, sizeof(msg), NULL);
			}
			break;
		default:
			break;
		}
		
	}
}

Client::Client(string address) {
	readyToRead = false;
	
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Errror" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	addr.sin_port = htons(2020);
	addr.sin_family = AF_INET;
	
	
	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (sockaddr*)&addr, sizeof(addr)) != 0) {
		cout << "Error 2" << endl;
	}
	else {
		thread a([this]() {this->recvHandler(); });
		a.detach();
		sendHandler();
	}
}

Client::~Client()
{
}