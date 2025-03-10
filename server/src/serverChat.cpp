#include "../include/serverChat.h"
#include "../../messageSerialize/include/message.h"

#pragma warning(disable: 4996) 

namespace NetChat {
	Server::Server(int port, const std::string& ipAddress)
		:
		port(port),
		ipAddress(ipAddress),
		wsa{ 0 },
		serverSocket(INVALID_SOCKET) {
			memset(&serverAddr, 0, sizeof(serverAddr));
		}

	//---------------------------------------------------------------------------------------------
	void Server::init() {

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

		//init WinSock
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			throw std::runtime_error("WSA init FAILED!\n");
			// __debugbreak();
		}
		printf("WSA init seccess!\n");

		// socket()
		printf("Creating Socket server...\n");
		serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (serverSocket == INVALID_SOCKET) {
			WSACleanup();
			throw std::runtime_error("Socket creation FAILED!\n");
		}
		printf("Success Socket(%d) server!\n", static_cast<int>(serverSocket));

		printf("bind Socket...\n");
		if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			closesocket(serverSocket);
			WSACleanup();
			throw std::runtime_error("Bind ERROR!");
		}
		printf("Socket binded!\n");


		printf("SERVER STARTED AT:%s:%d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
		printf("---------------------------------------------------------------\n");

		if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
			closesocket(serverSocket);
			WSACleanup();
			throw std::runtime_error("Listen() ERROR!\n");
		}
	}	
	//---------------------------------------------------------------------------------------------
	void Server::handleClient(SOCKET clientSocket) {
		char buffer[SIZE];
		int bytesReceived = 0;

		while (true) {
			bytesReceived = recv(clientSocket, buffer, SIZE, 0);
			if (bytesReceived <= 0) {
				Server::logCurrentTime();
				std::cout << "Client(" << clientSocket << ") disconnected" << std::endl;
				break;
			}
			// lock clinet list to prevent concurrent modifications
			std::lock_guard<std::mutex> lock(clientMutex);

			for (SOCKET client : clients) {
				if (client != clientSocket) {
					if ((send(client, buffer, bytesReceived, 0)) == SOCKET_ERROR) {
						printf("send() FAILED...%d\n", WSAGetLastError());
						//exit(EXIT_FAILURE);
					}
					std::string message(buffer, bytesReceived);
					Server::logCurrentTime();
					std::cout << clientSocket << " send message to "<< client  << " | DATA: " << message << std::endl;
				}
			}
		}
		// Remove client from the list and close the socket
		{
			std::lock_guard<std::mutex> lock(clientMutex);                                       // that safe 
			clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
		}
		//delete lock - unlock mutex
		closesocket(clientSocket);
	}
	//---------------------------------------------------------------------------------------------
	void Server::handleClientSerialize(SOCKET clientSocket) {
		std::vector<uint8_t> buffer(SIZE);
		int bytesReceived = 0;

		while (true) {
			bytesReceived = recv(clientSocket, reinterpret_cast<char*>(buffer.data()), SIZE, 0);
			if (bytesReceived <= 0) {
				Server::logCurrentTime();
				std::cout << "Client(" << clientSocket << ") disconnected" << std::endl;
				break;
			}

			std::vector<SOCKET> clientsCopy;
			{
				// lock clinet list to prevent concurrent modifications
				std::lock_guard<std::mutex> lock(clientMutex);
				clientsCopy = clients;

			}
			
			for (SOCKET client : clientsCopy) {
				if (client != clientSocket) {
					if ((send(client, reinterpret_cast<char*>(buffer.data()), bytesReceived, 0)) == SOCKET_ERROR) {
						std::cerr << "Send() failed: " << WSAGetLastError() << std::endl;
						//exit(EXIT_FAILURE);
					}
					Core::Message message = Core::Message::deserialize(buffer);

					Server::logCurrentTime();
					
					std::cout << clientSocket << " send message to " << client << " | DATA:[";
					std::cout << message.getUsername() << "]: " << message.getMessage() << std::endl;
				}
			}
		}
		// Remove client from the list and close the socket
		{
			std::lock_guard<std::mutex> lock(clientMutex);     // that safe 
			clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
		}
		//delete lock - unlock mutex
		closesocket(clientSocket);
	}
	//---------------------------------------------------------------------------------------------
	void Server::start() {

		init();

		while (true) {

			sockaddr_in clientAddr;
			int clientAddrSize = sizeof(clientAddr);

 			SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

			if (clientSocket == SOCKET_ERROR) {
				std::cerr << "Accept() FAILED\n";
				continue;
			}
			Server::logCurrentTime();

			std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port);
			std::cout << "| Socket client = " << clientSocket << std::endl;

			// Lock client list before adding new client
			{
				std::lock_guard<std::mutex> lock(clientMutex);
				clients.push_back(clientSocket);
			}
			//delete lock

			//the thread fo client processing
			std::thread(&Server::handleClientSerialize, this, clientSocket).detach();
		}
	}
	//---------------------------------------------------------------------------------------------
	Server::~Server() {
		for (SOCKET client : clients) {
			closesocket(client);
		}

		WSACleanup();
		closesocket(serverSocket);
		std::cout << "Server stopped." << std::endl;
	}
	//---------------------------------------------------------------------------------------------
	void Server::logCurrentTime() {
		auto now = std::chrono::system_clock::now(); // Текущее время
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm = *std::localtime(&now_time); // Преобразуем в локальное время

		std::cout << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] ";
	}
	//---------------------------------------------------------------------------------------------

} // namespace NetChat