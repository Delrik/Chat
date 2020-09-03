#include "Client.h"

void Client::recvHandler()
{
	char msg[256];
	memset(msg, '\0', sizeof(msg));
	while (true) {
		recv(connection, msg, sizeof(msg), NULL);
		cout << msg << endl;
	}
}

void Client::sendHandler()
{
	char msg[256];
	while (true) {
		cin.getline(msg, sizeof(msg));
		if (msg == "0") break;
		send(connection, msg, sizeof(msg), NULL);
	}
}

Client::Client(string address) {
	
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