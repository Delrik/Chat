#include "Client.h"

void Client::recvHandler()
{
	msgType type;
	bool check = true;
	char msg[256];
	bool response;
	bool counter = false;
	while (true) {
		type = FAILURE;
		recv(connection, (char*)&type, sizeof(msgType), NULL);
		switch (type)
		{
		case MESSAGE:
			recv(connection, msg, sizeof(msg), NULL);
			mut.lock();
			if (readyToRead) cout << msg;
			if (counter) {
				cout << endl;
				mut.unlock();
				mut.unlock();
			}
			counter = !counter;
			break;
		case CONNECTION:
			send(connection, (char*)&check, sizeof(bool), NULL);
			break;
		case AUTH:
			recv(connection, (char*)&response, sizeof(bool), NULL);
			authenticated = response;
			boolBuf = true;
			break;
		case REGISTER:
			recv(connection, (char*)&response, sizeof(bool), NULL);
			authenticated = response;
			boolBuf = true;
			break;
		default:
			break;
		}
	}
}

void Client::sendHandler()
{
	char option[2];
	int key;
	bool response;
	size_t pass;
	msgType type = FAILURE;
	char msg[256], msg1[256];
	while (true) {
		system("cls");
		cout << "Choose the option:\n1. Start chatting\n2. Sign in\n3. Sign up\n4. Sing out\n5. Quit\nOption: ";
		cin.getline(option, sizeof(option));
		cin.clear();
		system("cls");
		switch (option[0])
		{
		case '1':
			if (!authenticated) {
				cout << "You're not logged in\n";
				system("pause");
				break;
			}
			readyToRead = true;
			cout << "Press \"Enter\" button to type a message and send it\n";
			while (true) {
				key = 0;
				while (key != 13) {
					key = _getch();
				}
				mut.lock();
				system("cls");
				cout << "Input 0 to go back\n";
				cout << username << ": ";
				cin.getline(msg, sizeof(msg));
				cin.clear();
				if (strcmp(msg, "0") == 0)
				{
					mut.unlock();
					readyToRead = false;
					break;
				}
				type = MESSAGE;
				send(connection, (char*)&type, sizeof(msgType), NULL);
				send(connection, msg, sizeof(msg), NULL);
				system("cls");
				cout << "Press \"Enter\" button to type a message and send it\n";
				mut.unlock();
			}
			break;
		case '2':
			if (authenticated) {
				cout << "You're already logged in\n";
				system("pause");
				break;
			}
			type = AUTH;
			cout << "Sign in form\nLogin: ";
			cin.getline(msg, sizeof(msg));
			username = string(msg);
			cout << "Password: ";
			cin.getline(msg1, sizeof(msg1));
			pass = hash<std::string>{}(string(msg1));
			send(connection, (char*)&type, sizeof(msgType), NULL);
			send(connection, msg, sizeof(msg), NULL);
			send(connection, (char*)&pass, sizeof(pass), NULL);
			boolBuf = false;
			while (!boolBuf);
			if (authenticated) {
				cout << "Successfuly logged in\n";
			}
			else cout << "Wrong login or password\n";
			system("pause");
			break;
		case '3':
			type = REGISTER;
			cout << "Sign in form\nLogin: ";
			cin.getline(msg, sizeof(msg));
			username = string(msg);
			cout << "Password: ";
			cin.getline(msg1, sizeof(msg1));
			pass = hash<std::string>{}(string(msg1));
			send(connection, (char*)&type, sizeof(msgType), NULL);
			send(connection, (char*)&msg, sizeof(msg), NULL);
			send(connection, (char*)&pass, sizeof(size_t), NULL);
			boolBuf = false;
			while (!boolBuf);
			if (authenticated) {
				cout << "Successfuly created an account and logged in\n";
			}
			else cout << "Registration denied with this data\n";
			system("pause");
			break;
		case '4':
			if (!authenticated) break;
			username = "";
			authenticated = false;
			pass = 0;
			break;
		case '5':
			exit(0);
			break;
		default:
			break;
		}
		
	}
}

Client::Client(string address) {
	readyToRead = false;
	authenticated = false;
	boolBuf = false;
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