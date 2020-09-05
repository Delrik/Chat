#include "Server.h"


Server::Server(string address)
{
	counter = 0;

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	addr.sin_addr.s_addr = NULL;
	addr.sin_port = htons(2020);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (sockaddr*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);


	SOCKET buf;
	msgType type;
	char msg[256] = "";
	while (true) {
		buf = accept(sListen, (sockaddr*)&addr, &sizeofaddr);
		if (counter < 256) {
			for (int i = 0; i < 256; i++) {
				/*if (i == 255 && connections.find(i) != connections.end()) {
					type = MESSAGE;
					strcpy(msg, "Server is full");
					send(buf, (char*)&type, sizeof(msgType), NULL);
					send(buf, (char*)&msg, sizeof(msg), NULL);
					closesocket(buf);
					counter--;
					break;
				}*/
				if (connections.find(i) != connections.end()) continue;
				connections.insert(make_pair(i, buf));
				thread th([&, this]() {this->clientHandler(i); });
				th.detach();
				break;
			}
			counter++;
		}
		else {
			type = MESSAGE;
			strcpy_s(msg, "Server is full");
			send(buf, (char*)&type, sizeof(msgType), NULL);
			send(buf, (char*)&msg, sizeof(msg), NULL);
			closesocket(buf);
		}
	}
}

Server::~Server()
{
}

bool Server::closeConnection(int index)
{
	if (connections.find(index) == connections.end()) return false;
	cout << "id " << index << " disconnected from the chat\n";
	return true;
}

void Server::clientHandler(int index)
{
	cout << "id " << index << " connected to the chat\n";
	msgType type = FAILURE;
	bool connectionCheck = true;
	char msg[256];
	char prevMsg[256];
	while (true) {
		recv(connections[index], (char*)&type, sizeof(msgType), NULL);
		switch (type)
		{
		case MESSAGE:
			recv(connections[index], msg, sizeof(msg), NULL);
			if (strcmp(msg, prevMsg) == 0)
			{
				if (!checkConnection(index)) {
					closesocket(connections[index]);
					connections.erase(index);
					cout << "id " << index << " disconnected from the chat\n";
					return;
				}
			}
			for (auto it = connections.begin(); it != connections.end(); it++) {
				if (it->first == index) continue;
				send(it->second, (char*)&type, sizeof(msgType), NULL);
				send(it->second, msg, sizeof(msg), NULL);
			}
			strcpy_s(prevMsg, msg);
			break;
		case CONNECTION:
			recv(connections[index], (char*)&connectionCheck, sizeof(bool), NULL);
			if (!connectionCheck) {
				closesocket(connections[index]);
				connections.erase(index);
				cout << "id " << index << " disconnected from the chat\n";
				return;
			}
			break;
		default:
			closesocket(connections[index]);
			connections.erase(index);
			cout << "id " << index << " disconnected from the chat\n";
			return;
		}
	}
}

bool Server::checkConnection(int index)
{
	bool buf = false;
	msgType type = CONNECTION;
	send(connections[index], (char*)&type, sizeof(msgType), NULL);
	recv(connections[index], (char*)&buf, sizeof(bool), NULL);
	if (!buf) {
		return false;
	}
	return true;
}
