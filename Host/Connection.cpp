//Maintains connection to client

#include <iostream>
#include <winsock2.h>
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

    cout << "-----" << endl;

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
        } else {
            usersMutex.lock();
            users.push_back(User(acceptSocket, "USER")); //All users arbuitarily names "USER".
            usersMutex.unlock();
            cout << "New client added." << endl;
            users.back().listen = thread(&Connection::recieveMessage, this, std::ref(users.back()));
        }
    }
}

//Sets up connection in preparation for recieving clients.
bool Connection::setup() {
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
    char* inMsg = nullptr;
    char* outMsg;
    unsigned short msgSize;
    int byteCount;

    while (online) {

        char* inMsg = nullptr;
        //Get size of message
        recv(sender.socket, (char*)&msgSize, sizeof(msgSize), 0); //Note: There is no error checking to see if it is numerical.
        cout << "Recieved: " << msgSize << "." << endl;

        //Recieve full message
        inMsg = new char[msgSize];
        byteCount = recv(sender.socket, inMsg, msgSize, 0);

        if (byteCount > 0) {
            if (strcmp(inMsg, "end") == 0) {
                end();
            } else {
                cout << "Messaged recieved from " << sender.name << ": " << inMsg << endl;

                //Send to each connected client
                msgSize += sender.name.length() + 3; //3 for ": " and "/0"
                outMsg = new char[msgSize];
                snprintf(outMsg, msgSize, "%s: %s", sender.name.c_str(), inMsg);

                usersMutex.lock();
                for (User& user : users) { //Note: There is no error checking here.
                    send(user.socket, outMsg, msgSize, 0);
                }
                usersMutex.unlock();
                cout << "Sent message to clients. " << endl;
                delete [] outMsg;
                delete[] inMsg;
            }
        }
    }
}


void Connection::end() {
    cout << "----\nDisconnecitng..." << endl;
    online = false;
    //Disconnect all clients
    //NOTE: This is for debugging purposes. In practical application, the server should only disconnect all clients if the server is to disconnect.
    usersMutex.lock();
    for (User& user : users) {
        closesocket(user.socket);
    }
    usersMutex.unlock();
 
    cout << "Disconected clients." << endl;

    //Dissconect server
    WSACleanup();
    cout << "Connection ended." << endl;
}