#include "../include/serverChat.h"



int main() {

	int PORT = 8888;
	const std::string ipAddress = "127.0.0.1";

	try {
		NetChat::Server server(PORT, ipAddress);
		server.start();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}