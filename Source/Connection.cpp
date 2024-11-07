#include "../Headers/Connection.h"

//Handle shared class commands
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

        if(inpStr == "end\0") {
            end();
            break;
        } else if(inpStr == "/config\0") {
            ready = false;
            config();
            continue;
        }

        prepMsg(inpStr);
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
    toManager("Or '/quit' to return to chat room.");

    while(true) {
        cin >> choice;
        if(!cin.fail()) {
            if(choice == "1") {
                char* input;
                string c;
                string old = getName();
                short size;
                toManager("Set text for new name: ");
                cin >> c;
                options[0] = make_pair("Name", c);
                toManager("Name has been changed to " + getName() + ".");
                size = old.length() + 29 + getName().length();
                input = new char[size];
                snprintf(input, size, "%s has changed their name to %s.", old.data(), getName().c_str());
                sendMessage(input, size);
            } else if(choice == "/quit") {
                system("CLS");
                toManager("Returned to chat room.");
                ready = true;
                break;
            } else {
                toManager("Invalid option given. Please enter a valid option: ");
                continue;
            }
            configSet(stoi(choice));
        }
    }
}