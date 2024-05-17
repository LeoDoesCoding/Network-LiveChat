//Maintains connection to client

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>
#include <string>
#include <thread>

#include "Connection.h"
using namespace std;

void Connection::start() {
    //Setup connection
    if (setup()) {
        cout << "Connection successful." << endl;
    } else {
        cout << "Connection failed." << endl;
        end();
    }

    //Setup listening
    cout << "Listen start: " << endl;
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "Listen(): Error listening on socket" << WSAGetLastError() << endl;
        end();
    } else {
        cout << "listen() is OK." << endl;
    }

    //Listen for clients
    while (online) {
        //Wait until a connection is made, attempt request acceptence.
        cout << "Listening for a new connection..." << endl;
        acceptSocket = accept(serverSocket, NULL, NULL);
        if (acceptSocket == INVALID_SOCKET) {
            cout << "Accept failed: " << WSAGetLastError() << endl;
            continue;
        } else {
            usersMutex.lock();
            users.push_back(User(acceptSocket, "USER")); //All users arbuitarily names "USER".
            usersMutex.unlock();
            cout << "New client added." << endl;
            threads.emplace_back(&Connection::recieveMessage, this, ref(users.back()));
        }
    }
}

//Sets up connection in preparation for recieving clients.
bool Connection::setup() {
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
    } else {
        cout << "The Winsock dll not found." << endl;
        return false;
    }


    //Settup socket
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //Check socket is setup
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket():" << WSAGetLastError() << endl;
        return false;
    } else {
        cout << "Socket() is OK!" << endl;
    }

    //Bind socket to IP and port.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "Bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        return false;
    } else {
        cout << "Bind() OK!" << endl;
    }


    //Connection setup complete successfully.
    online = true;
    return true;
}


//THREAD: For each client, wait for messages and send to all clients.
void Connection::recieveMessage(User& sender) {
    cout << "Waiting to recieve from client " << sender.name << "... " << endl;
    char buffer[200];

    while (online) {
        int byteCount = recv(sender.socket, buffer, 200, 0);
        if (byteCount > 0) {
            if (strcmp(buffer, "end") == 0) {
                end();
            } else {
                cout << "Messaged recieved from " << sender.name << ": " << buffer << endl;

                //Send to each connected client
                usersMutex.lock();
                for (User& user : users) { //Note: There is no error checking here.
                    send(user.socket, buffer, 200, 0);
                }
                usersMutex.unlock();
                cout << "Sent message to clients. " << endl;
            }
        } else {
            cout << "Error recieving message." << endl;
            end();
        }
    }
}


void Connection::end() {
    cout << "Disconnecitng..." << endl;
    //Disconnect all clients
    //NOTE: This is for debugging purposes. In practical application, the server should only disconnect all clients if the server is to disconnect.
    char buffer[4];
    strcpy_s(buffer, "end");

    usersMutex.lock();
    for (User& user : users) {
        closesocket(user.socket);
    }
    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
    usersMutex.unlock();
    cout << "Disconected clients." << endl;

    //Dissconect server
    online = false;
    WSACleanup();
    cout << "Connection ended." << endl;
    system("pause");
}