#pragma once

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <thread>
#include <string>


#include <WinSock2.h>
#include <ws2tcpip.h>
//-----------------------------------------------------------------------------------------------------------

#pragma comment(lib, "Ws2_32.lib") 
#define SIZE 1024

namespace NetChat {
	class Client {
	private:
		WSADATA wsa;
		SOCKET clientSocket;
		struct sockaddr_in serverAddr;

		int port;                  // port server
		std::string ipAddress;     // ip server


	public:
		Client(int port, const std::string& ipAddress);
		~Client();
	public:
		void init();
		void connect();
		void disconnect() const;

		void start();

		void sendMessage(const std::string& message) const;
		void receiveMessage();   
	}; // class Client
} // namespace NetChat