#include "Client.h"
#include "Server.h"
#include <iostream>

using namespace std;

int main() {
	char choice[2];
	while (true) {
		cout << "1. I wanna be a server\n2. I wanna be a client\n3. I wanna be a server(with specific IP)\n4. I wanna be a client(with specific IP)\n";
		cin.getline(choice, sizeof(choice));
		system("cls");
		if (choice[0] == '1') {
			Server S("127.0.0.1");
			return 0;
		}
		if (choice[0] == '2') {
			Client C("127.0.0.1");
			return 0;
		}
		if (choice[0] == '3') {
			char ip[20];
			cout << "IP: ";
			cin.getline(ip, sizeof(ip));
			Server S(ip, "root", "toor");
			return 0;
		}
		if (choice[0] == '4') {
			char ip[20];
			cout << "IP: ";
			cin.getline(ip, sizeof(ip));
			Client C(ip);
			return 0;
		}
	}
}