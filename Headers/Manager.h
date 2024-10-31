#pragma once
#include "Host.h"
#include "client.h"

using namespace std;


class Manager {
private:
	static void displayMessage(string);
public:
	static void startHost();
	static void startClient();
};