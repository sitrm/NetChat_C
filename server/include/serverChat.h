#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
//-----------------------------------------------------------------------------------------------------------
#include <WinSock2.h>
#include <ws2tcpip.h>
//-----------------------------------------------------------------------------------------------------------

#pragma comment(lib, "Ws2_32.lib") 
#define SIZE 1024


namespace NetChat {
	class Server {
	private:
		WSADATA wsa;                    
		SOCKET  serverSocket;            // descriptor socket 
		std::vector<SOCKET> clients;     // vector connected client 
		struct sockaddr_in serverAddr;


		int port;
		std::string ipAddress;

		std::shared_mutex clientMutex;  // mutex for thread safety

		//void handleClient(SOCKET clientSocket);
		void handleClientSerialize(SOCKET clientSocket);

	public:
		Server(int port, const std::string& ipAddress);
		~Server();
	public:
		void init();

		void start();
		void stop();
	public:
		static void logCurrentTime();
	}; // class Server
} // namespace NetChat
