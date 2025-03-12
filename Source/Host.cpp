//Hosts server
#include "../Headers/Host.h"


void Host::start(function<void(string)> callback) {
    toManager = callback;
    options[0] = (make_pair("Name", string("HOST")));

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

        //Log handling
        if(getLogState() == PERSISTANT) {
            fileMutex.lock();
            logFile << msgStr << endl;
            logFile.flush();
            fileMutex.unlock();
        } else if(getLogState() == TEMPORARY) {
            log += msgStr + "\n";
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