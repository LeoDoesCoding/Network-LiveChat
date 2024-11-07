//User connection to host server
#include "../Headers/Client.h"


bool Client::start(function<void(string)> callback, const wstring IP) {
    toManager = callback;

    //Settup Winsock
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    //Check if Winsock dll found
    if(wsaerr != 0) {
        toManager("Winsock dll not found.");
        WSACleanup();
        return false;
    }

    //Settup socket
    mySocket = INVALID_SOCKET;
    mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mySocket == INVALID_SOCKET) {
        toManager("Error at socket() : " + WSAGetLastError());
        WSACleanup();
        return false;
    }


    //Connect to socket
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    if(InetPton(AF_INET, IP.c_str(), &clientService.sin_addr.s_addr) <= 0) {
        toManager("Connection failiure with error: " + WSAGetLastError());
        WSACleanup();
        return false;
    }


    clientService.sin_port = htons(port);
    if(connect(mySocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        toManager("Client: connect() - Failed to connect\nERORR CODE: " + WSAGetLastError());
        WSACleanup();
        return false;
    }

    //Connection setup complete successfully.
    online = true;
    toManager("Successfully connected. You may now send messages.");

    //Recieve and send message thread start
    thread sendMsg(&Client::handleInput, this);
    thread recMsg(&Client::recieveMessage, this);

    sendMsg.join();
    recMsg.join();
    return true;
}

//Called by handleInput()
void Client::prepMsg(string msg) {
    unsigned short bufferSize;
    char* input;

    bufferSize = msg.size() + 1; //+1 for null-terminator
    input = new char[bufferSize];
    strcpy_s(input, bufferSize, msg.c_str()); //c_str adds null-terminator

    sendMessage(input, bufferSize);
    delete[] input;
}


//Sends message to sever
void Client::sendMessage(char* message, unsigned short msgSize) {
    //Send length
    send(mySocket, (char*)&msgSize, sizeof(msgSize), 0);

    //Send message
    int byteCount = send(mySocket, message, msgSize, 0);
    if(byteCount < 0) {
        int errCode = WSAGetLastError();
        if(errCode == WSAEINTR || errCode == WSAENOTSOCK) {
            return;
        }
        toManager("There was an issue sending the message.");
        end();
    }
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
        if(online) {
            msg = new char[msgSize];
            byteCount = recv(mySocket, msg, msgSize, 0);

            if(byteCount > 0) {
                if(strcmp(msg, "\0") == 0) {
                    return;
                }

                if(strcmp(msg, "end\0") == 0) {
                    toManager("Server has disconnected.");
                    end();
                    break;
                }

                toManager(msg);

            } else {
                toManager("Server has disconnected.");
                end();
            }
        }
    }
}


bool Client::configSet(short choice) {
    if(choice == 1) {
        string c = "/setname";
        sendMessage(c.data(), 9);
        sendMessage(const_cast<char*>(getName().c_str()), getName().length());
    }
    return true;
}


void Client::end() {
    online = false;
    WSACleanup();
    toManager("Connection ended.");
}