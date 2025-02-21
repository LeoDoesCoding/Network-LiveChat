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
                input = old + "has changed their name to " + getName() + ".";
                toManager(input);
                sendMessage(input);

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