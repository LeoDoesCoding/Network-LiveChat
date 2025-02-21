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
    thread host(&Host::handleInput, this);

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
            users.push_back(User(acceptSocket));
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


//THREAD: For each client, wait for messages and send to all clients.
void Host::recieveMessage(User& sender) {
    toManager("Waiting to recieve from client " + string(sender.name)+ "... ");
    char* msg;
    string msgStr;
    unsigned short msgSize;
    int byteCount;

    while(online) {
        //Get size of message
        recv(sender.socket, (char*)&msgSize, sizeof(msgSize), 0);

        //Recieve full message
        msg = new char[msgSize];
        msg[msgSize] = '\0';
        byteCount = recv(sender.socket, msg, msgSize, 0);
        msgStr = string(msg);

        if(msgStr == "/end" || byteCount <= 0) { //Disconnect user
            if(online) {
                msgStr = sender.name + " has left the chat.";
                if (ready) toManager(msgStr);
                removeUser(sender);
                sendMessage(msgStr);
            }
            return;
        } else if(msgStr == "/setname") {
            recv(sender.socket, (char*)&msgSize, sizeof(msgSize), 0);
            msg = new char[msgSize];
            recv(sender.socket, msg, msgSize, 0);
            sender.name = msg;
            continue;
        }

        if(ready) toManager(msgStr);
        sendMessage(msgStr);
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

//To be send by Connection::sendMessage
void Host::sendConverted(char* message, unsigned short msgSize) {
    usersMutex.lock();
    for(User& user : users) { //Note: There is no error checking here.
        send(user.socket, (char*)&msgSize, sizeof(msgSize), 0);
        int byteCount = send(user.socket, message, strlen(message) + 1, 0);
        if(byteCount < 0) {
            int errCode = WSAGetLastError();
            toManager("Unable to send to client " + user.name + ".");
            removeUser(user);
        }
    }
    usersMutex.unlock();
    delete[] message;
    if(ready) toManager("Sent message to clients.");
}


//Handle config interaction
bool Host::configSet(short choice) {
    if(choice == 2) {
        system("CLS");
        string choice2;
        toManager("Current log mode: " + logString());
        toManager("To change log mode, please enter a number for one of the following options:\n1. No log\n2. Temporary log \n3. Text-file log\n/help\n/back");

        while(true) {
            cin >> choice2;
            if(choice2 == "1") {
                options[1].second = NOLOG;
                toManager("Log mode has been changed to " + logString());
                system("pause");
                break;
            } else if(choice2 == "2") {
                options[1].second = TEMPORARY;
                toManager("Log mode has been changed to " + logString());
                system("pause");
                break;
            } else if(choice2 == "3") {
                options[1].second = PERSISTANT;
                toManager("Log mode has been changed to " + logString());
                system("pause");
                break;
            } else if(choice2 == "/help") {
                system("CLS");
                toManager("Log mode dictates if and how the chat is kept.");
                toManager("No log: No log is being kept of the chat.\n        If a user enters option mode or quits the room, they will lose access to the previous messages.");
                toManager("Temporary log: A local log of chat is kept as a variable and users joining the room can view the whole chat session.\n               Quitting the chat will erase the chat session.");
                toManager("Text-file log: A local log of chat is kept as a text file and users joining the room can view the whole chat session.");
                system("pause");
                system("CLS");
                toManager("Current log mode: " + logString());
                toManager("To change log mode, please enter a number for one of the following options:\n1. No log\n2. Temporary log \n3. Text-file log\n/help\n/back");
                continue;
            } else if(choice2 == "/back") {
                return true;
            }
            toManager("Invalid option given. Please enter a valid option: ");
        }
    }
    return true;
}


void Host::end() {
    toManager("----\nDisconnecitng...");
    online = false;

    //Disconnect all clients
    usersMutex.lock();
    for(User& user : users) {
        send(user.socket, "5", 2, 0);
        send(user.socket, "/end\0", 4, 0);
        closesocket(user.socket);
    }
    usersMutex.unlock();

    toManager("Disconected clients.");

    //Dissconect server
    WSACleanup();
    toManager("Connection ended.");
}