#include "clientChat.h"



namespace NetChat {
    Client::Client(int port, const std::string& ipAddress)
        :
        clientSocket(INVALID_SOCKET),
        port(port),
        ipAddress(ipAddress)
        {
            ZeroMemory(&wsa, sizeof(wsa));
            memset(&serverAddr, 0, sizeof(serverAddr));
        }
    //--------------------------------------------------------------------------------------------------------
    void Client::init() {

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
       //inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);


        //init WinSock
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            fprintf(stderr, "WSAStartup failed.\n");
            throw std::runtime_error("WSAStartup failed");
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
        //as soon as we are connected, we start receiving messages!!!
        std::thread(&Client::receiveMessage, this).detach();


        //char ipStr[INET_ADDRSTRLEN];
        const char* ipStr = inet_ntoa(serverAddr.sin_addr);
       // inet_ntop(AF_INET, &(serverAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
        printf("----------------------------------------------------------\n");

        printf("CONNECTED TO SERVER AT: %s:%d\n", ipStr, ntohs(serverAddr.sin_port));
    }
    //--------------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------
    void Client::receiveMessage() {
        char buffer[SIZE];

        while (true) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                //std::string message(buffer, bytesReceived);
                emit messageReceived(QString(buffer));
            }else if (bytesReceived == 0) {
                std::cerr << "Connection closed by server." << std::endl;
                break;
            } else {
                std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
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
    void Client::disconnect() const noexcept{
        closesocket(clientSocket);
        WSACleanup();
        std::cout << "Disconnected from server." << std::endl;
    }
    //--------------------------------------------------------------------------------------------------------
    Client::~Client() {
        disconnect();
    }
} // namespace NetChat
