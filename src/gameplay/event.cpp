#include "event.h"

GameEvent cleanEvent(GameEvent eventObject) {
	eventObject.name = "";
	eventObject.description = "";

	eventObject.needed[0].value = 0;
	eventObject.needed[0].symbol = '\0';
	eventObject.needed[1].value = 0;
	eventObject.needed[1].symbol = '\0';
	eventObject.cost[0].value = 0;
	eventObject.cost[0].symbol = '\0';
	eventObject.cost[1].value = 0;
	eventObject.cost[1].symbol = '\0';
	eventObject.reward[0].value = 0;
	eventObject.reward[0].symbol = '\0';
	eventObject.reward[1].value = 0;
	eventObject.reward[1].symbol = '\0';
	eventObject.consequense[0].value = 0;
	eventObject.consequense[0].symbol = '\0';
	eventObject.consequense[1].value = 0;
	eventObject.consequense[1].symbol = '\0';

	eventObject.evilEvent = false;

	return eventObject;
}

GameEvent readEvent(GameEvent eventObject, int eventNumber) {
	std::ifstream myFile;
	std::string row;
	std::string cell;
	int fill = 0;
	int numberOfDataPoints = 0;

	myFile.open("event_sheet.csv");
	if (myFile.is_open()) {
		for (int i = 0; i <= eventNumber; i++)
			getline(myFile, row);
		//std::cout << "Found line: " << row << std::endl;
		std::stringstream ss(row);

		while (ss.good()) {
			getline(ss, cell, ',');
			//std::cout << "Cell: " << cell << std::endl;
			switch (fill) {
			case 0: // Name
				eventObject.name = cell;
				fill++;
				break;
			case 1: // Description
				eventObject.description = cell;
				fill++;
				break;
			case 2: // Level Needed to Defend
				numberOfDataPoints = cell[0] - 48;
				for (int i = 0; i < numberOfDataPoints; i++) { // Works with max of 2
					eventObject.needed[i].value = cell[2 + (i * 4)];
					eventObject.needed[i].symbol = cell[4 + (i * 4)];
				}
				fill++;
				break;
			case 3: // Cost for Defending
				numberOfDataPoints = cell[0] - 48;
				for (int i = 0; i < numberOfDataPoints; i++) { // Works with max of 2
					eventObject.cost[i].value = cell[2 + (i * 4)];
					eventObject.cost[i].symbol = cell[4 + (i * 4)];
				}
				fill++;
				break;
			case 4: // Full Defense Reward
				numberOfDataPoints = cell[0] - 48;
				for (int i = 0; i < numberOfDataPoints; i++) { // Works with max of 2
					eventObject.reward[i].value = cell[2 + (i * 4)];
					eventObject.reward[i].symbol = cell[4 + (i * 4)];
				}
				fill++;
				break;
			case 5: // Non Defended Consequence
				numberOfDataPoints = cell[0] - 48;
				for (int i = 0; i < numberOfDataPoints; i++) { // Works with max of 2
					eventObject.consequense[i].value = cell[2 + (i * 4)];
					eventObject.consequense[i].symbol = cell[4 + (i * 4)];
				}
				fill++;
				break;
			case 6: // Good or Bad Event
				eventObject.evilEvent = cell[0] - 48;
			}
		}
		fill = 0;
	}
	else
		printf("Could not open file");

	myFile.close();

	printEvent(eventObject);

	return eventObject;
}

void printEvent(GameEvent eventObject) {
	std::cout << "Event Name: " << eventObject.name << std::endl;
	std::cout << "Event Description: " << eventObject.description << std::endl;

	if (eventObject.needed[1].value == 0) { // Needed
		if (eventObject.needed[0].value - 48 != 0)
			std::cout << "Needed To Defend \n\tSkill: " << eventObject.needed[0].symbol
			<< "\n\tLevel: " << eventObject.needed[0].value - 48 << std::endl;
		else
			std::cout << "Needed To Defend \n\tThere is no need for defending" << std::endl;
	}
	else {
		std::cout << "Needed To Defend \n\tSkill: " << eventObject.needed[0].symbol
			<< "\n\tLevel: " << eventObject.needed[0].value - 48
			<< "\n\tAND\n\tSkill: " << eventObject.needed[1].symbol
			<< "\n\tLevel: " << eventObject.needed[1].value - 48 << std::endl;
	}

	if (eventObject.cost[1].value == 0) { // Cost
		if (eventObject.cost[0].value != 0)
			std::cout << "Cost for Defending \n\tResource: " << eventObject.cost[0].symbol
			<< "\n\tAmount: " << eventObject.cost[0].value - 48 << std::endl;
		else
			std::cout << "Cost for Defending \n\tThere is nothing to pay" << std::endl;
	}
	else {
		std::cout << "Cost for Defending \n\tResource: " << eventObject.cost[0].symbol
			<< "\n\tAmount: " << eventObject.cost[0].value - 48
			<< "\n\tAND\n\tResource: " << eventObject.cost[1].symbol
			<< "\n\tAmount: " << eventObject.cost[1].value - 48 << std::endl;
	}

	if (eventObject.reward[1].value == 0) { // Reward
		std::cout << "Reward \n\tResource: " << eventObject.reward[0].symbol
			<< "\n\tAmount: " << eventObject.reward[0].value - 48 << std::endl;
	}
	else {
		std::cout << "Cost for Defending \n\tResource: " << eventObject.reward[0].symbol
			<< "\n\tAmount: " << eventObject.reward[0].value - 48
			<< "\n\tAND\n\tResource: " << eventObject.reward[1].symbol
			<< "\n\tAmount: " << eventObject.reward[1].value - 48 << std::endl;
	}

	if (eventObject.consequense[1].value == 0) { // Consequense
		if (eventObject.consequense[0].value != 0)
			std::cout << "Cost for Failing to Defend \n\tResource: " << eventObject.consequense[0].symbol
			<< "\n\tAmount: " << eventObject.consequense[0].value - 48 << std::endl;
		else
			std::cout << "Cost for Failing to Defend \n\tThere is no consequence" << std::endl;
	}
	else {
		std::cout << "Cost for Failing to Defend \n\tResource: " << eventObject.consequense[0].symbol
			<< "\n\tAmount: " << eventObject.consequense[0].value - 48
			<< "\n\tAND\n\tResource: " << eventObject.consequense[1].symbol
			<< "\n\tAmount: " << eventObject.consequense[1].value - 48 << std::endl;
	}

	std::cout << "Event Alignment: " << eventObject.evilEvent << std::endl;

	std::cout << std::endl;
}