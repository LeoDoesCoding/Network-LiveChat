#include "../Headers/Connection.h"

//Recieves user input + shared responses.
void Connection::handleInput() {
    string inpStr;
    cin.ignore();

    while(online) {
        getline(cin, inpStr);

        if(cin.fail()) {
            toManager("Input error. Exiting.");
            end();
            break;
        }

        if(inpStr == "/end") {
            end();
            break;
        } else if(inpStr == "/config") {
            ready = false;
            config();
            continue;
        } else {
            inpStr = getName() + ": " + inpStr ;
        }

        //Log handling
        if(getLogState() == PERSISTANT) {
            if(!logFile.is_open()) {
                ofstream logFile(logName, ios::app);
            }
            fileMutex.lock();
            logFile << inpStr << endl;
            logFile.flush();
            fileMutex.unlock();
        } else if(getLogState() == TEMPORARY) {
            log += inpStr + "\n";
        }

        sendMessage(inpStr);
    }
}

void Connection::config() {
    string choice;
    system("CLS");
    short i = 1;
    toManager("Please enter a number: ");
    for(const auto& option : options) {
        toManager(to_string(i) + ". " + option.first);
        i++;
    }
    toManager("Or '/back' to return to chat room.");

    while(true) {
        cin >> choice;

        if(!cin.fail()) {
            int choiceInt;
            try {
                choiceInt = stoi(choice);
            } catch(const invalid_argument& e) { //Non-integer input
                if(choice == "/back") {
                    system("CLS");
                    toManager("Returned to chat room.");
                    cin.ignore();
                    switch(getLogState()) {
                    case TEMPORARY:
                        cout << log;
                        break;
                    case PERSISTANT:
                        fileMutex.lock();
                        ifstream readLog(logName);
                        string text;
                        while(getline(readLog, text)) {
                            toManager(text);
                        }
                        readLog.close();
                        fileMutex.unlock();
                        break;
                    }
                    ready = true;
                    return;
                }
                toManager("Invalid option given. Please enter a valid option: ");
                continue;
            }

            //Integer input
            if(choiceInt == 1) { //Name change
                string input;
                string old = getName();
                toManager("Set text for new name: ");
                cin >> input;
                options[0] = make_pair("Name", input);
                input = old + " has changed their name to " + getName() + ".";
                toManager(input);
                sendMessage(input);

            } else if (choiceInt == 2) {
                system("CLS");
                string choice2;

                while(true) {
                    toManager("Current log mode: " + logString());
                    toManager("To change log mode, please enter a number for one of the following options:\n1. No log\n2. Temporary log \n3. Text-file log\nhelp\n/back");
                    cin >> choice2;
                    if(choice2 == "1") {
                        options[1].second = NOLOG;
                        toManager("Log mode has been changed to " + logString() + ".");
                        logFile.close();
                        system("pause");
                        break;
                    } else if(choice2 == "2") {
                        options[1].second = TEMPORARY;
                        toManager("Log mode has been changed to " + logString() + ".");
                        logFile.close();
                        system("pause");
                        break;
                    } else if(choice2 == "3") {
                        options[1].second = PERSISTANT;
                        toManager("Log mode has been changed to " + logString() + ", with log file " + logName + ".");
                        logFile.open(logName, ios::app);
                        system("pause");
                        break;
                    /* } else if(choice2 == "4") {
                        options[1].second = SERVER;
                        toManager("Log mode has been changed to " + logString());
                        logFile.close();
                        system("pause");
                        break;*/
                    } else if(choice2 == "/help") {
                        system("CLS");
                        toManager("Log mode dictates if and how the chat is kept.");
                        toManager("No log: No log is being kept of the chat.\n        Leaving the chat room or entering opotions mode erases the chat for this user.");
                        toManager("Temporary log: A local log of chat is kept as a variable.\n               Quitting the chat will erase the chat session.");
                        toManager("Text-file log: A local log of chat is kept as a text file and users joining the room can view the whole chat session.");
                        //toManager("Server: Retrives log from the server when joining the chat or exiting options mode. If the server is not keeping a log, no chat is retrived.");
                        system("pause");
                        system("CLS");
                        continue;
                    } else if(choice2 == "/back") {
                        break;
                    }
                    system("CLS");
                    toManager("Invalid option given. Please enter a valid option: ");
                    }
            } else if(choiceInt > options.size() && choiceInt <= 0) { //Invalid option (beyond range of options)
                toManager("Invalid option given. Please enter a valid option: ");
                continue;
            }
            configSet(stoi(choice));
            break;
        }
    }
    config();
}

void Connection::sendMessage(string message) {
    int msgSize = message.length() +1;
    char* msg = new char[message.length() +1];
    strcpy_s(msg, message.length()+1, message.c_str());

    sendConverted(msg, msgSize);
}