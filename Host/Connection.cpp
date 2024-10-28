//Maintains connection to client
#include "Connection.h"
using namespace std;

void Connection::start(function<void(string)> callback) {
    toManager = callback;

    //Setup connection
    if(setup()) {
        toManager("Connection successful.");
    } else {
        toManager("Connection failed.");
        end();
    }

    thread host(&Connection::hostMessanger, this);

    toManager("-----");

    //Setup listening
    toManager("Listen start: ");
    if(listen(serverSocket, 1) == SOCKET_ERROR) {
        toManager("Listen(): Error listening on socket");
        end();
    } else {
        toManager("listen() is OK.");
    }

    //Listen for clients
    while(online) {
        //Wait until a connection is made, attempt request acceptence.
        toManager("Listening for a new connection...");
        acceptSocket = accept(serverSocket, NULL, NULL);
        if(acceptSocket == INVALID_SOCKET) {
            toManager("Accept failed: " + WSAGetLastError());
        } else {
            usersMutex.lock();
            users.push_back(User(acceptSocket)); //All users arbuitarily names "USER".
            usersMutex.unlock();
            toManager("New client added.");
            users.back().listen = thread(&Connection::recieveMessage, this, std::ref(users.back()));
        }
    }
    host.join();
}

//Note: Host is present in chat, however does not have their own User struct (no connection to own port).

//Sets up connection in preparation for recieving clients.
bool Connection::setup() {
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
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Check socket is setup
    if(serverSocket == INVALID_SOCKET) {
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
    if(::bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        toManager("Bind() failed: ");
        closesocket(serverSocket);
        return false;
    } else {
        toManager("Bind() OK!");
    }

    //Connection setup complete successfully.
    online = true;
    return true;
}

//THREAD: Host messaging
void Connection::hostMessanger() {
    string inpStr;
    char* input;
    int bufferSize;

    while(online) {
        getline(cin, inpStr);

        if(cin.fail()) {
            cerr << "Input error. Exiting." << endl;
            break;
        }

        if(inpStr == "end") {
            break;
        }

        bufferSize = inpStr.length() + strlen(name) + 3;
        input = new char[bufferSize];
        snprintf(input, bufferSize, "%s: %s", name, inpStr.c_str());

        sendMessage(input);
    }
}


//THREAD: For each client, wait for messages and send to all clients.
void Connection::recieveMessage(User& sender) {
    cout << "Waiting to recieve from client " << sender.name << "... " << endl;
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

        if(byteCount > 0) {
            if(strcmp(msg, "end") == 0) {
                end();
            } else {
                cout << msg << endl;
                msgSize += strlen(sender.name) + 2;
                msgwName = new char[msgSize];
                snprintf(msgwName, msgSize, "%s: %s", sender.name, msg);
                toManager(msgwName); //CALLBACK

                sendMessage(msgwName);
                toManager("Sent message to clients.");
                delete[] msg;
            }
        }
    }
}

void Connection::sendMessage(char* message) {
    usersMutex.lock();
    for(User& user : users) { //Note: There is no error checking here.
        send(user.socket, message, strlen(message) + 1, 0);
    }
    usersMutex.unlock();
    delete[] message;
}



void Connection::end() {
    cout << "----\nDisconnecitng..." << endl;
    online = false;
    //Disconnect all clients
    //NOTE: This is for debugging purposes. In practical application, the server should only disconnect all clients if the server is to disconnect.
    usersMutex.lock();
    for(User& user : users) {
        closesocket(user.socket);
    }
    usersMutex.unlock();

    cout << "Disconected clients." << endl;

    //Dissconect server
    WSACleanup();
    cout << "Connection ended." << endl;
}