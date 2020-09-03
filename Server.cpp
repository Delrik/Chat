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
	addr.sin_port = htons(2020);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (sockaddr*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	
	
	SOCKET buf;
	while (true) {
		buf = accept(sListen, (sockaddr*)&addr, &sizeofaddr);
		if (counter < 256) {
			for (int i = 0; i < 256; i++) {
				if (i == 255 && connections.find(i) != connections.end()) {
					//SAY NO
					break;
				}
				if (connections.find(i) != connections.end()) continue;
				connections.insert(make_pair(i, buf));
				thread th([&,this]() {this->clientHandler(i); });
				th.detach();
			}
			counter++;
		}
		else {
			//SAY NO;
		}
	}
}

void Server::clientHandler(int index)
{
	char msg[256];
	while (true) {
		recv(connections[index], msg, sizeof(msg), NULL);
		for (auto it = connections.begin(); it != connections.end(); it++) {
			if (it->first == index) continue;
			send(it->second, msg, sizeof(msg), NULL);
		}
	}
}