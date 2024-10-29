//User connection to host server
#include "../Headers/Client.h"
using namespace std;


void Client::start(function<void(string)> callback, const wstring IP) {
    toManager = callback;

    //Settup Winsock
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    //Check if Winsock dll found
    if(wsaerr != 0) {
        toManager("Winsock dll not found.");
        end();
    }

    //Settup socket
    mySocket = INVALID_SOCKET;
    mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mySocket == INVALID_SOCKET) {
        toManager("Error at socket() : " + WSAGetLastError());
        end();
    }


    //Connect to socket
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, IP.c_str(), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if(connect(mySocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        toManager("Client: connect() - Failed to connect\nERORR CODE: " + WSAGetLastError());
        end();
    }

    //Connection setup complete successfully.
    online = true;

    //Recieve message thread start
    thread client(&Client::recieveMessage, this);

    //Send messages
    string inpStr;
    char* input;
    unsigned short bufferSize;
    cin.ignore();

    while(online) {
        getline(cin, inpStr);

        if(cin.fail()) {
            toManager("Input error. Exiting.");
            end();
            break;
        }

        if(inpStr == "end") {
            end();
            break;
        }

        bufferSize = inpStr.size()+1; //+1 for null-terminator
        input = new char[bufferSize];
        strcpy_s(input, bufferSize, inpStr.c_str()); //c_str adds null-terminator

        sendMessage(input, bufferSize);
    }

    client.join();
}


//Sends message to sever (Called by handler.cpp).
void Client::sendMessage(char* message, unsigned short msgSize) {
    //Send length
    send(mySocket, (char*)&msgSize, sizeof(msgSize), 0);

    //Send message
    int byteCount = send(mySocket, message, msgSize, 0);
    if(byteCount < 0) {
        toManager("There was an issue sending the message.");
        end();
    }
    delete[] message;
}

//Recieving message from server.
void Client::recieveMessage() {
    char* msg;
    char* msgwName;
    unsigned short msgSize;
    int byteCount;

    while(online) {
        //Get size of message
        recv(mySocket, (char*)&msgSize, sizeof(msgSize), 0); //Note: There is no error checking to see if it is numerical.

        //Recieve full message
        msg = new char[msgSize];
        byteCount = recv(mySocket, msg, msgSize, 0);

        if(byteCount > 0) {
            if(strcmp(msg, "end") == 0) {
                end();
                break;
            }

            toManager(msg);
        }
    }
}


void Client::end() {
    online = false;
    WSACleanup();
    toManager("Connection ended.");
    system("pause");
}