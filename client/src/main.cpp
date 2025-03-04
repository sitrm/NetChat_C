#include "../include/clientChat.h"



int main() {
	int PORT_SERVER = 8888;
	const std::string IP_SERVER = "127.0.0.1";
	
	try{
		NetChat::Client client(PORT_SERVER, IP_SERVER);
		client.startSerialize();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}



	return 0;
}