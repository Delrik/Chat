#include "Server.h"


Server::Server(string address, string mysqlLogin, string mysqlPassword)
{
	counter = 0;
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql, address.c_str(),
		mysqlLogin.c_str(), mysqlPassword.c_str(), "chat_db", 3306, 0, 0);
	if (connection == NULL) {
		cout << "Error 3 : (" << mysql_error(&mysql) << ")" << endl;
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


void Server::clientHandler(int index)
{
	cout << "id " << index << " connected to the chat\n";

	msgType type = FAILURE;
	MYSQL_RES* result;
	MYSQL_ROW row;
	int query_state;
	string query, buf;
	bool response;
	size_t pass, pass2;
	bool connectionCheck = true;
	char msg[256], msg1[256];
	char prevMsg[256];
	while (true) {
		type = FAILURE;
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
					usernames.erase(index);
					cout << "id " << index << " disconnected from the chat\n";
					return;
				}
			}
			buf = usernames[index] + ": ";
			strcpy_s(msg1, buf.c_str());
			for (auto it = connections.begin(); it != connections.end(); it++) {
				send(it->second, (char*)&type, sizeof(msgType), NULL);
				send(it->second, msg1, sizeof(msg1), NULL);
				send(it->second, (char*)&type, sizeof(msgType), NULL);
				send(it->second, msg, sizeof(msg), NULL);
			}
			query = "INSERT INTO Messages(user_id,message) VALUES((SELECT id FROM Users WHERE username='" + usernames[index] + "'),'" + msg + "');";
			query_state = mysql_query(connection, query.c_str());
			strcpy_s(prevMsg, msg);
			break;
		case CONNECTION:
			recv(connections[index], (char*)&connectionCheck, sizeof(bool), NULL);
			if (!connectionCheck) {
				closesocket(connections[index]);
				connections.erase(index);
				usernames.erase(index);
				cout << "id " << index << " disconnected from the chat\n";
				return;
			}
			break;
		case REGISTER:
			recv(connections[index], msg, sizeof(msg), NULL);
			buf = string(msg);
			query = "SELECT * FROM Users WHERE username = '" + buf + "'";
			recv(connections[index], (char*)&pass, sizeof(pass), NULL);
			type = REGISTER;
			send(connections[index], (char*)&type, sizeof(type), 0);
			query_state = mysql_query(connection, query.c_str());
			response = false;
			if (query_state != 0) {
				send(connections[index], (char*)&response, sizeof(bool), 0);
				break;
			}
			result = mysql_store_result(connection);
			row = mysql_fetch_row(result);
			if (row == NULL) {
				query = "INSERT INTO Users(username,password) VALUES('" + buf + "','" + to_string(pass) + "');";
				query_state = mysql_query(connection, query.c_str());
				if (query_state != 0) {
					send(connections[index], (char*)&response, sizeof(bool), 0);
					break;
				}
				response = true;
				usernames.insert(make_pair(index, buf));
			}
			send(connections[index], (char*)&response, sizeof(bool), 0);
			break;
		case AUTH:
			recv(connections[index], msg, sizeof(msg), NULL);
			buf = string(msg);
			query = "SELECT password FROM users WHERE username = '" + buf + "';";
			recv(connections[index], (char*)&pass, sizeof(pass), NULL);
			type = AUTH;
			send(connections[index], (char*)&type, sizeof(type), 0);
			query_state = mysql_query(connection, query.c_str());
			response = false;
			if (query_state != 0) {
				send(connections[index], (char*)&response, sizeof(bool), 0);
				break;
			}
			result = mysql_store_result(connection);
			row = mysql_fetch_row(result);
			if (row != NULL) {
				sscanf_s(row[0], "%zu", &pass2);
				if (pass == pass2) {
					response = true;
					usernames.insert(make_pair(index, buf));
				}
			}
			send(connections[index], (char*)&response, sizeof(bool), 0);
			break;

		default:
			closesocket(connections[index]);
			connections.erase(index);
			usernames.erase(index);
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
