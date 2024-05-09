//Maintains connection to server

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>

#include "Connection.h"
using namespace std;


boolean Connection::setup() {
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0) {
        cout << "Winsock dll not found." << endl;
        return false;
    } else {
        cout << "Winsock dll found." << endl;
    }

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    } else {
        cout << "Socket() is OK!" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect" << endl;
        WSACleanup();
        return false;
    } else {
        cout << "Client connect() is OK" << endl;
    }



    char buffer[200];
    cout << "Please enter a message: ";
    cin.getline(buffer, 200);
    int byteCount = send(clientSocket, buffer, 200, 0);
    if (byteCount > 0) {
        cout << "Message sent." << endl;
    } else {
        cout << "There was an issue sending the message." << endl;
        WSACleanup();
    }

    //Recieve from sever
    byteCount = recv(clientSocket, buffer, 200, 0);
    if (byteCount > 0) {
        cout << "Server replies: " << buffer << endl;
    } else {
        WSACleanup();
        cout << "Error recieving message." << endl;
    }

    cout << "Process Complete. Please continue to end connection." << endl;
    system("pause");
    WSACleanup();
    return 0;
    return true;
}