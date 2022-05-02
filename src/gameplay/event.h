#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <windows.h>
#include <limits.h>
#include <filesystem>

constexpr int TOTAL_EVENTS = 19;
constexpr int MAX_NUMBER_OF_RESOURCES = 2;

class Handler {
public:
	int value;
	char symbol;
};

class GameEvent {
public:
	std::string name;
	std::string description;

	Handler needed[MAX_NUMBER_OF_RESOURCES];
	Handler cost[MAX_NUMBER_OF_RESOURCES];
	Handler reward[MAX_NUMBER_OF_RESOURCES];
	Handler consequense[MAX_NUMBER_OF_RESOURCES];

	bool evilEvent;
};

GameEvent cleanEvent(GameEvent eventObject); // Meant to zero out values
GameEvent readEvent(GameEvent eventObject, int eventNumber); // meant to read one line of csv file and fill out the event with data
void printEvent(GameEvent eventObject); // Print out values of event