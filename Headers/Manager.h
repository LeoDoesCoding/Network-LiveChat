#pragma once
#include "Host.h"
#include "client.h"


class Manager {
private:
	static void displayMessage(string);
public:
	static void start();
	static void start(const wstring);
};