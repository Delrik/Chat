#include "Client.h"
#include "Server.h"
#include <iostream>

using namespace std;

int main() {
	int choice;
	while (true) {
		cout << "1. I wanna be a server\n2. I wanna be a client\n";
		cin >> choice;
		system("cls");
		if (choice == 1) {
			Server S("127.0.0.1");
			break;
		}
		if (choice == 2) {
			Client C("127.0.0.1");
			break;
		}
	}
	system("pause");
	return 0;
}