//Hosts server
#include "../Headers/Host.h"


void Host::start(function<void(string)> callback) {
    toManager = callback;

    //Setup connection
    if(setup()) {
        toManager("Connection successful.");
    } else {
        toManager("Connection failed.");
        end();
    }

    //Host messanger thread start
    thread host(&Host::hostMessanger, this);

    toManager("-----");

    //Setup listening
    toManager("Listen start: ");
    if(listen(mySocket, 1) == SOCKET_ERROR) {
        toManager("Listen(): Error listening on socket");
        end();
    } else {
        toManager("listen() is OK.");
    }

    //Listen for clients
    while(online) {
        //Wait until a connection is made, attempt request acceptence.
        toManager("Listening for a new connection...");
        acceptSocket = accept(mySocket, NULL, NULL);
        if(acceptSocket == INVALID_SOCKET) {
            toManager("Accept failed: " + WSAGetLastError());
        } else {
            usersMutex.lock();
            users.push_back(User(acceptSocket)); //All users arbuitarily names "USER".
            usersMutex.unlock();
            toManager("New client added.");
            thread listen(&Host::recieveMessage, this, ref(users.back()));
            listen.detach();
        }
    }
    host.join();
}

//Sets up connection in preparation for recieving clients.
bool Host::setup() {
    //Settup Winsock
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    //Check if Winsock dll found
    if(wsaerr == 0) {
        toManager("Winsock dll was found.");
        toManager("Status: " + string(wsaData.szSystemStatus));
    } else {
        toManager("The Winsock dll not found.");
        return false;
    }

    //Settup socket
    mySocket = INVALID_SOCKET;
    mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Check socket is setup
    if(mySocket == INVALID_SOCKET) {
        toManager("Error at socket()");
        return false;
    } else {
        toManager("Socket() is OK!");
    }

    //Bind socket to IP and port.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);
    if(::bind(mySocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        toManager("Bind() failed: ");
        closesocket(mySocket);
        return false;
    } else {
        toManager("Bind() OK!");
    }

    //Connection setup complete successfully.
    online = true;
    return true;
}

//THREAD: Host messaging
void Host::hostMessanger() {
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

        if(inpStr == "end\0") {
            end();
            break;
        }

        bufferSize = inpStr.length() + strlen(name) + 3; //+3 for ": " and null-terminator
        input = new char[bufferSize];
        snprintf(input, bufferSize, "%s: %s", name, inpStr.c_str());

        sendMessage(input, bufferSize);
    }
}


//THREAD: For each client, wait for messages and send to all clients.
void Host::recieveMessage(User& sender) {
    toManager("Waiting to recieve from client " + string(sender.name)+ "... ");
    char* msg;
    char* msgwName;
    unsigned short msgSize;
    int byteCount;

    while(online) {
        //Get size of message
        recv(sender.socket, (char*)&msgSize, sizeof(msgSize), 0); //Note: There is no error checking to see if it is numerical.

        //Recieve full message
        msg = new char[msgSize];
        byteCount = recv(sender.socket, msg, msgSize, 0);

        if(strcmp(msg, "end\0") == 0 || byteCount <= 0) { //Disconnect user
            if(online) {
                msgSize += strlen(sender.name) + 20;
                msgwName = new char[msgSize];
                snprintf(msgwName, msgSize, "%s has left the chat.", sender.name);
                toManager(msgwName);
                removeUser(sender);
                sendMessage(msgwName, msgSize);
            }
            return;
        }

        msgSize += strlen(sender.name) + 2;
        msgwName = new char[msgSize];
        snprintf(msgwName, msgSize, "%s: %s", sender.name, msg);

        toManager(msgwName);
        sendMessage(msgwName, msgSize);
        toManager("Sent message to clients.");
    }
}

//Erase user
void Host::removeUser(User& user) {
    usersMutex.lock();
    closesocket(user.socket);
    users.erase(remove_if(users.begin(), users.end(), [&](User& iuser) {
        return iuser == user;
        }), users.end());
    usersMutex.unlock();
}


void Host::sendMessage(char* message, unsigned short msgSize) {
    usersMutex.lock();
    for(User& user : users) { //Note: There is no error checking here.
        send(user.socket, (char*)&msgSize, sizeof(msgSize), 0);
        send(user.socket, message, strlen(message) + 1, 0);
    }
    usersMutex.unlock();
    delete[] message;
}



void Host::end() {
    toManager("----\nDisconnecitng...");
    online = false;

    //Disconnect all clients
    usersMutex.lock();
    for(User& user : users) {
        send(user.socket, "4", 2, 0);
        send(user.socket, "end\0", 4, 0);
        closesocket(user.socket);
    }
    usersMutex.unlock();

    toManager("Disconected clients.");

    //Dissconect server
    WSACleanup();
    toManager("Connection ended.");
}