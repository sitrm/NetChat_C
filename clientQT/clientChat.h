#ifndef CLIENTCHAT_H
#define CLIENTCHAT_H
//-----------------------------------------------------------------------------------------------------------
#include <iostream>
#include <cassert>
#include <vector>
#include <thread>
#include <string>
//-----------------------------------------------------------------------------------------------------------
#include <WinSock2.h>
#include <ws2tcpip.h>
//-----------------------------------------------------------------------------------------------------------
#include <QObject>
#include "widget.h"
//-----------------------------------------------------------------------------------------------------------
#pragma comment(lib, "Ws2_32.lib")
#define SIZE 1024

namespace NetChat {
    class Client final : public QObject {
        Q_OBJECT
    private:
        WSADATA            wsa;
        SOCKET             clientSocket;
        struct sockaddr_in serverAddr;

        int         port;                         // port server
        std::string ipAddress;                    // ip server

    public:
        explicit Client(int port, const std::string& ipAddress);
        ~Client();
    public:

        void init();
        void connect();
        void disconnect() const noexcept;


        void sendMessage(const std::string& message) const;
        void receiveMessage();

    signals:
        void messageReceived(const QString& message);  // a signal to receive a message

    }; // class Client
} // namespace NetChat

#endif // CLIENTCHAT_H
