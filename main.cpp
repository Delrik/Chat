#pragma comment(lib, "Ws2_32.lib")
#include "Client.h"
#include <iostream>

using namespace std;

int main() {
	Client C = Client("127.0.0.1");
	return 0;
}