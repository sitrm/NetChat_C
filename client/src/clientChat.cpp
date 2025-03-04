#include "../include/clientChat.h"
#include "../../messageSerialize/include/message.h"


namespace NetChat {
	Client::Client(int port, const std::string& ipAddress)
		:
		port(port),
		ipAddress(ipAddress),
		clientSocket(INVALID_SOCKET) {
			ZeroMemory(&wsa, sizeof(wsa));
			memset(&serverAddr, 0, sizeof(serverAddr));
		}
	//--------------------------------------------------------------------------------------------------------
	void Client::init() {

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);


		//init WinSock
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			fprintf(stderr, "WSAStartup failed.\n");
			throw std::runtime_error("WSAStartup failed");
			__debugbreak();
		}
		printf("WSA init seccess!\n");

		// socket()
		printf("Creating Socket client...\n");
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket == INVALID_SOCKET) {
			WSACleanup();
			throw std::runtime_error("Socket creation failed\n");
		}
		printf("Success Socket(%d) client!\n", static_cast<int>(clientSocket));
	}
	//--------------------------------------------------------------------------------------------------------
	void Client::connect() {
		init();

		if (::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			closesocket(clientSocket);
			WSACleanup();
			throw std::runtime_error("Connect FAILED");
		}
		char ipStr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(serverAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
		printf("----------------------------------------------------------\n");

		printf("CONNECTED TO SERVER AT: %s:%d\n", ipStr, ntohs(serverAddr.sin_port));
	}
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	void Client::receiveMessage() {
		char buffer[SIZE];
		int bytesReceived;
		while (true) {
			bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

			if (bytesReceived > 0) {
				std::string message(buffer, bytesReceived);
				std::cout  << message << std::endl;
			} else if (bytesReceived == 0) {
                std::cout << "Server disconnected." << std::endl;
                break;
            } else {
                std::cerr << "Recv() failed: " << WSAGetLastError() << std::endl;
                break;
            }

		}
	}
	//--------------------------------------------------------------------------------------------------------
	void Client::receiveMessageSerialize() {
		std::vector<uint8_t> buffer(SIZE);
		
		while (true) {
			int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(buffer.data()), SIZE, 0);

			if (bytesReceived > 0) {
				Core::Message message = Core::Message::deserialize(buffer);
				std::cout << message.getUsername() << ": " << message.getMessage() << std::endl;
			}else if (bytesReceived == 0) {
                std::cout << "Server disconnected." << std::endl;
                break;
            } else {
                std::cerr << "Recv() failed: " << WSAGetLastError() << std::endl;
                break;
            }

		}
	}
	//--------------------------------------------------------------------------------------------------------
	void Client::sendMessage(const std::string& message) const {
		if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
			std::cerr << "Send failed" << std::endl;
		}
	}
	//--------------------------------------------------------------------------------------------------------
	void Client::sendMessage(const std::vector<uint8_t> buffer) const {
		if (buffer.empty()) {
			std::cerr << "Buffer is empty!" << std::endl;
			return;
		}
		if (send(clientSocket, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0) == SOCKET_ERROR) {
			std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
		}
	}
	//--------------------------------------------------------------------------------------------------------
	void Client::start() {

		try {
            connect();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return ;
        }
	
		std::thread(&Client::receiveMessage, this).detach();
		std::cout << "To leave the chat, enter \"exit\" or Ctrl+C" << std::endl;
		std::cout << "Please enter your userName: ";
		std::string name;
		std::cin >> name;
		std::cout <<"Seccess! To send a text, write a message and press Enter." <<std::endl;
		printf("----------------------------------------------------------\n");

		while (true) {
			std::string message = name + ": ";
			std::string data;
			//std::cout << "Input messsage: ";
			std::getline(std::cin, data);
			if (data == "") {
				continue;
			}
			if(data == "exit"){
				disconnect();
			}

			message += data;
			sendMessage(message);
		}
	}
	//--------------------------------------------------------------------------------------------------------
	void Client::startSerialize() {
		try {
			connect();
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			//exit(EXIT_FAILURE);
		}
		std::thread(&Client::receiveMessageSerialize, this).detach();
		std::cout << "To leave the chat, enter \"exit\" or Ctrl+C" << std::endl;
		std::cout << "Please enter your userName: ";
		std::string name;
		std::cin >> name;
		std::cout << std::endl;

		while (true) {
			std::string data;
			std::getline(std::cin, data);
			if (data.empty()) continue;
			if (data == "exit") {
				disconnect();
				break;
			}
			Core::Message message(name, data);
			std::vector<uint8_t> buffer = message.serialize();
			sendMessage(buffer);
		}
	}	
	//--------------------------------------------------------------------------------------------------------
	void Client::disconnect() const {
		closesocket(clientSocket);
		WSACleanup();
		std::cout << "Disconnected from server." << std::endl;
	}
	//--------------------------------------------------------------------------------------------------------
	Client::~Client() {
		disconnect();
	}

} // namespace NetChat
