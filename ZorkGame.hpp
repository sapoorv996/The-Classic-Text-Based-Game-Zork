#ifndef ZORKGAME_H_
#define ZORKGAME_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "rapidxml.hpp"
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <queue>  
#include "Room.hpp"
#include <sstream>
#include <iterator>

using namespace rapidxml;
using namespace std;

class ZorkGame
{
public:
	ZorkGame(char*);
	~ZorkGame();

	//Functions to setup the game

	//
	void startGame();
	void showRoomDescription();
	void checkUserInput();
	void change_room(string);
	
	//Arrays that contain Room, Container, Item and Creature Nodes
	vector<Room *> roomNodes;
	// vector<Container *> containerNodes;
	// vector<Item *> itemNodes;
	// vector<Creature *> creatureNodes;

	bool isGameOver;
	string userIn; //User input from stdin
	Room * curr_room; //The room I am currently in

};

#endif /* ZORKGAME_H_ */