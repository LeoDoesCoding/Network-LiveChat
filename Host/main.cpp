// C++ LiveChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>

using namespace std;


int main()
{
    cout << "Sockets test - SERVER" << endl;

    //Settup Winsock
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    //Check if Winsock dll found
    if (wsaerr == 0) {
        cout << "Winsock dll was found." << endl;
        cout << "Status: " << wsaData.szSystemStatus << endl;
    }
    else {
        cout << "The Winsock dll not found." << endl;
    }


    //Settup socket
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //Check socket is setup
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket():" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    } else {
        cout << "Socket() is OK!" << endl;
    }

    //Bind socket to IP and port.
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "Bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "Bind() OK!" << endl;
    }

    //Setup listening process
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "Listen(): Error listening on socket" << WSAGetLastError() << endl;
        return 0;
    } else {
        cout << "listen() is OK, I am waiting for connections." << endl;
    }

    //Wait until a connection is made, attempt request acceptence.
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "Accept failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }


    //Recieve message from client
    char buffer[200];
    int byteCount = recv(acceptSocket, buffer, 200, 0);

    //Check all bytes are recieved.
    char confirmBuff[200];
    if (byteCount > 0) {
        cout << "Message recieved: " << buffer << endl;
        strcpy_s(confirmBuff, "Message recieved!");
        byteCount = send(acceptSocket, confirmBuff, 200, 0);

    } else {
        strcpy_s(confirmBuff, "Issue getting message :(");
        byteCount = send(acceptSocket, confirmBuff, 200, 0);
        WSACleanup();
        cout << "There was an isuse recieving the message." << endl;
    }


    //Connection setup complete successfully.
    cout << "Process Complete. Please continue to end connection." << endl;
    system("pause");
    WSACleanup();
    return 0;

}