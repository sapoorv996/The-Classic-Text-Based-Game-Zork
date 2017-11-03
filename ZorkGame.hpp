#ifndef ZORKGAME_H_
#define ZORKGAME_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "rapidxml.hpp"
#include <vector>
#include <iterator>

#include "Room.hpp"
#include "Creature.hpp"
#include "Container.hpp"
#include "Item.hpp"

using namespace rapidxml;
using namespace std;

class ZorkGame
{
public:
	ZorkGame(char*);
	~ZorkGame();

	//Functions to setup the game

	//Helper functions
	void startGame();
	void showRoomDescription();
	void checkUserInput();
	void change_room(string);
	void show_inventory();
	void take(string);
	void remove_from_vec(vector<string> &vec, int index);
	void remove_from_vec(vector<string> &vec, string str);
	void open(string);
	void gameOver();
	void read(string);
	void drop(string);
	void put(string, string);
	
	//Arrays that contain Room, Container, Item and Creature Nodes
	vector<Room *> roomNodes;
	vector<Container *> containerNodes;
	vector<Item *> itemNodes;
	vector<Creature *> creatureNodes;

	//Inventory list
	vector<string> inventory;

	bool isGameOver;
	string userIn; //User input from stdin
	Room * curr_room; //The room I am currently in

};

#endif /* ZORKGAME_H_ */