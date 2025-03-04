#include "../include/clientChat.h"


void printHelp(const std::string& programName) {
    std::cout << "Usage: " << programName << " <IP_ADDRESS> <PORT>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help    Show this help message and exit" << std::endl;
}


int main(int argc, char* argv[]) {
	
	
	for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printHelp(argv[0]);
            return EXIT_SUCCESS;
        }
    }

	
	if (argc < 3) {
		std::cerr << "Error: Not enough arguments." << std::endl;
		printHelp(argv[0]);
		return EXIT_FAILURE;
	}

	const std::string IP_SERVER = argv[1];
	int PORT_SERVER = std::stoi(argv[2]);

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